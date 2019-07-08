
// MyQQClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyQQClient.h"
#include "MyQQClientDlg.h"
#include "afxdialogex.h"
#include"TCPMediator.h"
#include"resource.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//HHOOK m_hKeyBoard=NULL;
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


// CMyQQClientDlg 对话框



CMyQQClientDlg::CMyQQClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyQQClientDlg::IDD, pParent)
	, m_UserName((""))
	, m_Password((""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	m_pTCPMediator=new CTCPMediator(this);
	m_pMyFriend=new CMyFriend;
	m_pMyChatDlg=new CMyDlgChat;

}

void CMyQQClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_UserName);
	DDX_Text(pDX, IDC_EDIT2,m_Password);
}

BEGIN_MESSAGE_MAP(CMyQQClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CMyQQClientDlg::OnBnClickedButton1)
	ON_MESSAGE(UM_ONLINEMSG,&CMyQQClientDlg::OnLineMsg)
	ON_MESSAGE(UM_DATAINFOMSG,&CMyQQClientDlg::DataInfoMsg)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyQQClientDlg::OnBnClickedButton2)
	ON_MESSAGE(UM_DESTROYMSG,& CMyQQClientDlg::DestroyMsg)
	ON_MESSAGE(UM_FILEINFOMSG,& CMyQQClientDlg::OnFileMsg)
	ON_MESSAGE(UM_FILEINFO,& CMyQQClientDlg::OnFileInfoMsg) //只接收大小
	ON_MESSAGE(UM_PICINFOMSG,& CMyQQClientDlg::OnPicMsg)
	ON_MESSAGE(UM_OFFLINE_ADD,& CMyQQClientDlg::OffLineMsg)
	ON_MESSAGE(UM_INVITEMSG,& CMyQQClientDlg::OnInviteMsg)
	ON_MESSAGE(UM_ONKEEPCONNECTMSG,& CMyQQClientDlg::OnKeepConnectMsg)
	ON_BN_CLICKED(IDC_BTN_ABOUT, &CMyQQClientDlg::OnBnClickedBtnAbout)
	ON_BN_CLICKED(IDC_BTN_MIN, &CMyQQClientDlg::OnBnClickedBtnMin)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CMyQQClientDlg::OnBnClickedBtnClose)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMyQQClientDlg 消息处理程序
LRESULT CMyQQClientDlg::OnLineMsg(WPARAM wparam,LPARAM lparam)
{

	STRU_ONLINERS * so=(STRU_ONLINERS *)wparam;
	CString MyName=so->m_username;
	m_pMyFriend->Create(IDD_DIALOG1);
	m_pMyFriend->ShowWindow(SW_SHOW);
	m_pMyFriend->SetWindowText(MyName);

	 in_addr addr;
	 addr.S_un.S_addr=lparam;
	 CString str;
	 str=inet_ntoa(addr);
	 m_pMyFriend->m_lstMyFriend.AddString(str);
	 m_lstFriend.push_back(str);
	 str="";
	 return 0;
}

LRESULT 	CMyQQClientDlg::OnInviteMsg(WPARAM wparam,LPARAM lparam)
{
	STRU_QUNLIAO_RQ * sq=(STRU_QUNLIAO_RQ * )wparam;
	CString str="群聊";
	//判断群聊是否产生窗口，没有就创建窗口显示 有就显示
	CMyDlgChat * pDlg=m_pMyFriend->m_mapIpToDlg[str];
	if( ! pDlg)
	{
		pDlg=new CMyDlgChat;
		pDlg->Create(IDD_DIALOG2);
		m_pMyFriend->m_mapIpToDlg[str]=pDlg;
	}
    pDlg->ShowWindow(SW_SHOW);
	//把自己的ip加入到群聊好友列表中
	return 0;
}

