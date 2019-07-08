// MyFriend.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyQQClient.h"
#include "MyFriend.h"
#include "afxdialogex.h"
#include"resource.h"
#include"CgHeadDlg.h"
#include"MyQQClientDlg.h"

// CMyFriend �Ի���

IMPLEMENT_DYNAMIC(CMyFriend, CDialogEx)

CMyFriend::CMyFriend(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyFriend::IDD, pParent)
{
	m_pWnd=pParent;
}

CMyFriend::~CMyFriend()
{
}

void CMyFriend::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST1, m_lstMyFriend);
}

BEGIN_MESSAGE_MAP(CMyFriend, CDialogEx)
	ON_LBN_DBLCLK(IDC_LIST1, &CMyFriend::OnLbnDblclkList1)
	ON_BN_CLICKED(IDC_HEAD, &CMyFriend::OnBnClickedHead)
	ON_BN_CLICKED(IDC_BTN_MIN, &CMyFriend::OnBnClickedBtnMin)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CMyFriend::OnBnClickedBtnClose)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON1, &CMyFriend::OnBnClickedButton1)
	ON_MESSAGE(UM_OFFLINEMSG,&CMyFriend::OnOffLineMsg)
END_MESSAGE_MAP()

void CMyFriend::InitListCtr(void)
{
	//DWORD dwStyle = m_lstMyFriend.GetExtendedStyle(); //��ʾ������
	//dwStyle |= LVS_EX_FULLROWSELECT;            
 //   dwStyle |= LVS_EX_GRIDLINES;     
	//m_friendList.SetExtendedStyle(dwStyle);

	//

	////����ͷ��
	//m_imageList.Create(40, 40, ILC_MASK | ILC_COLOR32, 1, 1);

	//m_imageList.Add( AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	//m_imageList.Add(AfxGetApp()->LoadIcon(IDI_HEAD1));
	//m_imageList.Add(AfxGetApp()->LoadIcon(IDI_HEAD2));
	//m_imageList.Add(AfxGetApp()->LoadIcon(IDI_HEAD3));
	//m_imageList.Add(AfxGetApp()->LoadIcon(IDI_HEAD4));
	//m_imageList.Add(AfxGetApp()->LoadIcon(IDI_HEAD5));
	//m_imageList.Add(AfxGetApp()->LoadIcon(IDI_HEAD6));

	////m_friendList.SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), FALSE);
	//m_friendList.SetImageList(&m_imageList, LVSIL_SMALL);

	//m_friendList.InsertColumn(0, _T("�����û�"), LVCFMT_CENTER, 120);

	////����һ�Ⱥ����ʾ
	//m_friendList.InsertItem(0, _T("QQȺ"), 0); //����������Ϊͼ�������
}

BOOL CMyFriend::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//���ñ�����Ϊ�Լ����û������Ա�ʶ�ĸ����Լ����Լ����ĸ��Ի���
	CString userNameTitle;
	//((COpenQQDlg *)AfxGetMainWnd())->GetDlgItemText(IDC_USERNAME, userNameTitle);
	//SetWindowText(userNameTitle);


	HBITMAP hBitmap;
	hBitmap = LoadBitmap(AfxGetInstanceHandle(), 
		MAKEINTRESOURCE(IDB_BK2));
	((CListCtrl *)GetDlgItem(IDC_LIST1))->SetBkImage(hBitmap);

	((CButton *)GetDlgItem(IDC_BTN_HEAD))->SetIcon(this->m_tHIcon);

	CString windowText;
	GetWindowText(windowText);
	windowText += _T(" ����");
	GetDlgItem(IDC_ONLINEMSG)->SetWindowText(windowText);

	hBitmap = LoadBitmap(AfxGetInstanceHandle(), 
		MAKEINTRESOURCE(IDB_BITMAP2)); // IDB_BITMAP_TESTΪ��ԴͼƬID

	((CButton *)GetDlgItem(IDC_BTN_HEAD))->SetBitmap(hBitmap); 

		hBitmap = LoadBitmap(AfxGetInstanceHandle(),   //��С��ͼƬ
		MAKEINTRESOURCE(IDB_BITMAP10));
	((CButton *)GetDlgItem(IDC_BTN_CLOSE))->SetBitmap(hBitmap);

	hBitmap = LoadBitmap(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDB_BITMAP11));
	((CButton *)GetDlgItem(IDC_BTN_MIN))->SetBitmap(hBitmap);

	


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CMyFriend::OnLbnDblclkList1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ȡѡ����
	int nIndex=m_lstMyFriend.GetCurSel();
	if(nIndex==-1)return;
	//��ȡѡ����IP
	CString strIP;
	m_lstMyFriend.GetText( nIndex,strIP);
	GetDlg(strIP);
}

