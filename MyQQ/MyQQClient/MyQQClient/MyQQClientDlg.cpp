
// MyQQClientDlg.cpp : ʵ���ļ�
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


// CMyQQClientDlg �Ի���



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
	ON_MESSAGE(UM_FILEINFO,& CMyQQClientDlg::OnFileInfoMsg) //ֻ���մ�С
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


// CMyQQClientDlg ��Ϣ�������
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
	CString str="Ⱥ��";
	//�ж�Ⱥ���Ƿ�������ڣ�û�оʹ���������ʾ �о���ʾ
	CMyDlgChat * pDlg=m_pMyFriend->m_mapIpToDlg[str];
	if( ! pDlg)
	{
		pDlg=new CMyDlgChat;
		pDlg->Create(IDD_DIALOG2);
		m_pMyFriend->m_mapIpToDlg[str]=pDlg;
	}
    pDlg->ShowWindow(SW_SHOW);
	//���Լ���ip���뵽Ⱥ�ĺ����б���
	return 0;
}

LRESULT 	CMyQQClientDlg::OnKeepConnectMsg(WPARAM wparam,LPARAM lparam)
{
	//�յ��������ӻظ� �������ʱ�����б���ɾ��
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
		strIP="Ⱥ��";
	}
    //�жϸú����Ƿ�������ڣ�û�оʹ���������ʾ �о���ʾ
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
		MAKEINTRESOURCE(IDB_BITMAP2)); // IDB_BITMAP_TESTΪ��ԴͼƬID

	((CButton *)GetDlgItem(IDC_HEAD))->SetBitmap(hBitmap); 
	//==================================================
	hBitmap = LoadBitmap(AfxGetInstanceHandle(),   //��С��ͼƬ
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


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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


void CMyQQClientDlg::LoadHeadPic(UINT headNum)  //����ͷ��
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



// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMyQQClientDlg::OnPaint()
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMyQQClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMyQQClientDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
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


void CMyQQClientDlg::OnBnClickedButton1()//�û���¼
{

	UpdateData();

	int nlen = m_UserName.GetLength();
	if(nlen <0 || nlen >16)
	{
		MessageBox(_T("�û���������1-16λ��Ч�ַ�"));
		return;
	}
	 nlen = m_Password.GetLength();
	if(nlen <6 || nlen >16)
	{
		MessageBox(_T("����������6-16λ��Ч�ַ�"));
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
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==13)
		return FALSE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMyQQClientDlg::OnBnClickedButton2()//�û�ע��
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	int nlen = m_UserName.GetLength();
	if(nlen <0 || nlen >16)
	{
		MessageBox(_T("�û���������1-16λ��Ч�ַ�"));
		return;
	}
	 nlen = m_Password.GetLength();
	if(nlen <6 || nlen >16)
	{
		MessageBox(_T("����������6-16λ��Ч�ַ�"));
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

	//ѡ�񱣴�·��
	CString path;
	path=OnBnClickedDecgen();
	path+="\\";
	path+=sfrs->m_FileName;

	char szpath[100]={"C:\\Users\\lenovo\\Desktop\\�½��ļ���\\"};
	//strcat_s(path,100,sfrs->m_FileName);
	fstream fs;
	fs.open(path,fstream::binary|fstream::out|fstream::trunc);
	fs.write(sfrs->mFileContent+(sfrs->offset)*sfrs->m_TranSize,sfrs->m_TranSize);
	fs.close();



    //�жϸú����Ƿ�������ڣ�û�оʹ���������ʾ �о���ʾ
	CMyDlgChat * pDlg=m_pMyFriend->m_mapIpToDlg[strIP];
	if( ! pDlg)
	{
		pDlg=new CMyDlgChat;
		pDlg->Create(IDD_DIALOG2);
		m_pMyFriend->m_mapIpToDlg[strIP]=pDlg;
	}
    pDlg->ShowWindow(SW_SHOW);

		CString ww="He say: �ѽ����ļ�";
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
		TCHAR szPath[MAX_PATH]={0};     //���ѡ���Ŀ¼·��     
		CString strPath=TEXT("");    
		ZeroMemory(szPath, sizeof(szPath));      
		BROWSEINFO bi;       
		ZeroMemory(&bi,sizeof(bi));

		bi.hwndOwner = m_hWnd;       
		bi.pidlRoot = 0;         
		bi.lpszTitle = _T("��ѡ����Ҫ�����Ŀ¼��");      
		bi.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;;      
		bi.lpfn = NULL;       
		bi.lParam = 0;       
		bi.iImage = 0;     

        //����ѡ��Ŀ¼�Ի���   
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

    //�жϸú����Ƿ�������ڣ�û�оʹ���������ʾ �о���ʾ
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
//����·�����Ҹ�·����ָ��ͼƬ

	CFileStatus fstatus;    
	CFile file;    
	ULONGLONG cb;    
		  
	 //���ļ�������ļ�����Ч��  
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
	 // �����ļ���С�����ڴ�ռ�,�ǵ��ͷ��ڴ�  
	 HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, (unsigned int)cb);    
	  if (hGlobal== NULL)   
	  {  
		  return -1;  
	  }  
	  // �����ղŷ�����ڴ�ռ�  
	  LPVOID pvData = NULL;    
	  pvData = GlobalLock(hGlobal);  
	  if (pvData == NULL)    
	 {    
		 GlobalFree(hGlobal);  // �ǵ��ͷ��ڴ�  
		 return -1;  
	  }   
		  
	 // ���ļ��ŵ�����  
	  IStream *pStm;    
	  file.Read(pvData,(unsigned int)cb);    
	  GlobalUnlock(hGlobal);    
	  CreateStreamOnHGlobal(hGlobal, TRUE, &pStm); 

	  IPicture *pPic;   
	  if(OleLoadPicture(pStm,(LONG)fstatus.m_size,TRUE,IID_IPicture,(LPVOID*)&pPic)!=S_OK)   
	  {   
		  GlobalFree(hGlobal);  // �ǵ��ͷ��ڴ�  
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
		  GlobalFree(hGlobal);  // �ǵ��ͷ��ڴ�  
		  return -1;  
	  }  
	  ReleaseDC(pDC);
	  pPic->Release();   
	  GlobalFree(hGlobal);  // �ǵ��ͷ��ڴ�  
	  		
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


void CMyQQClientDlg::OnBnClickedBtnMin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowWindow(SW_MINIMIZE);
}


void CMyQQClientDlg::OnBnClickedBtnClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DestroyWindow();
}


void CMyQQClientDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	SendMessage(WM_SYSCOMMAND,0xF012,0);
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMyQQClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//�����ʱ�����б� �����Ǵ��ҵĺ�����ɾ��
	auto ite2=m_strlstKeepConnent.begin();
	while( ite2!=m_strlstKeepConnent.end())
	{
		m_lstFriend.remove(*ite2);
		int nindex=m_pMyFriend->m_lstMyFriend.FindStringExact(0,*ite2);
		m_pMyFriend->m_lstMyFriend.DeleteString(nindex);
		++ite2;
	}
	m_strlstKeepConnent.clear();

	/// �ֱ��ͱ������ӵ���Ϣ�������͹���IP���뵽m_strlstKeepConnent
	auto ite=m_lstFriend.begin();
	while( ite!=m_lstFriend.end())
	{
		CString user=*ite; //�ҵĺ��ѵ�ip
		STRU_KEEP_CONNECT sk;
		sk.m_nType=_DEF_PROTOCOL_KEEP_CONNECT_RQ;
		strcpy_s((char*)&sk.m_MyIP,(size_t)sizeof(sk.m_MyIP),(char*)_MY_IP_STR);
		strcpy_s((char*)&sk.m_HisIP,(size_t)sizeof(sk.m_HisIP),(char*)&(*ite));
		m_pTCPMediator->SendData(_HOST_IP,(char *)&sk,sizeof(sk));
		//��������� ��������ҵ���ʱ�����б�
		m_strlstKeepConnent.push_back(sk.m_HisIP);

		++ite;
	}
	CDialogEx::OnTimer(nIDEvent);
}