LRESULT 	CMyQQClientDlg::OnKeepConnectMsg(WPARAM wparam,LPARAM lparam)
{
	//收到保持连接回复 将其从临时好友列表中删除
	STRU_KEEP_CONNECT*sk=(STRU_KEEP_CONNECT*)wparam;
	auto ite=m_strlstKeepConnent.begin();
	while( ite!=m_strlstKeepConnent.end())
	{
		if(sk->m_HisIP==(*ite))
		{
			m_strlstKeepConnent.remove(*ite);
		}

		++ite;
	}

	return 0;
}

LRESULT 	CMyQQClientDlg::OffLineMsg(WPARAM wparam,LPARAM lparam)
{
	STRU_OFFLINE* so=(STRU_OFFLINE* )wparam;
	m_pTCPMediator->SendData(_HOST_IP,(char *)so,sizeof(so));
	return 0;

}

LRESULT CMyQQClientDlg::DataInfoMsg(WPARAM wparam,LPARAM lparam)
{
	STRU_DATAINFO_RS*sdrs=(STRU_DATAINFO_RS*)wparam;
	CString chat=sdrs->m_szBuf;
	CString strIP=sdrs->m_SourceIP;
	if(sdrs->m_nType==_DEF_PROTOCOL_QUNLIAO_INFO_RQ)
	{
		strIP="群聊";
	}
    //判断该好友是否产生窗口，没有就创建窗口显示 有就显示
	CMyDlgChat * pDlg=m_pMyFriend->m_mapIpToDlg[strIP];
	if( ! pDlg)
	{
		pDlg=new CMyDlgChat;
		pDlg->Create(IDD_DIALOG2);
		m_pMyFriend->m_mapIpToDlg[strIP]=pDlg;
	}
    pDlg->ShowWindow(SW_SHOW);
	    CString ww=sdrs->m_SourceIP;
		ww+=" say: ";
		ww+=chat;
		pDlg->m_lstContent.AddString(ww);
		ww="";
		ww.Empty();
		strIP="";
		strIP.Empty();
	UpdateData(FALSE);

	return 0;
}

BOOL CMyQQClientDlg::OnInitDialog()
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
	if(!m_pTCPMediator->open())
	{
		TRACE("%d\n",GetLastError());
		MessageBox(_T("open failed"));
		return FALSE;
	}
//================================================
	this->SetMenu(0);
	HBITMAP   hBitmap; 
	hBitmap = LoadBitmap(AfxGetInstanceHandle(), 
		MAKEINTRESOURCE(IDB_BITMAP2)); // IDB_BITMAP_TEST为资源图片ID

	((CButton *)GetDlgItem(IDC_HEAD))->SetBitmap(hBitmap); 
	//==================================================
	hBitmap = LoadBitmap(AfxGetInstanceHandle(),   //最小化图片
		MAKEINTRESOURCE(IDB_BITMAP10));
	((CButton *)GetDlgItem(IDC_BTN_CLOSE))->SetBitmap(hBitmap);

	hBitmap = LoadBitmap(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDB_BITMAP11));
	((CButton *)GetDlgItem(IDC_BTN_MIN))->SetBitmap(hBitmap);

	hBitmap = LoadBitmap(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDB_BITMAP9));
	((CButton *)GetDlgItem(IDC_BTN_ABOUT))->SetBitmap(hBitmap);
	//=========================================================
	SetTimer( TIMER_CONNECT, DEFAULT_REFRESH_TIME, NULL ); 


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMyQQClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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