CMyDlgChat* CMyFriend::GetDlg(CString strIP)
{
    //�жϸú����Ƿ�������ڣ�û��-������������ʾ ��-����ʾ
	CMyDlgChat * pDlg=m_mapIpToDlg[strIP];
	if( ! pDlg)
	{
		pDlg=new CMyDlgChat;
		pDlg->Create(IDD_DIALOG2);
		m_mapIpToDlg[strIP]=pDlg;
	}
	pDlg->GetDlgItem(IDC_BUTTON6)->ShowWindow(SW_HIDE);
	pDlg->GetDlgItem(IDC_SEND)->ShowWindow(SW_HIDE);
	pDlg->GetDlgItem(IDC_PROGRESS1)->ShowWindow(SW_HIDE);
    pDlg->ShowWindow(SW_SHOW);
	pDlg->SetWindowText(strIP);
	return pDlg;
}


BOOL CMyFriend::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==13)
		return FALSE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMyFriend::OnBnClickedHead()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCgHeadDlg headDlg;
	 headDlg.DoModal();
}

SOCKET CMyFriend::GetSockCli(void)
{
	return m_sockCli;
}


void CMyFriend::OnBnClickedBtnMin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowWindow(SW_HIDE);
	NotifyIcon.cbSize=sizeof(NOTIFYICONDATA);
	NotifyIcon.hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	NotifyIcon.hWnd=m_hWnd;
	CString onLineMsg;
	GetDlgItemText(IDC_ONLINEMSG, onLineMsg);
	lstrcpy(NotifyIcon.szTip, onLineMsg);
	NotifyIcon.uCallbackMessage=WM_NC;
	NotifyIcon.uFlags=NIF_ICON | NIF_MESSAGE | NIF_TIP;
	Shell_NotifyIcon(NIM_ADD,&NotifyIcon);
}


void CMyFriend::OnBnClickedBtnClose()//����
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	STRU_OFFLINE so;
	so.m_nType=_DEF_PROTOCOL_OFFLINE_RQ;
	strcpy_s((char*)&so.m_MyIP,(size_t)sizeof(so.m_MyIP),(char*)_MY_IP_STR);
	CString name;
	GetWindowText(name);
	strcpy_s((char*)&so.m_username,(size_t)sizeof(so.m_username),(char*)&name);

	((CMyQQClientDlg*)GetParent())->GetMediator()->SendData(_HOST_IP,((char *)&so),sizeof(so));
	EndDialog(0);
}


void CMyFriend::OnPaint()
{
	CPaintDC dc(this); // device context for painting
		// TODO: �ڴ˴������Ϣ����������
		// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	CRect rc;
	GetClientRect(&rc);
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap bmpBackground;
	bmpBackground.LoadBitmap(IDB_BK2);

	BITMAP bitmap;
	bmpBackground.GetBitmap(&bitmap);
	CBitmap* pbmpPri = dcMem.SelectObject(&bmpBackground);
	dc.StretchBlt(0,0,rc.Width(), rc.Height(), &dcMem,0,0,bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
}


void CMyFriend::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	SendMessage(WM_SYSCOMMAND,0xF012,0);
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMyFriend::OnBnClickedButton1()   //Ⱥ��
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyDlgChat * pDlg=new CMyDlgChat;
	pDlg->Create(IDD_DIALOG2);
	CString strIP="Ⱥ��";
	m_mapIpToDlg[strIP]=pDlg;
	pDlg->GetDlgItem(IDC_BUTTON3)->ShowWindow(SW_HIDE);
	pDlg->GetDlgItem(IDC_BUTTON4)->ShowWindow(SW_HIDE);
	pDlg->GetDlgItem(IDC_BUTTON5)->ShowWindow(SW_HIDE);
    pDlg->ShowWindow(SW_SHOW);
	pDlg->SetWindowText(strIP);
}

LRESULT CMyFriend::OnOffLineMsg(WPARAM wparam,LPARAM lparam)
{
	STRU_OFFLINE* so=(STRU_OFFLINE*)wparam;
	CString strIp;
	for( int i=0;i<m_lstMyFriend.GetCount();i++)
	 {
		 m_lstMyFriend.GetText(i,strIp);
		 if(so->m_MyIP==strIp )
		 {
			 m_lstMyFriend.DeleteString(i);
			 break;
		 }
	 }
	return 0;
}