// VideoDlg.cpp : 实现文件

#include "stdafx.h"
#include "MyQQClient.h"
#include "VideoDlg.h"
#include "afxdialogex.h"

//#pragma comment(lib,"./SDL.lib")

typedef unsigned char uint8_t;
SDL_Event g_event ;             //触发事件
/*extern*/ CCriticalSection  cs;    //临界区，线程锁
/*extern*/ list<uint8_t> packetList;//公共列表，用于存储视频帧数据
int g_size;   //YUV420图像大小
int g_width;
int g_height;


// CVideoDlg 对话框

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


// CVideoDlg 消息处理程序

static int yuyv_to_yuv420P(unsigned char yuv422[], unsigned char yuv420[], int width, int height)
{
	int ynum=width*height;
    int i,j,k=0;
	//得到Y分量
	for(i=0;i<ynum;i++)
	{
		yuv420[i]=yuv422[i*2];
	}
	//得到U分量	   
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
	//得到V分量	   
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

//回调函数
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
	                                                           //解码线程
	return 0;
}

BOOL CVideoDlg::OnInitDialog()
{
	//HWND g_hWnd;
	CDialogEx::OnInitDialog();
	BOOL m_bInit = FALSE;
 //   CWnd *pWnd = GetDlgItem(IDC_MINE);//得到预示窗口指针
 //   CRect rect;
 //   pWnd->GetWindowRect(&rect); 
	////捕获窗口的句柄
 //   g_hWnd = capCreateCaptureWindow(NULL,WS_CHILD|WS_VISIBLE|WS_EX_CLIENTEDGE|WS_EX_DLGMODALFRAME,
	//	0,0,rect.Width(),rect.Height(),
 //      pWnd->GetSafeHwnd(),0); // 设置预示窗口 捕获窗口

	//if (g_hWnd == 0)
	//{
	//	AfxMessageBox(_T("创建AVI窗口失败"));
	//	return FALSE;
	//}
	//
	//capSetCallbackOnVideoStream(g_hWnd, capVideoStreamCallback);

 //   capCaptureSequenceNoFile  ( g_hWnd); //开始捕获

	//while(!capDriverConnect(g_hWnd,0))
	//{
	//	m_bInit = capDriverConnect(g_hWnd,0);  //连接0号驱动器
	//    TRACE("%d\n",WSAGetLastError());
	//}
	////获得驱动器性能
	//CAPDRIVERCAPS m_CapDrvCap;
 // 	capDriverGetCaps(g_hWnd,sizeof(CAPDRIVERCAPS),&m_CapDrvCap);
	//BITMAPINFO m_InInfo;
	//capGetVideoFormat(g_hWnd,&m_InInfo,sizeof(BITMAPINFO));
	//g_width=m_InInfo.bmiHeader.biWidth;
	//g_height=m_InInfo.bmiHeader.biHeight;

	//CAPSTATUS m_CapStatus;

	//if(m_CapDrvCap.fCaptureInitialized)    
	//{
	//	capSetUserData(g_hWnd,this);  //指针绑定句柄
	//	capGetStatus(g_hWnd, &m_CapStatus,sizeof(m_CapStatus));
	//	capPreviewRate(g_hWnd,30); 
	//	capPreviewScale(g_hWnd,TRUE);		
	//	capPreview(g_hWnd,TRUE);           //预览视频
 //       //ShowWindow(SW_SHOW);
	//}

	return TRUE;  

}



BOOL CVideoDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	//capCaptureAbort(g_hWnd);
	//capDriverDisconnect(g_hWnd);
	//capCaptureStop(g_hWnd);
	return CDialogEx::DestroyWindow();
}


void CVideoDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	DestroyWindow();
}


void CVideoDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	//capDlgVideoFormat(g_hWnd);   //视频格式
	//capDlgVideoSource(g_hWnd);   //视频源
	//capGrabFrame(g_hWnd);   //发送数据 调用回调函数
	m_ffmpeg.SetStart(m_bdesktop,m_bcamera,m_baudio);
	//发送开始视频的通知
}


void CVideoDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bdesktop  = ((CButton*)GetDlgItem(IDC_DESKTOP))->GetCheck();
	m_bcamera  = ((CButton*)GetDlgItem(IDC_CAMERA))->GetCheck();
	m_baudio  = ((CButton*)GetDlgItem(IDC_AUDIO))->GetCheck();
	m_ffmpeg.Factory(m_bdesktop,m_bcamera,m_baudio);

}
