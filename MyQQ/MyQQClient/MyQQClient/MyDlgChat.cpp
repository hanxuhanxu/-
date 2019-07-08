// MyDlgChat.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyQQClient.h"
#include "MyDlgChat.h"
#include "afxdialogex.h"
#include"MyQQClientDlg.h"
#include"resource.h"
#include"afxole.h" 
#include"richole.h"
#include<fstream>
#include"VideoDlg.h"
#include"ChatFriendDlg.h"
#include"resource.h"

int ContentType=1;
CString sname;
HBITMAP hbitmap=NULL;
CString path;

// CMyDlgChat �Ի���

IMPLEMENT_DYNAMIC(CMyDlgChat, CDialogEx)

CMyDlgChat::CMyDlgChat(CWnd* pParent )
	: CDialogEx(CMyDlgChat::IDD, pParent)
	, m_edtContent(_T(""))
{
}

CMyDlgChat::~CMyDlgChat()
{
}

void CMyDlgChat::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_edtContent);
	DDX_Control(pDX, IDC_LIST1, m_lstContent);
	//DDX_Control(pDX, IDC_RICHEDIT21, m_reRecv);
	//DDX_Control(pDX, IDC_RICHEDIT22, m_reSend);
	DDX_Control(pDX, IDC_PROGRESS1, m_tranProgress);
}


BEGIN_MESSAGE_MAP(CMyDlgChat, CDialogEx)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CMyDlgChat::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyDlgChat::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMyDlgChat::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMyDlgChat::OnBnClickedButton4)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON5, &CMyDlgChat::OnBnClickedButton5)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON6, &CMyDlgChat::OnBnClickedButton6)
END_MESSAGE_MAP()


// CMyDlgChat ��Ϣ�������


void CMyDlgChat::OnDestroy()
{
	CDialogEx::OnDestroy();
	//���Լ���map���Ƴ� ����Ϣ
	CString ip;
	GetWindowText(ip);
	GetParent()->SendMessage(UM_DESTROYMSG,(WPARAM)&ip,0);
	// TODO: �ڴ˴������Ϣ����������
}


