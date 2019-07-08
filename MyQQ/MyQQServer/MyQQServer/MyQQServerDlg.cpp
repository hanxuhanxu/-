// MyQQServerDlg.cpp : ʵ���ļ�
#include "stdafx.h"
#include "MyQQServer.h"
#include "MyQQServerDlg.h"
#include "afxdialogex.h"
#include"TCPMediator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMyQQServerDlg �Ի���

CMyQQServerDlg::CMyQQServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyQQServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pTCPMediator=new CTCPMediator(this);
	m_pMySql=new CMySql;
}

void CMyQQServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_USER, m_lstUser);
	DDX_Control(pDX, IDC_LIST2, m_lstMsg);
}

BEGIN_MESSAGE_MAP(CMyQQServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_BEGIN, &CMyQQServerDlg::OnBnClickedBtnBegin)
	ON_BN_CLICKED(IDC_BTN_MIN, &CMyQQServerDlg::OnBnClickedBtnMin)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CMyQQServerDlg::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_END, &CMyQQServerDlg::OnBnClickedBtnEnd)
	ON_MESSAGE(UM_ONLINE_ADD,&CMyQQServerDlg::OnUserAdd)
	ON_MESSAGE(UM_OFFLINE_ADD,&CMyQQServerDlg::OnUserOffAdd)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CMyQQServerDlg ��Ϣ�������


BOOL CMyQQServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	if(!m_pMySql->ConnectMySql("localhost","han","hanxu4414708","user"))
	{
		TRACE("%d\n",GetLastError());
		MessageBox(_T("open failed"));
		return FALSE;
	}
	//MessageBox(_T("connect MySql success"));
	//CMyQQServerDlg *dlg=NULL;
	//dlg=new CMyQQServerDlg;
	//dlg->Create(IDD_MYQQSERVER_DIALOG);
	//dlg->ShowWindow(SW_SHOW);


	//if(!m_pTCPMediator->open())
	//{
	//	TRACE("%d\n",GetLastError());
	//	MessageBox(_T("open failed"));
	//	return FALSE;
	//}

	//if(!m_pMySql->ConnectMySql("localhost","han","hanxu4414708","user"))
	//{
	//	TRACE("%d\n",GetLastError());
	//	MessageBox(_T("open failed"));
	//	return FALSE;
	//}
	//MessageBox(_T("connect MySql success"));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMyQQServerDlg::OnBnClickedBtnBegin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
			if(!m_pTCPMediator->open())
	{
		TRACE("%d\n",GetLastError());
		MessageBox(_T("open failed"));
		return ;
	}


	((CButton *)GetDlgItem(IDC_BTN_BEGIN))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_BTN_END))->EnableWindow(TRUE);
}

void CMyQQServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMyQQServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CDialogEx::OnPaint();
		CPaintDC dc(this);
		CRect rc;
		GetClientRect(&rc);
		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);
		CBitmap bmpBackground;
		bmpBackground.LoadBitmap(IDB_BITMAP1);

		BITMAP bitmap;
		bmpBackground.GetBitmap(&bitmap);
		CBitmap* pbmpPri = dcMem.SelectObject(&bmpBackground);
		dc.StretchBlt(0,0,rc.Width(), rc.Height(), &dcMem,0,0,bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMyQQServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMyQQServerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	if(m_pTCPMediator)
	{
		m_pTCPMediator->close();
		delete(m_pTCPMediator);
		m_pTCPMediator=NULL;
	}
	if(m_pMySql)
	{
		m_pMySql->DisConnect();
		delete(m_pMySql);
		m_pMySql=NULL;
	}
	
}


LRESULT CMyQQServerDlg::OnUserAdd(WPARAM wparam,LPARAM lparam)
{
	STRU_ONLINE*so=(STRU_ONLINE*)wparam;
	CString name=so->m_username;
	m_lstUser.InsertColumn(0,_T("�û�"),LVCFMT_CENTER,60);
	m_lstUser.InsertColumn(1, _T("����"), LVCFMT_CENTER, 100);
	 m_lstUser.InsertItem(0, name);
	 m_lstUser.SetItemText(0,1,"����");
	 name+="������";
	 
	 CString logTempInfo;
	 CTime currTime = CTime::GetCurrentTime();
	 logTempInfo = currTime.Format("%H:%M:%S ");
	 name+=logTempInfo;
     m_lstMsg.AddString(name); 
	//m_lstUser.SetItemText(0,0,_T("��һ"));
	ShowWindow(SW_SHOW);
	return 0;
}

LRESULT CMyQQServerDlg::OnUserOffAdd(WPARAM wparam,LPARAM lparam)
{
	STRU_OFFLINE*so=(STRU_OFFLINE*)wparam;
	CString name=so->m_username;
	//m_lstUser.InsertColumn(0,_T("�û�"),LVCFMT_CENTER,60);
	//m_lstUser.InsertColumn(1, _T("����"), LVCFMT_CENTER, 100);
	// m_lstUser.InsertItem(0, name);
	// m_lstUser.SetItemText(0,1,"����");
	//���û���Ϣ��Ϊ����
	for (int m=0; m<m_lstUser.GetItemCount(); m++)
	{
		if (!_tcscmp(name, m_lstUser.GetItemText(m, 0)))
		{
			//m_userList.DeleteItem(m);
			m_lstUser.SetItemText(m, 1, _T("����"));
			break;
		}
	}
	
	name+="������";
	 
	 CString logTempInfo;
	 CTime currTime = CTime::GetCurrentTime();
	 logTempInfo = currTime.Format("%H:%M:%S ");
	 name+=logTempInfo;
     m_lstMsg.AddString(name); 
	ShowWindow(SW_SHOW);
	return 0;
}


void CMyQQServerDlg::OnBnClickedBtnMin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		ShowWindow(SW_MINIMIZE);
}


void CMyQQServerDlg::OnBnClickedBtnClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		DestroyWindow();
}


void CMyQQServerDlg::OnBnClickedBtnEnd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DestroyWindow();
}


void CMyQQServerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	SendMessage(WM_SYSCOMMAND,0xF012,0);
	CDialogEx::OnLButtonDown(nFlags, point);
}
