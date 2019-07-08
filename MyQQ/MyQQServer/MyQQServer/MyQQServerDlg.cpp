// MyQQServerDlg.cpp : 实现文件
#include "stdafx.h"
#include "MyQQServer.h"
#include "MyQQServerDlg.h"
#include "afxdialogex.h"
#include"TCPMediator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMyQQServerDlg 对话框

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


// CMyQQServerDlg 消息处理程序


BOOL CMyQQServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMyQQServerDlg::OnBnClickedBtnBegin()
{
	// TODO: 在此添加控件通知处理程序代码
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyQQServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
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

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMyQQServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMyQQServerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
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
	m_lstUser.InsertColumn(0,_T("用户"),LVCFMT_CENTER,60);
	m_lstUser.InsertColumn(1, _T("报告"), LVCFMT_CENTER, 100);
	 m_lstUser.InsertItem(0, name);
	 m_lstUser.SetItemText(0,1,"上线");
	 name+="上线啦";
	 
	 CString logTempInfo;
	 CTime currTime = CTime::GetCurrentTime();
	 logTempInfo = currTime.Format("%H:%M:%S ");
	 name+=logTempInfo;
     m_lstMsg.AddString(name); 
	//m_lstUser.SetItemText(0,0,_T("卡一"));
	ShowWindow(SW_SHOW);
	return 0;
}

LRESULT CMyQQServerDlg::OnUserOffAdd(WPARAM wparam,LPARAM lparam)
{
	STRU_OFFLINE*so=(STRU_OFFLINE*)wparam;
	CString name=so->m_username;
	//m_lstUser.InsertColumn(0,_T("用户"),LVCFMT_CENTER,60);
	//m_lstUser.InsertColumn(1, _T("报告"), LVCFMT_CENTER, 100);
	// m_lstUser.InsertItem(0, name);
	// m_lstUser.SetItemText(0,1,"上线");
	//将用户信息改为离线
	for (int m=0; m<m_lstUser.GetItemCount(); m++)
	{
		if (!_tcscmp(name, m_lstUser.GetItemText(m, 0)))
		{
			//m_userList.DeleteItem(m);
			m_lstUser.SetItemText(m, 1, _T("离线"));
			break;
		}
	}
	
	name+="下线啦";
	 
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
	// TODO: 在此添加控件通知处理程序代码
		ShowWindow(SW_MINIMIZE);
}


void CMyQQServerDlg::OnBnClickedBtnClose()
{
	// TODO: 在此添加控件通知处理程序代码
		DestroyWindow();
}


void CMyQQServerDlg::OnBnClickedBtnEnd()
{
	// TODO: 在此添加控件通知处理程序代码
	DestroyWindow();
}


void CMyQQServerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SendMessage(WM_SYSCOMMAND,0xF012,0);
	CDialogEx::OnLButtonDown(nFlags, point);
}
