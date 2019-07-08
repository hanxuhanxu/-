#ifndef _PACKDEF_H
#define _PACKDEF_H 

#include<list>
#include <string>
#include<iostream>
using namespace std;

#define _HOST_IP  inet_addr(/*"10.51.211.173"*//*"192.168.216.1"*/"10.50.131.160"/*"192.168.43.187"*/)
#define _MY_IP  inet_addr("10.51.211.173"/*"192.168.43.161"*//*"10.50.131.160"*/)
#define _MY_IP_STR "10.51.211.173"/*"10.50.131.160"*/

//#define _HOST_IP  inet_addr("10.51.211.173"/*"192.168.43.161"*/)
//#define _MY_IP  inet_addr("10.51.211.173"/*"192.168.43.161"*/)
//#define _MY_IP_STR "10.51.211.173"
//�߽�ֵ
#define _DEF_PORT 1234
#define _DEF_SIZE 4096
#define _DEF_SQLLEN 200
#define _DEF_BUFFERSIZE  2000*1024
#define _MAX_PROTOCOL_LEN 100
#define _MAX_PATH_LEN  100
#define _MAX_NAME_LEN  100
#define _DEF_STREAMSIZE  1024/**1024*/
#define VIDEOCHAT_VIDEO_WIDTH			320						/// ��Ƶ�Ŀ�
#define VIDEOCHAT_VIDEO_HEIGHT			240						/// ��Ƶ�ĳ�
#define VIDEOCHAT_VIDEOWND_WIDTH		( 320 + 2 * 2 )			/// ��Ƶ���ڵĿ�
#define VIDEOCHAT_VIDEOWND_HEIGHT		( 240 + 2 * 2 )			/// ��Ƶ���ڵĳ�

#define AUDIO_BUFFER_SIZE				16384					/// ��Ƶ����������
#define AUDIO_BUFFER_BLOCK				5	
#define DEFALUT_VIDEOCHAT_REFRESH		( 2000 )				/// ��Ƶ����ˢ��	
#define TIMER_VIDEOCHAT					5						/// ��Ƶ�����timer��

#define TIMER_CONNECT					1
#define DEFAULT_REFRESH_TIME			( 10000 * 5 )			/// ˢ����ϵ���б��ʱ��,��λ����

//��Ϣ
#define UM_ONLINEMSG      WM_USER + 1
#define UM_OFFLINEMSG     WM_USER + 2
#define UM_DATAINFOMSG  WM_USER + 3
#define UM_DESTROYMSG  WM_USER + 4
#define UM_FILEINFOMSG   WM_USER + 5
#define UM_PICINFOMSG    WM_USER + 6
#define UM_ONLINE_ADD   WM_USER + 7
#define UM_OFFLINE_ADD  WM_USER + 8
#define UM_INVITEMSG       WM_USER + 9
#define UM_JOINMSG          WM_USER + 10
#define UM_JOIN_HIS_MSG   WM_USER + 11
#define UM_ONKEEPCONNECTMSG  WM_USER + 12
#define UM_FILEINFO  WM_USER + 13
//������Ϣ
#define NM_READMSG  WM_USER + 50

//Э�������

#define _DEF_PROTOCOL_BASE  100
#define _DEF_PROTOCOL_ONLINE_RQ                 _DEF_PROTOCOL_BASE  +0
#define _DEF_PROTOCOL_ONLINE_RS                  _DEF_PROTOCOL_BASE  +1
#define _DEF_PROTOCOL_OFFLINE_RQ                _DEF_PROTOCOL_BASE  +2
#define _DEF_PROTOCOL_DATAINFO_RQ            _DEF_PROTOCOL_BASE  +3
#define _DEF_PROTOCOL_DATAINFO_RS             _DEF_PROTOCOL_BASE  +4
#define  _DEF_PROTOCOL_REGISTER_RQ              _DEF_PROTOCOL_BASE  +5
#define  _DEF_PROTOCOL_REGISTER_RS               _DEF_PROTOCOL_BASE  +6
#define _DEF_PROTOCOL_FILESTREAM_RQ		      _DEF_PROTOCOL_BASE +7
#define _DEF_PROTOCOL_FILESTREAM_RS		      _DEF_PROTOCOL_BASE +8
#define _DEF_PROTOCOL_PICTURE_RQ		          _DEF_PROTOCOL_BASE +9
#define _DEF_PROTOCOL_PICTURE_RS		              _DEF_PROTOCOL_BASE +10
#define _DEF_PROTOCOL_QUNLIAO_RQ               _DEF_PROTOCOL_BASE +11
#define _DEF_PROTOCOL_QUNLIAO_RS                _DEF_PROTOCOL_BASE +12
#define _DEF_PROTOCOL_QUNLIAO_INFO_RQ     _DEF_PROTOCOL_BASE +13
#define _DEF_PROTOCOL_JOIN_QUNLIAO_MSG   _DEF_PROTOCOL_BASE +14
#define _DEF_PROTOCOL_KEEP_CONNECT_RQ     _DEF_PROTOCOL_BASE +15
#define _DEF_PROTOCOL_KEEP_CONNECT_RS      _DEF_PROTOCOL_BASE +16
#define _DEF_PROTOCOL_FILESTREAMINFO_RQ		      _DEF_PROTOCOL_BASE +17
#define _DEF_PROTOCOL_FILESTREAMINFO_RS		      _DEF_PROTOCOL_BASE +18