void CMyDlgChat::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	UpdateData();
	CString strIP;
	GetWindowText(strIP);
	if(strIP==_T("Ⱥ��"))
	{
		ContentType=4;
	}

	if(ContentType==1)//����ͨ��Ϣ
	{
		//HKEY hKey;
		//if( RegOpenKeyEx( HKEY_CLASSES_ROOT, _T("RICHTEXT.RichtextCtrl\\CLSID"), 0, KEY_READ, &hKey ) != ERROR_SUCCESS ) 
		//{
		//	HINSTANCE hLib = LoadLibrary(  _T("RICHTX32.OCX"));
		//	/// �ؼ�������
		//	if( !hLib )
		//	{
		//		MessageBox(  _T("RICHTX32.OCX�ؼ�δ�ҵ�" ));
		//	}
		//	else
		//	{
		//		//��ȡע�ắ��DllRegisterServer��ַ
		//		FARPROC lpDllEntryPoint;
		//		lpDllEntryPoint = GetProcAddress( hLib, "DllRegisterServer" );
		//		/// ע��richtx32.ocx�ؼ�
		//		lpDllEntryPoint();
		//	}
		//}
			STRU_DATAINFO sd;
			sd.m_nType=_DEF_PROTOCOL_DATAINFO_RQ;
			sd.m_nNum=m_edtContent.GetLength()+1;	
			WideCharToMultiByte(CP_ACP,0,m_edtContent,-1,sd.m_szBuf,sizeof(sd.m_szBuf),0,0);//��������
			WideCharToMultiByte(CP_ACP,0,strIP,-1,sd.m_ToIP,sizeof(sd.m_ToIP),0,0);//�Է�ip

			if(((CMyQQClientDlg*)GetParent())->GetMediator()->SendData(_HOST_IP,((char *)&sd),sizeof(sd)))
			{
					 this->ShowWindow(SW_SHOW);
					  CString ww="I say: ";
					  ww+=m_edtContent;
					   m_lstContent.AddString(ww);
             
					  m_edtContent="";
					  int m= m_lstContent.GetCurSel();
					  if(m_lstContent.GetCount()>10)
					  {				  
						 m_lstContent.SetCurSel(m_lstContent.GetCount()-1);
					  }			 
					  UpdateData(FALSE);
					  ww="";
					  ww.Empty();
			}
	}
	else if(ContentType==2)//���ļ�
	{
		GetDlgItem(IDC_SEND)->ShowWindow(TRUE);
		GetDlgItem(IDC_PROGRESS1)->ShowWindow(TRUE);
		STRU_FILE sf;
		sf.m_nType=_DEF_PROTOCOL_FILESTREAMINFO_RQ;
		WideCharToMultiByte(CP_ACP,0,m_edtContent,-1,sf.mFileStream,sizeof(sf.mFileStream),0,0);//����������ݸ�ֵ��·��
		m_edtContent="";
		UpdateData(FALSE);
		WideCharToMultiByte(CP_ACP,0,sname,-1,sf.m_FileName,sizeof(sf.m_FileName),0,0);//�ļ���
		WideCharToMultiByte(CP_ACP,0,strIP,-1,sf.m_ToIP,sizeof(sf.m_ToIP),0,0);//�Է�ip

		FILE *pFile=NULL;
		fopen_s(&pFile,sf.mFileStream,"rb");
		if(pFile == NULL)
			return;
		fseek(pFile,0,SEEK_END);
		int nFileSize = ftell(pFile);
		sf.m_FileSize=nFileSize;
		sf.m_TranSize=1024;
		sf.offset=0;
		fseek(pFile,0,SEEK_SET);

		while(1)
		{
			sf.m_nType=_DEF_PROTOCOL_FILESTREAM_RQ;
			sf.m_TranSize=fread_s(sf.mFileContent/*Ŀ�껺����*/,sizeof(sf.mFileContent)/*��������С*/,sizeof(char),sizeof(sf.mFileContent),pFile);
			if(sf.m_TranSize==0)
			{
				break;
			}
			sf.offset++;
			((CMyQQClientDlg*)GetParent())->GetMediator()->SendDataTCP(_HOST_IP,((char *)&sf),sizeof(sf));
		}
		fclose(pFile);

		//fstream fs;//�ļ�
		//fs.open(sf.mFileStream,fstream::in|fstream::binary);//���ļ�
		//fs.seekg(0,fs.end);
		//sf.m_FileSize=fs.tellg(); //�ļ���С
		//fs.seekg(0,fs.beg);
		//fs.read(sf.mFileContent,sf.m_FileSize);//���� sf.mFileContent ��
		//m_tranProgress.SetPos(100);
		//((CMyQQClientDlg*)GetParent())->GetMediator()->SendDataTCP(_HOST_IP,((char *)&sf),sizeof(sf));
		//fs.close();
		ContentType=1;
	}
	else if(ContentType==3) //��ͼƬ
	{
		STRU_PIC_RQ sp;
		sp.m_nType=_DEF_PROTOCOL_PICTURE_RQ;
		WideCharToMultiByte(CP_ACP,0,strIP,-1,sp.m_SourceIP,sizeof(sp.m_SourceIP),0,0);//�Է�ip
		//WideCharToMultiByte(CP_ACP,0,m_edtContent,-1,sp.mFileContent,sizeof(sp.mFileContent),0,0); //ͼƬ
		WideCharToMultiByte(CP_ACP,0,path,-1,sp.mFilePath,sizeof(sp.mFilePath),0,0); //��ַ
		fstream fs;//�ļ�
		fs.open(sp.mFilePath,fstream::in|fstream::binary);//���ļ�
		fs.seekg(0,fs.end);
		sp.m_FileSize=fs.tellg(); //�ļ���С
		fs.seekg(0,fs.beg);
		fs.read(sp.mFileContent,sp.m_FileSize);//���� sf.mFileContent ��

		((CMyQQClientDlg*)GetParent())->GetMediator()->SendDataTCP(_HOST_IP,((char *)&sp),sizeof(sp));
		ContentType=1;
	}
	else if(ContentType==4) //Ⱥ��
	{
		    STRU_DATAINFO sd;
			sd.m_nType=_DEF_PROTOCOL_QUNLIAO_INFO_RQ;
			sd.m_nNum=m_edtContent.GetLength()+1;	
			WideCharToMultiByte(CP_ACP,0,m_edtContent,-1,sd.m_szBuf,sizeof(sd.m_szBuf),0,0);//��������
			//WideCharToMultiByte(CP_ACP,0,strIP,-1,sd.m_ToIP,sizeof(sd.m_ToIP),0,0);//�Է�ip

			if(((CMyQQClientDlg*)GetParent())->GetMediator()->SendData(_HOST_IP,((char *)&sd),sizeof(sd)))
			{
					 this->ShowWindow(SW_SHOW);
					  CString ww="I say: ";
					  ww+=m_edtContent;
					   m_lstContent.AddString(ww);
             
					  m_edtContent="";
					  int m= m_lstContent.GetCurSel();
					  if(m_lstContent.GetCount()>10)
					  {				  
						 m_lstContent.SetCurSel(m_lstContent.GetCount()-1);
					  }			 
					  UpdateData(FALSE);
					  ww="";
					  ww.Empty();
			}
		ContentType=1;
	}
}