void CMyQQClientDlg::LoadHeadPic(UINT headNum)  //加载头像
{
	HBITMAP   hBitmap; 
	switch (headNum)
	{
	case 0:
		hBitmap = LoadBitmap(AfxGetInstanceHandle(), 
			MAKEINTRESOURCE(IDB_BITMAP2));	
		((CButton *)GetDlgItem(IDC_HEAD))->SetBitmap(hBitmap);
		break;

	case 1:
		hBitmap = LoadBitmap(AfxGetInstanceHandle(), 
			MAKEINTRESOURCE(IDB_BITMAP3));	
		((CButton *)GetDlgItem(IDC_HEAD))->SetBitmap(hBitmap);
		break;

	case 2:
		hBitmap = LoadBitmap(AfxGetInstanceHandle(), 
			MAKEINTRESOURCE(IDB_BITMAP4));	
		((CButton *)GetDlgItem(IDC_HEAD))->SetBitmap(hBitmap);
		break;

	case 3:
		hBitmap = LoadBitmap(AfxGetInstanceHandle(), 
			MAKEINTRESOURCE(IDB_BITMAP5));	
		((CButton *)GetDlgItem(IDC_HEAD))->SetBitmap(hBitmap);
		break;

	case 4:
		hBitmap = LoadBitmap(AfxGetInstanceHandle(), 
			MAKEINTRESOURCE(IDB_BITMAP6));	
		((CButton *)GetDlgItem(IDC_HEAD))->SetBitmap(hBitmap);
		break;

	case 5:
		hBitmap = LoadBitmap(AfxGetInstanceHandle(), 
			MAKEINTRESOURCE(IDB_BITMAP7));	
		((CButton *)GetDlgItem(IDC_HEAD))->SetBitmap(hBitmap);
		break;

	case 6:
		hBitmap = LoadBitmap(AfxGetInstanceHandle(), 
			MAKEINTRESOURCE(IDB_BITMAP8));	
		((CButton *)GetDlgItem(IDC_HEAD))->SetBitmap(hBitmap);
		break;
	}
	return ;
}


UINT CMyQQClientDlg::WhichHeadPic(void)
{
	HBITMAP   hBitmap;
	hBitmap =((CButton *)GetDlgItem(IDC_HEAD))->GetBitmap();

	if (hBitmap ==  LoadBitmap(AfxGetInstanceHandle(), 
			MAKEINTRESOURCE(IDB_BITMAP3)))
			return 1;
	else if (hBitmap ==  LoadBitmap(AfxGetInstanceHandle(), 
			MAKEINTRESOURCE(IDB_BITMAP4)))
			return 2;
	else if (hBitmap ==  LoadBitmap(AfxGetInstanceHandle(), 
			MAKEINTRESOURCE(IDB_BITMAP5)))
			return 3;
	else if (hBitmap ==  LoadBitmap(AfxGetInstanceHandle(), 
			MAKEINTRESOURCE(IDB_BITMAP6)))
			return 4;
	else if (hBitmap ==  LoadBitmap(AfxGetInstanceHandle(), 
			MAKEINTRESOURCE(IDB_BITMAP7)))
			return 5;
	else if (hBitmap ==  LoadBitmap(AfxGetInstanceHandle(), 
			MAKEINTRESOURCE(IDB_BITMAP8)))
			return 6;
	else
		return 0;
}


HICON CMyQQClientDlg::GetHICONByHeadNum(UINT headNum)
{
	HICON hIcon=NULL;
	switch (headNum)
	{
	case 0:
		hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
		break;

	case 1:
		hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);
		break;

	case 2:
		hIcon = AfxGetApp()->LoadIcon(IDI_ICON3);
		break;

	case 3:
		hIcon = AfxGetApp()->LoadIcon(IDI_ICON4);
		break;

	case 4:
		hIcon = AfxGetApp()->LoadIcon(IDI_ICON5);
		break;

	case 5:
		hIcon = AfxGetApp()->LoadIcon(IDI_ICON6);
		break;

	case 6:
		hIcon = AfxGetApp()->LoadIcon(IDI_ICON7);
		break;
	}
	return hIcon;
}



// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyQQClientDlg::OnPaint()
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
		//m_pMyChatDlg->OnPaint();
		CPaintDC dc(this);
        CRect rc;
        GetClientRect(&rc);
        CDC dcMem;
        dcMem.CreateCompatibleDC(&dc);
        CBitmap bmpBackground;
        bmpBackground.LoadBitmap(IDB_BACK);

        BITMAP bitmap;
        bmpBackground.GetBitmap(&bitmap);
        CBitmap* pbmpPri = dcMem.SelectObject(&bmpBackground);
        dc.StretchBlt(0,0,rc.Width(), rc.Height(), &dcMem,0,0,bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMyQQClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMyQQClientDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if(m_pTCPMediator)
	{
		m_pTCPMediator->close();
		delete(m_pTCPMediator);
		m_pTCPMediator=NULL;
	}
	if(m_pMyFriend)
	{
		delete(m_pMyFriend);
		m_pMyFriend=NULL;
	}
	if(m_pMyChatDlg)
	{
		delete(m_pMyChatDlg);
		m_pMyChatDlg=NULL;

	}
	//auto ite=m_lstFriend.begin();
	//while(ite!=m_lstFriend.end())
	//{
	//	delete(*ite);
	//	ite=NULL;
	//	++ite;
	//}
}


void CMyQQClientDlg::OnBnClickedButton1()//用户登录
{

	UpdateData();

	int nlen = m_UserName.GetLength();
	if(nlen <0 || nlen >16)
	{
		MessageBox(_T("用户名请输入1-16位有效字符"));
		return;
	}
	 nlen = m_Password.GetLength();
	if(nlen <6 || nlen >16)
	{
		MessageBox(_T("密码请输入6-16位有效字符"));
		return;
	}
	STRU_ONLINE so;
	so.m_nType=_DEF_PROTOCOL_ONLINE_RQ;
	WideCharToMultiByte(CP_ACP,0,m_UserName,-1,so.m_username,sizeof(so.m_username),0,0);
	WideCharToMultiByte(CP_ACP,0,m_Password,-1,so.m_password,sizeof(so.m_password),0,0);
	m_pTCPMediator->SendData(_HOST_IP,(char*)&so,sizeof(so));
}

BOOL CMyQQClientDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==13)
		return FALSE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMyQQClientDlg::OnBnClickedButton2()//用户注册
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	int nlen = m_UserName.GetLength();
	if(nlen <0 || nlen >16)
	{
		MessageBox(_T("用户名请输入1-16位有效字符"));
		return;
	}
	 nlen = m_Password.GetLength();
	if(nlen <6 || nlen >16)
	{
		MessageBox(_T("密码请输入6-16位有效字符"));
		return;
	}
    STRU_ONLINE so;
	so.m_nType=_DEF_PROTOCOL_REGISTER_RQ;
	WideCharToMultiByte(CP_ACP,0,m_UserName,-1,so.m_username,sizeof(so.m_username),0,0);
	WideCharToMultiByte(CP_ACP,0,m_Password,-1,so.m_password,sizeof(so.m_password),0,0);
	m_pTCPMediator->SendData(_HOST_IP,(char*)&so,sizeof(so));
}

LRESULT CMyQQClientDlg::DestroyMsg(WPARAM wparam,LPARAM lparam)
{
	CString strip=*(CString *)wparam;
	 auto ite=m_pMyFriend->m_mapIpToDlg.find(strip);
	 if(ite!=m_pMyFriend->m_mapIpToDlg.end())
	 {
		 delete ite->second;
		 ite->second=NULL;
		 m_pMyFriend->m_mapIpToDlg.erase(ite);
	 }
	return 0;
}