//
//Э���
typedef int PackdefType;
//�������� ���߻ظ� ��������

struct STRU_ONLINE
{
	PackdefType m_nType;//Э������
	char m_username[64];
	char m_password[64];
};

 struct STRU_OFFLINE
{
	PackdefType m_nType;//Э������
	char m_MyIP[20];
	char m_username[64];
};

struct STRU_ONLINERS
{
	PackdefType m_nType;//Э������
	BOOL IfSuccess;
	char m_username[64];
};

struct STRU_REGISTER_RS
{
	PackdefType m_nType;//Э������
	BOOL IfSuccess;
};


struct STRU_DATAINFO
{
	PackdefType m_nType;
	char        m_szBuf[_DEF_SIZE];
	int         m_nNum;  //���
	char      m_ToIP[20]; //���ն�ip
	//char      m_MyIP[20];//���Ͷ�ip
};

struct STRU_DATAINFO_RS
{
	PackdefType m_nType;
	char        m_szBuf[_DEF_SIZE];
	char      m_SourceIP[20];//���Ͷ�ip
};


struct STRU_STARTTRANSFER_RQ
{
	 PackdefType m_nType;
	 char m_szName[_DEF_SIZE];	
};

struct STRU_STARTTRANSFER_RS
{
	 PackdefType m_nType;
	 char m_szName[_DEF_SIZE];	
	 char szResult;
};

struct STRU_FILESTREAM_RQ
{
	 PackdefType m_nType;
	 char m_szName[_DEF_SIZE];	
	 char m_szStream[_DEF_STREAMSIZE];
	 int  m_nlen;

};

struct STRU_FILE
{
	 PackdefType m_nType;
	 char m_FileName[_MAX_NAME_LEN];	
	 char mFileStream[_MAX_PATH_LEN];
	 char mFileContent[_DEF_STREAMSIZE];
	  int  m_FileSize;  //ʵ���ļ���С
	  int m_TranSize;  //һ�δ���Ĵ�С 
	  int offset;
	  char  m_ToIP[20];
};

struct STRU_FILE_RS
{
	 PackdefType m_nType;
	 char m_FileName[_MAX_NAME_LEN];	
	 char mFileContent[_DEF_STREAMSIZE];
	  int  m_FileSize;    //ʵ���ļ���С
	  int m_TranSize;  //һ�δ���Ĵ�С 
	    int offset;
	  char      m_SourceIP[20];//���Ͷ�ip
};

struct STRU_PIC_RQ
{
	 PackdefType m_nType;
	 char mFileContent[_DEF_STREAMSIZE];
	  char mFilePath[_MAX_PATH_LEN];
	  int  m_FileSize;
	  char      m_SourceIP[20];//���Ͷ�ip
};

struct STRU_QUNLIAO_RQ
{
	 PackdefType m_nType;
	  char  m_ToIP[20];//���ն�ip
	 // list<CString>m_lstFriends; //�Ѿ����ڵĺ����б�
	  char      m_HadFriends[_DEF_SIZE];  //Ⱥ�Ĵ��ڵĺ���
	  char  m_MyIP[20];
};

struct STRU_QUNLIAO_RS
{
	 PackdefType m_nType;
	  char  m_ToIP[20];//���ն�ip
	  char  m_MyIP[20];
	  bool IsOK;  //�Ƿ��Ӧ����
};

struct STRU_JOIN_QUNLIAO
{
	 PackdefType m_nType;
	  char  m_HisIP[20];//���ն�ip
	  char  m_MyIP[20];
};

struct STRU_KEEP_CONNECT  //���󱣳����Ӱ�
{
	 PackdefType m_nType;
	  char  m_HisIP[20];//���ն�ip
	  char  m_MyIP[20];
};

static long GetValidIp()
	{
		char szName[20] = {0};
		struct hostent *remoteHost = NULL;
		long ip;
		struct in_addr addr;

		//1.���������
		if(!gethostname(szName,sizeof(szName)))
		{
				//2.IPLIST		
			remoteHost = gethostbyname(szName);
			int i = 0;
			while (remoteHost->h_addr_list[i] != 0) {
                 addr.s_addr = *(u_long *) remoteHost->h_addr_list[i++];
				 ip= addr.s_addr ;
            }
		}
		return ip;
	}


#endif