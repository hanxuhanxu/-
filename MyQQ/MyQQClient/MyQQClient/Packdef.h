#include"../../Res.h"
//#ifndef _PACKDEF_H
//#define _PACKDEF_H 
//
//#define _HOST_IP  inet_addr("10.51.211.173")
//#define _DEF_MY_IP  inet_addr("10.51.211.173")
////边界值
//#define _DEF_PORT 1234
//#define _DEF_SIZE 4096
//#define _DEF_BUFFERSIZE  200*1024
//#define _MAX_PROTOCOL_LEN 100
//#define _MAX_PATH_LEN  100
//#define _MAX_NAME_LEN  100
//#define _DEF_STREAMSIZE /*1000*/100*1024
//
////消息
//#define UM_ONLINEMSG   WM_USER + 1
//#define UM_OFFLINEMSG  WM_USER + 2
//#define UM_DATAINFOMSG  WM_USER + 3
//#define UM_DESTROYMSG  WM_USER + 4 
//#define UM_FILEINFOMSG  WM_USER + 5
////网络消息
//#define NM_READMSG  WM_USER + 50
//
////协议包类型
//
//#define _DEF_PROTOCOL_BASE  100
//#define _DEF_PROTOCOL_ONLINE_RQ     _DEF_PROTOCOL_BASE  +0
//#define _DEF_PROTOCOL_ONLINE_RS     _DEF_PROTOCOL_BASE  +1
//
//#define _DEF_PROTOCOL_OFFLINE_RQ     _DEF_PROTOCOL_BASE  +2
//
//
//#define _DEF_PROTOCOL_DATAINFO_RQ    _DEF_PROTOCOL_BASE  +3
//#define _DEF_PROTOCOL_DATAINFO_RS    _DEF_PROTOCOL_BASE  +4
//#define  _DEF_PROTOCOL_REGISTER_RQ     _DEF_PROTOCOL_BASE  +5
//#define  _DEF_PROTOCOL_REGISTER_RS     _DEF_PROTOCOL_BASE  +6
//#define _DEF_PROTOCOL_FILESTREAM_RQ		_DEF_PROTOCOL_BASE +7
//#define _DEF_PROTOCOL_FILESTREAM_RS		_DEF_PROTOCOL_BASE +8
//
////
////协议包
//typedef int PackdefType;
////上线请求 上线回复 下线请求
//
//typedef struct STRU_ONLINE
//{
//	PackdefType m_nType;//协议类型
//	char m_username[64];
//	char m_password[64];
//	
//}STRU_OFFLINE;
//
//struct STRU_ONLINERS
//{
//	PackdefType m_nType;//协议类型
//	BOOL IfSuccess;
//};
//
//struct STRU_DATAINFO_RS
//{
//	PackdefType m_nType;
//	char        m_szBuf[_DEF_SIZE];
//	char      m_SourceIP[20];//对方ip
//};
//
//struct STRU_REGISTER_RS
//{
//	PackdefType m_nType;//协议类型
//	BOOL IfSuccess;
//};
//
// struct STRU_DATAINFO
//{
//	PackdefType m_nType;
//	char      m_szBuf[_DEF_SIZE];
//	int         m_nNum;  //序号
//	char      m_ToIP[20];  //要聊天的对方ip
//	//char      m_MyIP[20];//我的ip
//};
//
//static long GetValidIp()
//	{
//		char szName[20] = {0};
//		struct hostent *remoteHost = NULL;
//		long ip;
//		struct in_addr addr;
//
//		//1.获得主机名
//		if(!gethostname(szName,sizeof(szName)))
//		{
//				//2.IPLIST		
//			remoteHost = gethostbyname(szName);
//			int i = 0;
//			while (remoteHost->h_addr_list[i] != 0) {
//                 addr.s_addr = *(u_long *) remoteHost->h_addr_list[i++];
//				// m_lstIP.push_back(addr.s_addr );
//				 ip= addr.s_addr ;
//            }
//		}
//		return ip;
//	}
//
//struct STRU_STARTTRANSFER_RQ
//{
//	 PackdefType m_nType;
//	 char m_szName[_DEF_SIZE];	
//};
//
//struct STRU_STARTTRANSFER_RS
//{
//	 PackdefType m_nType;
//	 char m_szName[_DEF_SIZE];	
//	 char szResult;
//};
//
//struct STRU_FILESTREAM_RQ
//{
//	 PackdefType m_nType;
//	 char m_szName[_DEF_SIZE];	
//	 char m_szStream[_DEF_STREAMSIZE];
//	 int  m_nlen;
//
//};
//
//struct STRU_FILE
//{
//	 PackdefType m_nType;
//	 char m_FileName[_MAX_NAME_LEN];	
//	 char mFileStream[_MAX_PATH_LEN];
//	 char mFileContent[_DEF_STREAMSIZE];
//	 int  m_FileSize;
//	 char  m_ToIP[20];
//};
//
//struct STRU_FILE_RS
//{
//	 PackdefType m_nType;
//	 char m_FileName[_MAX_NAME_LEN];	
//	 char mFileContent[_DEF_STREAMSIZE];
//	  int  m_FileSize;
//	  char      m_SourceIP[20];//发送端ip
//};
//
//#endif