LRESULT CMyQQClientDlg::OnFileMsg(WPARAM wparam ,LPARAM lparam)
{
	STRU_FILE_RS*sfrs=(STRU_FILE_RS*)wparam;
	CString chat=sfrs->m_FileName;
	CString strIP=sfrs->m_SourceIP;

	//选择保存路径
	CString path;
	path=OnBnClickedDecgen();
	path+="\\";
	path+=sfrs->m_FileName;

	char szpath[100]={"C:\\Users\\lenovo\\Desktop\\新建文件夹\\"};
	//strcat_s(path,100,sfrs->m_FileName);
	fstream fs;
	fs.open(path,fstream::binary|fstream::out|fstream::trunc);
	fs.write(sfrs->mFileContent+(sfrs->offset)*sfrs->m_TranSize,sfrs->m_TranSize);
	fs.close();



    //判断该好友是否产生窗口，没有就创建窗口显示 有就显示
	CMyDlgChat * pDlg=m_pMyFriend->m_mapIpToDlg[strIP];
	if( ! pDlg)
	{
		pDlg=new CMyDlgChat;
		pDlg->Create(IDD_DIALOG2);
		m_pMyFriend->m_mapIpToDlg[strIP]=pDlg;
	}
    pDlg->ShowWindow(SW_SHOW);

		CString ww="He say: 已接收文件";
		ww+=chat;
		pDlg->m_lstContent.AddString(ww);
		ww="";
		ww.Empty();
		strIP="";
		strIP.Empty();
		chat="";
		chat.Empty();
		path="";
		path.Empty();
	UpdateData(FALSE);
	return 0;
}

LRESULT CMyQQClientDlg::OnFileInfoMsg(WPARAM wparam,LPARAM lparam)
{

	return 0;
}

CString CMyQQClientDlg::OnBnClickedDecgen()    
{   
		TCHAR szPath[MAX_PATH]={0};     //存放选择的目录路径     
		CString strPath=TEXT("");    
		ZeroMemory(szPath, sizeof(szPath));      
		BROWSEINFO bi;       
		ZeroMemory(&bi,sizeof(bi));

		bi.hwndOwner = m_hWnd;       
		bi.pidlRoot = 0;         
		bi.lpszTitle = _T("请选择需要保存的目录：");      
		bi.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;;      
		bi.lpfn = NULL;       
		bi.lParam = 0;       
		bi.iImage = 0;     

        //弹出选择目录对话框   
		LPITEMIDLIST lp = SHBrowseForFolder(&bi);      
		if(lp )       
		{        
			if(SHGetPathFromIDList(lp,szPath))
			{
				strPath=szPath;
			}
		}    

		if(lp)
		{
			CoTaskMemFree(lp);
		}
	return strPath;
}

LRESULT 	CMyQQClientDlg::OnPicMsg(WPARAM wparam,LPARAM lparam)
{
	STRU_PIC_RQ*sp=(STRU_PIC_RQ*)wparam;
	CString strIP=sp->m_SourceIP;
	CString path=sp->mFilePath;

    //判断该好友是否产生窗口，没有就创建窗口显示 有就显示
	CMyDlgChat * pDlg=m_pMyFriend->m_mapIpToDlg[strIP];
	if( ! pDlg)
	{
		pDlg=new CMyDlgChat;
		pDlg->Create(IDD_DIALOG2);
		m_pMyFriend->m_mapIpToDlg[strIP]=pDlg;
	}
    pDlg->ShowWindow(SW_SHOW);
//=======================================================
    CWnd * pWnd=FindWindow(NULL,strIP);
	HWND hWnd=pWnd->GetSafeHwnd();
	HWND HH=::GetDlgItem(hWnd,IDC_LIST1);
	CWnd * pWnd3 = FromHandle(HH);
	 CDC * pDC=pWnd3->GetDC();

		/*CString ww="He say: ";
		pDlg->m_lstContent.AddString(ww);*/
//根据路径查找该路径下指定图片

	CFileStatus fstatus;    
	CFile file;    
	ULONGLONG cb;    
		  
	 //打开文件并检测文件的有效性  
    if (!file.Open(path,CFile::modeRead))  
	 {  
		 return -1;  
	 }  
	 if (!file.GetStatus(path,fstatus))  
	 {  
		 return -1;  
	 }  
	 if ((cb =fstatus.m_size)<=0)  
	 {  
		 return -1;  
	 }  	  
	 // 根据文件大小分配内存空间,记得释放内存  
	 HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, (unsigned int)cb);    
	  if (hGlobal== NULL)   
	  {  
		  return -1;  
	  }  
	  // 锁定刚才分配的内存空间  
	  LPVOID pvData = NULL;    
	  pvData = GlobalLock(hGlobal);  
	  if (pvData == NULL)    
	 {    
		 GlobalFree(hGlobal);  // 记得释放内存  
		 return -1;  
	  }   
		  
	 // 将文件放到流中  
	  IStream *pStm;    
	  file.Read(pvData,(unsigned int)cb);    
	  GlobalUnlock(hGlobal);    
	  CreateStreamOnHGlobal(hGlobal, TRUE, &pStm); 

	  IPicture *pPic;   
	  if(OleLoadPicture(pStm,(LONG)fstatus.m_size,TRUE,IID_IPicture,(LPVOID*)&pPic)!=S_OK)   
	  {   
		  GlobalFree(hGlobal);  // 记得释放内存  
		  return -1;  
	  }
	  OLE_XSIZE_HIMETRIC hmWidth;    
	  OLE_YSIZE_HIMETRIC hmHeight;    
	  pPic->get_Width(&hmWidth);    
	  pPic->get_Height(&hmHeight);
	  RECT rtWnd;  
	  pDC->GetWindow()->GetWindowRect(&rtWnd);  
	  int iWndWidth=rtWnd.right-rtWnd.left;  
	  int iWndHeight=rtWnd.bottom-rtWnd.top; 
	  if(FAILED(pPic->Render(*pDC,0,0,hmHeight/10,hmWidth/10,0,hmHeight,hmWidth,-hmHeight,NULL)))    
	  {  
		  pPic->Release();  
		  GlobalFree(hGlobal);  // 记得释放内存  
		  return -1;  
	  }  
	  ReleaseDC(pDC);
	  pPic->Release();   
	  GlobalFree(hGlobal);  // 记得释放内存  
	  		
