// VideoDlg.cpp : ʵ���ļ�

#include "stdafx.h"
#include "MyQQClient.h"
#include "VideoDlg.h"
#include "afxdialogex.h"

//#pragma comment(lib,"./SDL.lib")

typedef unsigned char uint8_t;
SDL_Event g_event ;             //�����¼�
/*extern*/ CCriticalSection  cs;    //�ٽ������߳���
/*extern*/ list<uint8_t> packetList;//�����б����ڴ洢��Ƶ֡����
int g_size;   //YUV420ͼ���С
int g_width;
int g_height;


// CVideoDlg �Ի���

IMPLEMENT_DYNAMIC(CVideoDlg, CDialogEx)

HWND g_hWnd=0;

CVideoDlg::CVideoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoDlg::IDD, pParent)
	, m_VideoYours(0)
	, m_VideoMine(0)
{

}

CVideoDlg::~CVideoDlg()
{
}

void CVideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FRIEND, m_VideoFriend);
	DDX_Control(pDX, IDC_MINE, m_VideoSelf);
}


BEGIN_MESSAGE_MAP(CVideoDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CVideoDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CVideoDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CVideoDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CVideoDlg ��Ϣ�������

static int yuyv_to_yuv420P(unsigned char yuv422[], unsigned char yuv420[], int width, int height)
{
	int ynum=width*height;
    int i,j,k=0;
	//�õ�Y����
	for(i=0;i<ynum;i++)
	{
		yuv420[i]=yuv422[i*2];
	}
	//�õ�U����	   
	for(i=0;i<height;i++)
	{		   
		if((i%2)!=0)
			continue;		   
		for(j=0;j<(width/2);j++)
		{			  
			if((4*j+1)>(2*width))
				break;			   
			yuv420[ynum+k*2*width/4+j]=yuv422[i*2*width+4*j+1];			  		   
		}		    
		k++;	   
	}	   
	k=0;
	//�õ�V����	   
	for(i=0;i<height;i++)
	{		   
		if((i%2)==0)
			continue;		   
		for(j=0;j<(width/2);j++)
		{			   
			if((4*j+3)>(2*width))
				break;			   
			yuv420[ynum+ynum/4+k*2*width/4+j]=yuv422[i*2*width+4*j+3];			  		   
		}		    
		k++;	  
	}	   	          
	return 1;
}

//�ص�����
LRESULT CALLBACK capVideoStreamCallback(HWND hwnd, LPVIDEOHDR lpVHdr)
{
	//unsigned char filebuf[38017] = {0};
	//CVideoDlg* VFWobj=( CVideoDlg*)capGetUserData(hwnd);
	//yuyv_to_yuv420P(lpVHdr->lpData,filebuf,176,144);
	//g_size=176*144*3/2;
	//cs.Lock();
	//for(int i=0;i<g_size;i++)
	//{
 //        packetList.push_back(filebuf[i]);
	//}
	//cs.Unlock();

	////g_event.type=SFM_PEFRESH_EVENT;
	//CWinThread * m_CaptureThread;
	//m_CaptureThread=AfxBeginThread(CaptureThreadFunc,(void*) VFWobj);
	                                                           //�����߳�
	return 0;
}

BOOL CVideoDlg::OnInitDialog()
{
	//HWND g_hWnd;
	CDialogEx::OnInitDialog();
	BOOL m_bInit = FALSE;
 //   CWnd *pWnd = GetDlgItem(IDC_MINE);//�õ�Ԥʾ����ָ��
 //   CRect rect;
 //   pWnd->GetWindowRect(&rect); 
	////���񴰿ڵľ��
 //   g_hWnd = capCreateCaptureWindow(NULL,WS_CHILD|WS_VISIBLE|WS_EX_CLIENTEDGE|WS_EX_DLGMODALFRAME,
	//	0,0,rect.Width(),rect.Height(),
 //      pWnd->GetSafeHwnd(),0); // ����Ԥʾ���� ���񴰿�

	//if (g_hWnd == 0)
	//{
	//	AfxMessageBox(_T("����AVI����ʧ��"));
	//	return FALSE;
	//}
	//
	//capSetCallbackOnVideoStream(g_hWnd, capVideoStreamCallback);

 //   capCaptureSequenceNoFile  ( g_hWnd); //��ʼ����

	//while(!capDriverConnect(g_hWnd,0))
	//{
	//	m_bInit = capDriverConnect(g_hWnd,0);  //����0��������
	//    TRACE("%d\n",WSAGetLastError());
	//}
	////�������������
	//CAPDRIVERCAPS m_CapDrvCap;
 // 	capDriverGetCaps(g_hWnd,sizeof(CAPDRIVERCAPS),&m_CapDrvCap);
	//BITMAPINFO m_InInfo;
	//capGetVideoFormat(g_hWnd,&m_InInfo,sizeof(BITMAPINFO));
	//g_width=m_InInfo.bmiHeader.biWidth;
	//g_height=m_InInfo.bmiHeader.biHeight;

	//CAPSTATUS m_CapStatus;

	//if(m_CapDrvCap.fCaptureInitialized)    
	//{
	//	capSetUserData(g_hWnd,this);  //ָ��󶨾��
	//	capGetStatus(g_hWnd, &m_CapStatus,sizeof(m_CapStatus));
	//	capPreviewRate(g_hWnd,30); 
	//	capPreviewScale(g_hWnd,TRUE);		
	//	capPreview(g_hWnd,TRUE);           //Ԥ����Ƶ
 //       //ShowWindow(SW_SHOW);
	//}

	return TRUE;  

}



BOOL CVideoDlg::DestroyWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	//capCaptureAbort(g_hWnd);
	//capDriverDisconnect(g_hWnd);
	//capCaptureStop(g_hWnd);
	return CDialogEx::DestroyWindow();
}


void CVideoDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DestroyWindow();
}


void CVideoDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//capDlgVideoFormat(g_hWnd);   //��Ƶ��ʽ
	//capDlgVideoSource(g_hWnd);   //��ƵԴ
	//capGrabFrame(g_hWnd);   //�������� ���ûص�����
	m_ffmpeg.SetStart(m_bdesktop,m_bcamera,m_baudio);
	//���Ϳ�ʼ��Ƶ��֪ͨ
}


void CVideoDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bdesktop  = ((CButton*)GetDlgItem(IDC_DESKTOP))->GetCheck();
	m_bcamera  = ((CButton*)GetDlgItem(IDC_CAMERA))->GetCheck();
	m_baudio  = ((CButton*)GetDlgItem(IDC_AUDIO))->GetCheck();
	m_ffmpeg.Factory(m_bdesktop,m_bcamera,m_baudio);

}
