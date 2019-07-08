// MyFriend.cpp : 实现文件
//

#include "stdafx.h"
#include "MyQQClient.h"
#include "MyFriend.h"
#include "afxdialogex.h"
#include"resource.h"
#include"CgHeadDlg.h"
#include"MyQQClientDlg.h"

// CMyFriend 对话框

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
	//DWORD dwStyle = m_lstMyFriend.GetExtendedStyle(); //显示网格线
	//dwStyle |= LVS_EX_FULLROWSELECT;            
 //   dwStyle |= LVS_EX_GRIDLINES;     
	//m_friendList.SetExtendedStyle(dwStyle);

	//

	////加入头像
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

	//m_friendList.InsertColumn(0, _T("在线用户"), LVCFMT_CENTER, 120);

	////增加一项，群的显示
	//m_friendList.InsertItem(0, _T("QQ群"), 0); //第三个参数为图标的索引
}

BOOL CMyFriend::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//设置标题栏为自己的用户名，以标识哪个是自己，自己是哪个对话框
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
	windowText += _T(" 在线");
	GetDlgItem(IDC_ONLINEMSG)->SetWindowText(windowText);

	hBitmap = LoadBitmap(AfxGetInstanceHandle(), 
		MAKEINTRESOURCE(IDB_BITMAP2)); // IDB_BITMAP_TEST为资源图片ID

	((CButton *)GetDlgItem(IDC_BTN_HEAD))->SetBitmap(hBitmap); 

		hBitmap = LoadBitmap(AfxGetInstanceHandle(),   //最小化图片
		MAKEINTRESOURCE(IDB_BITMAP10));
	((CButton *)GetDlgItem(IDC_BTN_CLOSE))->SetBitmap(hBitmap);

	hBitmap = LoadBitmap(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDB_BITMAP11));
	((CButton *)GetDlgItem(IDC_BTN_MIN))->SetBitmap(hBitmap);

	


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMyFriend::OnLbnDblclkList1()
{
	// TODO: 在此添加控件通知处理程序代码
	//获取选中项
	int nIndex=m_lstMyFriend.GetCurSel();
	if(nIndex==-1)return;
	//获取选中项IP
	CString strIP;
	m_lstMyFriend.GetText( nIndex,strIP);
	GetDlg(strIP);
}

CMyDlgChat* CMyFriend::GetDlg(CString strIP)
{
    //判断该好友是否产生窗口，没有-》创建窗口显示 有-》显示
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
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==13)
		return FALSE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMyFriend::OnBnClickedHead()
{
	// TODO: 在此添加控件通知处理程序代码
	CCgHeadDlg headDlg;
	 headDlg.DoModal();
}

SOCKET CMyFriend::GetSockCli(void)
{
	return m_sockCli;
}


void CMyFriend::OnBnClickedBtnMin()
{
	// TODO: 在此添加控件通知处理程序代码
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


void CMyFriend::OnBnClickedBtnClose()//下线
{
	// TODO: 在此添加控件通知处理程序代码
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
		// TODO: 在此处添加消息处理程序代码
		// 不为绘图消息调用 CDialogEx::OnPaint()
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SendMessage(WM_SYSCOMMAND,0xF012,0);
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMyFriend::OnBnClickedButton1()   //群聊
{
	// TODO: 在此添加控件通知处理程序代码
	CMyDlgChat * pDlg=new CMyDlgChat;
	pDlg->Create(IDD_DIALOG2);
	CString strIP="群聊";
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