//==================================================================================
	/*	ww="";
		ww.Empty();*/
		strIP="";
		strIP.Empty();

		path="";
		path.Empty();
	UpdateData(FALSE);
	return 0;
}

void CMyQQClientDlg::OnBnClickedBtnAbout()
{
	// TODO: 在此添加控件通知处理程序代码
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


void CMyQQClientDlg::OnBnClickedBtnMin()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowWindow(SW_MINIMIZE);
}


void CMyQQClientDlg::OnBnClickedBtnClose()
{
	// TODO: 在此添加控件通知处理程序代码
	DestroyWindow();
}


void CMyQQClientDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SendMessage(WM_SYSCOMMAND,0xF012,0);
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMyQQClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//检查临时好友列表 将他们从我的好友中删除
	auto ite2=m_strlstKeepConnent.begin();
	while( ite2!=m_strlstKeepConnent.end())
	{
		m_lstFriend.remove(*ite2);
		int nindex=m_pMyFriend->m_lstMyFriend.FindStringExact(0,*ite2);
		m_pMyFriend->m_lstMyFriend.DeleteString(nindex);
		++ite2;
	}
	m_strlstKeepConnent.clear();

	/// 分别发送保持连接的消息，将发送过的IP加入到m_strlstKeepConnent
	auto ite=m_lstFriend.begin();
	while( ite!=m_lstFriend.end())
	{
		CString user=*ite; //我的好友的ip
		STRU_KEEP_CONNECT sk;
		sk.m_nType=_DEF_PROTOCOL_KEEP_CONNECT_RQ;
		strcpy_s((char*)&sk.m_MyIP,(size_t)sizeof(sk.m_MyIP),(char*)_MY_IP_STR);
		strcpy_s((char*)&sk.m_HisIP,(size_t)sizeof(sk.m_HisIP),(char*)&(*ite));
		m_pTCPMediator->SendData(_HOST_IP,(char *)&sk,sizeof(sk));
		//发送请求后 将其加入我的临时好友列表
		m_strlstKeepConnent.push_back(sk.m_HisIP);

		++ite;
	}
	CDialogEx::OnTimer(nIDEvent);
}