BOOL CMyDlgChat::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==13)
		return FALSE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMyDlgChat::OnBnClickedButton2() //cancel
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DestroyWindow();
}


void CMyDlgChat::OnBnClickedButton3()  //���ļ�
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ContentType=2;
	CFileDialog FileDlg(TRUE,0,0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,NULL,this);
	
	 if(FileDlg.DoModal() == IDOK) {
		sname=FileDlg.GetFileName(); //�õ��ļ���

		GetDlgItem(IDC_EDIT2)->SetWindowText(FileDlg.GetPathName());
		ShowWindow(SW_SHOW);
		UpdateData(TRUE);
	 }
}


void CMyDlgChat::OnBnClickedButton4() //ѡ��ͼƬ
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ContentType=3;
	CFileDialog FileDlg(TRUE,0,0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,NULL,this);
	
	 if(FileDlg.DoModal() == IDOK) {
		sname=FileDlg.GetFileName(); //�õ��ļ���

		//GetDlgItem(IDC_EDIT2)->SetWindowText(FileDlg.GetPathName());
		GetDlgItem(IDC_EDIT2)->ModifyStyle(0xF,SS_BITMAP|SS_CENTERIMAGE);
		path=FileDlg.GetPathName();

		//CClientDC dc(this);
		CDC*pDC=NULL;
		pDC=GetDlgItem(IDC_EDIT2)->GetDC();

		CFileStatus fstatus;
		CFile file;
		ULONGLONG cb;

		// ���ļ�������ļ�����Ч��  
		if (!file.Open(path,CFile::modeRead))  
		 {  
			 return;  
		 }  
		 if (!file.GetStatus(path,fstatus))  
		 {  
			 return ;  
		 }  
		 if ((cb =fstatus.m_size)<=0)  
		 {  
			 return ;  
		 }  	

		  // �����ļ���С�����ڴ�ռ�,�ǵ��ͷ��ڴ�  
		 HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, (unsigned int)cb);    
		  if (hGlobal== NULL)   
		  {  
			  return ;  
		  }  

		    // �����ղŷ�����ڴ�ռ�  
			  LPVOID pvData = NULL;    
			  pvData = GlobalLock(hGlobal);  
			  if (pvData == NULL)    
			 {    
				 GlobalFree(hGlobal);  // �ǵ��ͷ��ڴ�  
				 return ;  
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
		  return;
	  }
	   OLE_XSIZE_HIMETRIC hmWidth;    
	  OLE_YSIZE_HIMETRIC hmHeight;    
	  pPic->get_Width(&hmWidth);    
	  pPic->get_Height(&hmHeight);
	  RECT rtWnd;  
	  pDC->GetWindow()->GetWindowRect(&rtWnd);  
	  int iWndWidth=rtWnd.right-rtWnd.left;  
	  int iWndHeight=rtWnd.bottom-rtWnd.top; 
	  	//Invalidate(false);
	  if(FAILED(pPic->Render(*pDC,0,0,hmHeight/10,hmWidth/10,0,hmHeight,hmWidth,-hmHeight,NULL)))    
	  {  
		  pPic->Release();  
		  GlobalFree(hGlobal);  // �ǵ��ͷ��ڴ�  
		  return ;  
	  }  
	  ReleaseDC(pDC);
	   pPic->Release();   
	   GlobalFree(hGlobal);  // �ǵ��ͷ��ڴ�  
		ShowWindow(SW_SHOW);
		//Invalidate(false);
		UpdateData(FALSE);
	 }

}




void CMyDlgChat::OnBnClickedButton5()   //��Ƶ
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CVideoDlg *VideoDlg=new CVideoDlg;
	VideoDlg->Create(IDD_VIDEODLG);
	 VideoDlg->ShowWindow(SW_SHOW);
	 //��������������Ƶ�����

}


void CMyDlgChat::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	SendMessage(WM_SYSCOMMAND,0xF012,0);
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMyDlgChat::OnBnClickedButton6()  //��Ӻ���
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ʾ�����б�
	CChatFriendDlg * pDlgChat=new CChatFriendDlg;
	pDlgChat->Create(IDD_CHAT_FRD_DLG);
	pDlgChat->ShowWindow(SW_SHOW);
	pDlgChat->SetWindowText(_T("��Ӻ���"));

}
