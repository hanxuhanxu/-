#include "stdafx.h"
#include "TCPServer.h"

bool bFlagQuit;
SOCKET m_sockerWaiter;
IMediator* m_pMediator;
CTCPServer::CTCPServer(IMediator* pMediator)
{
	m_socketListen=NULL;
	bFlagQuit=true;
	m_pMediator=pMediator;
	m_hThread=NULL;
	m_sockerTCP=NULL;
	m_hThreadTCP=NULL;
	m_sockerWaiter=NULL;
	m_sockSend=NULL;
}

struct Game{
	char GameName[64];
	char GameInfo[4096];
	char  GameNum[20];
};

struct STRU_HALL_GAMELIST_RS  //游戏列表回复包
{
	PackdefType m_nType;
	Game	m_szGameList[5];
	int GameCount;   //游戏总数
};




CTCPServer::~CTCPServer(void)
{
}

bool CTCPServer::InitNetWork() 
{
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested=MAKEWORD(2,2);
	int err=WSAStartup(wVersionRequested,&wsaData);
	if(err!=0)
	{
		return false;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
        UnInitNetWork();
        return false;
    }


	//2.雇个人  -- 创建套接字（与外界通信接口）-
	m_socketListen=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	m_sockerTCP=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	m_sockSend=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	//u_long iMode = 0;
	BOOL bval = TRUE;
	setsockopt(m_sockSend,SOL_SOCKET,SO_BROADCAST,(const char*)&bval,sizeof(bval));
	//ioctlsocket(m_socketListen,FIONBIO,&iMode);
	//ioctlsocket(m_sockerTCP,FIONBIO,&iMode);

	if(m_socketListen == INVALID_SOCKET)
	{
		UnInitNetWork();
        return false;
	}
	if(m_sockerTCP == INVALID_SOCKET)
	{
		UnInitNetWork();
        return false;
	}

	//3.绑定 bind
	sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(_DEF_PORT);
	addr.sin_addr.S_un.S_addr=GetValidIp();
	if(SOCKET_ERROR == bind(m_socketListen,(const sockaddr*)&addr,sizeof(addr)))
	{
		UnInitNetWork();
		TRACE("%d\n",GetLastError());
		return false;
	}
	sockaddr_in addr2;
	addr2.sin_family=AF_INET;
	addr2.sin_port=htons(8888);
	addr2.sin_addr.S_un.S_addr=GetValidIp();
	if(SOCKET_ERROR == bind(m_sockerTCP,(const sockaddr*)&addr2,sizeof(addr2)))
	{
		UnInitNetWork();
		TRACE("%d\n",GetLastError());
		return false;
	}

	m_we=WSACreateEvent();
	WSAEventSelect(m_socketListen,m_we,FD_READ|FD_WRITE);
	////清空集合
	//FD_ZERO(&fdsets);
	////将socket加入
	//FD_SET(m_socketListen,&fdsets);
	//FD_SET(m_sockerTCP,&fdsets);
	////交给select管理
	// TIMEVAL  timeval;
	// select(0,&fdsets,&fdsets,NULL,&timeval);
	// //校验集合
	// if( !FD_ISSET(m_socketListen,&fdsets))
	// {
	//	 return false;
	// }

	//  if( !FD_ISSET(m_sockerTCP,&fdsets))
	// {
	//	 return false;
	// }

	////监听
	if(listen(m_sockerTCP,20)==SOCKET_ERROR)
	{
		UnInitNetWork();
		return false;
	}

	//创建线程
	m_hThread=(HANDLE)_beginthreadex(NULL,0,&ThreadProc,this,0,0);
	m_hThreadTCP=(HANDLE)_beginthreadex(NULL,0,&ThreadAccept,this,0,0);
	return true;

}

unsigned  _stdcall  CTCPServer::ThreadProc( void * lpvoid)
{
	CTCPServer * pthis=(CTCPServer*)lpvoid;

	//recv
	char szbuf[_DEF_BUFFERSIZE]={0};
	sockaddr_in addrClient;
	int nClientSize=sizeof(sockaddr_in);
	int nRelReadNum;
	
	while(bFlagQuit)
	{
		//if( ! pthis->SelectSocket())
		//{
		//	continue;
		//}
		WaitForSingleObject(pthis->m_we,INFINITE);
		TRACE("%d\n",GetLastError());
		ResetEvent(pthis->m_we);

		nRelReadNum=recvfrom(pthis->m_socketListen,szbuf,_DEF_BUFFERSIZE,0,(sockaddr *)&addrClient,&nClientSize);

		if(nRelReadNum>0)
		{
			//处理
			STRU_QUNLIAO_RQ * sd=(STRU_QUNLIAO_RQ*)szbuf;
			m_pMediator->DealData(szbuf,addrClient.sin_addr.S_un.S_addr);
		}
	}
	return 0;
}

unsigned  __stdcall CTCPServer::ThreadAccept( void * lpvoid )
{
	CTCPServer * pthis=(CTCPServer*)lpvoid;
	
	while(/*pthis->*/bFlagQuit)
	{
		//if( ! pthis->SelectSocket())
		//{
		//	continue;
		//}
		/*SOCKET */m_sockerWaiter=accept(pthis->m_sockerTCP,NULL,NULL);
			if(m_sockerWaiter == INVALID_SOCKET)
				continue;

			HANDLE m_hThreadRecv = (HANDLE)_beginthreadex(NULL,0,&ThreadRecv,(void * )m_sockerWaiter,0,0);
			if(m_hThreadRecv)
			{
				pthis->m_lsthThreadRecv.push_back(m_hThreadRecv);
			}
	}
	return 0;
}

unsigned  __stdcall CTCPServer::ThreadRecv( void * lpvoid )
{
	//CTCPServer * pthis=(CTCPServer*)lpvoid;
	/*SOCKET*/ m_sockerWaiter = (SOCKET)lpvoid;
	char* pszbuf = NULL;
	int nRelReadNum;
	int nPackSize;
	while(/*CTCPServer::*/bFlagQuit)
	{
		
		//接收包大小
		//nRelReadNum =  recv(m_sockerWaiter,(char*)&nPackSize,sizeof(int),0);
		//if(nRelReadNum <=0)
		//{
		//	//判断是不是客户端下线
		//	if(WSAECONNRESET == GetLastError())
		//	{
		//		closesocket(m_sockerWaiter);
		//		m_sockerWaiter = NULL;
		//		break;
		//	}
		//	continue;
		//}
			
		//接收完整数据包
		/* pszbuf = new char[nPackSize];
		 int noffset = 0;
		 while(nPackSize)
		 {*/
		char pszbuf[21000]={0};
			nRelReadNum =  recv(m_sockerWaiter,pszbuf,21000,0);
	/*		noffset += nRelReadNum;
			nPackSize -= nRelReadNum; 
		 }*/

		 STRU_HALL_GAMELIST_RS sh;
		 sh.m_nType=105;
	sh.GameCount=2;
	SOCKET sock=NULL;
	Game game1;
	//game1.GameName="五子棋";
	strcpy_s((char*)&game1.GameName,(size_t)sizeof(game1.GameName),"五子棋");
	//game1.GameInfo="这是五子棋游戏";
	strcpy_s((char*)&game1.GameInfo,(size_t)sizeof(game1.GameInfo),"这是五子棋游戏");
	Game game2;
	//game2.GameName="斗地主";
	//game2.GameInfo="这是斗地主游戏";
	strcpy_s((char*)&game2.GameName,(size_t)sizeof(game2.GameName),"斗地主");
	//game1.GameInfo="这是五子棋游戏";
	strcpy_s((char*)&game2.GameInfo,(size_t)sizeof(game2.GameInfo),"这是斗地主游戏");

	sh.m_szGameList[0]=game1;
	sh.m_szGameList[1]=game2;

	m_pMediator->SendDataTCP(inet_addr("10.50.131.160"),(char*)&sh,sizeof(sh));
	 
		//交给核心处理类处理
		 m_pMediator->DealDataTCP(pszbuf,m_sockerWaiter);

		 //delete []pszbuf;
		 //pszbuf =  NULL;
	}
	return 0;
}

bool CTCPServer::SendData(long lSendIP, char *szbuf ,int nlen)
{
	sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(8000);
	addr.sin_addr.S_un.S_addr=lSendIP;
	if(!szbuf || nlen <=0)
		return false;
	int * ptype=(int *)szbuf;
	switch( * ptype)
	{
	case _DEF_PROTOCOL_ONLINE_RS:
		{
				STRU_ONLINERS*sd=(STRU_ONLINERS*)szbuf;

				if(sendto(m_socketListen,(const char *)sd,nlen,0,(sockaddr *)&addr,sizeof(addr))<=0)
				{
					TRACE("%d\n",WSAGetLastError());
					 return false;
			    }
		}
		break;

	case _DEF_PROTOCOL_KEEP_CONNECT_RQ:
		{
				STRU_KEEP_CONNECT*sk=(STRU_KEEP_CONNECT*)szbuf;

				if(sendto(m_socketListen,(const char *)sk,nlen,0,(sockaddr *)&addr,sizeof(addr))<=0)
				{
					TRACE("%d\n",WSAGetLastError());
					 return false;
			    }
		}
		break;

	case _DEF_PROTOCOL_DATAINFO_RS:
		{
			STRU_DATAINFO_RS *sdr=(STRU_DATAINFO_RS *)szbuf;
			if(sendto(m_socketListen,(const char *)sdr,nlen,0,(sockaddr *)&addr,sizeof(addr))<=0)
			{
				TRACE("%d\n",WSAGetLastError());
					return false;
			}
		}
		break;
	case _DEF_PROTOCOL_QUNLIAO_INFO_RQ:
		{
			STRU_DATAINFO_RS *sdr=(STRU_DATAINFO_RS *)szbuf;
			if(sendto(m_sockSend,(const char *)sdr,nlen,0,(sockaddr *)&addr,sizeof(addr))<=0)
			{
				TRACE("%d\n",WSAGetLastError());
					return false;
			}
		}
		break;
	case _DEF_PROTOCOL_JOIN_QUNLIAO_MSG:
		{
			STRU_JOIN_QUNLIAO* sj=(STRU_JOIN_QUNLIAO*)szbuf;
			if(sendto(m_sockSend,(const char *)sj,nlen,0,(sockaddr *)&addr,sizeof(addr))<=0)
			{
				TRACE("%d\n",WSAGetLastError());
					return false;
			}
		}
		break;

	case _DEF_PROTOCOL_OFFLINE_RQ:
		{
			STRU_OFFLINE *so=(STRU_OFFLINE *)szbuf;
			if(sendto(m_socketListen,(const char *)so,nlen,0,(sockaddr *)&addr,sizeof(addr))<=0)
			{
				TRACE("%d\n",WSAGetLastError());
					return false;
			}
		}
		break;
	
	case _DEF_PROTOCOL_REGISTER_RS:
		{
				STRU_REGISTER_RS*sd=(STRU_REGISTER_RS*)szbuf;

				if(sendto(m_socketListen,(const char *)sd,nlen,0,(sockaddr *)&addr,sizeof(addr))<=0)
				{
					TRACE("%d\n",WSAGetLastError());
					 return false;
			    }
		}
		break;
	case _DEF_PROTOCOL_QUNLIAO_RQ:
		{
				STRU_QUNLIAO_RQ*sq=(STRU_QUNLIAO_RQ*)szbuf;

				if(sendto(m_socketListen,(const char *)sq,nlen,0,(sockaddr *)&addr,sizeof(addr))<=0)
				{
					TRACE("%d\n",WSAGetLastError());
					 return false;
			    }
		}
		break;
	case _DEF_PROTOCOL_QUNLIAO_RS:
		{
				STRU_QUNLIAO_RS*sd=(STRU_QUNLIAO_RS*)szbuf;

				if(sendto(m_socketListen,(const char *)sd,nlen,0,(sockaddr *)&addr,sizeof(addr))<=0)
				{
					TRACE("%d\n",WSAGetLastError());
					 return false;
			    }
		}
		break;
    }

	return true;
}

bool CTCPServer::SendDataTCP(SOCKET sock, char *szbuf ,int nlen) 
{
	if(!szbuf || nlen <=0)
	return false;
	int * ptype=(int *)szbuf;
	switch ( * ptype)
	{
		case _DEF_PROTOCOL_FILESTREAM_RS:
		{
			STRU_FILE_RS * sf=(STRU_FILE_RS * )szbuf;

			//包大小
			if(send(sock,(const char *)&nlen,sizeof(int),0)<0)
				{
					TRACE("%d\n",WSAGetLastError());
					 return false;
			    }
			//包内容
			if(send(sock,(const char *)sf,nlen,0)<0)
				return false;

		}
		break;
	case _DEF_PROTOCOL_FILESTREAMINFO_RS:
		{
			STRU_FILE_RS * sf=(STRU_FILE_RS * )szbuf;

			//包大小
			if(send(sock,(const char *)&nlen,sizeof(int),0)<0)
				{
					TRACE("%d\n",WSAGetLastError());
					 return false;
			    }
			//包内容
			if(send(sock,(const char *)sf,nlen,0)<0)
				return false;

		}
		break;
		case _DEF_PROTOCOL_PICTURE_RS:
			{
				STRU_PIC_RQ * sprq=(STRU_PIC_RQ*) szbuf;
				//包大小
			if(send(sock,(const char *)&nlen,sizeof(int),0)<0)
				{
					TRACE("%d\n",WSAGetLastError());
					 return false;
			    }
			//包内容
			if(send(sock,(const char *)sprq,nlen,0)<0)
				return false;
			}
			break;
		default:
			{
				STRU_HALL_GAMELIST_RS* sh=(STRU_HALL_GAMELIST_RS*)szbuf;
				if(send(sock,(const char *)sh,nlen,0)<0)
				{
						TRACE("%d\n",WSAGetLastError());
return false;
				}
				
			}
			break;
	}
	return 0;
}

bool CTCPServer::SelectSocket()
{
	timeval tv;
	tv.tv_sec=0;
	tv.tv_usec=100;

	fd_set fdsets;
	FD_ZERO(&fdsets);
	FD_SET(m_socketListen,&fdsets);
	FD_SET(m_sockerWaiter,&fdsets);
	select(NULL,&fdsets,NULL,NULL,&tv);
	if(  ( FD_ISSET(m_socketListen,&fdsets)==NULL)&&( FD_ISSET(m_sockerWaiter,&fdsets)==NULL))
	{
		if(FD_ISSET(m_socketListen,&fdsets)==NULL)
		{
			//
			int a =0;
		}
		if(FD_ISSET(m_sockerWaiter,&fdsets)==NULL)
		{
			//
			int a=1;
		}
		return false;
	}
	 //if( !FD_ISSET(m_sockerTCP,&fdsets))
	 //{
		// return false;
	 //}
	return true;

		////清空集合
	//FD_ZERO(&fdsets);
	////将socket加入
	//FD_SET(m_socketListen,&fdsets);
	//FD_SET(m_sockerTCP,&fdsets);
	////交给select管理
	// TIMEVAL  timeval;
	// select(0,&fdsets,&fdsets,NULL,&timeval);
	// //校验集合
	// if( !FD_ISSET(m_socketListen,&fdsets))
	// {
	//	 return false;
	// }

	//  if( !FD_ISSET(m_sockerTCP,&fdsets))
	// {
	//	 return false;
	// }
}

void CTCPServer::UnInitNetWork()
{
	bFlagQuit=false;
	WSACleanup();
	 if(m_socketListen)
	 {
		 closesocket(m_socketListen);
		m_socketListen= INVALID_SOCKET;
		 m_socketListen = NULL;
	 }
	  if(m_sockerTCP)
	 {
		 closesocket(m_sockerTCP);
		m_sockerTCP= INVALID_SOCKET;
		 m_sockerTCP = NULL;
	 }//m_sockerWaiter

	   if(m_sockerWaiter)
	 {
		 closesocket(m_sockerWaiter);
		m_sockerWaiter= INVALID_SOCKET;
		 m_sockerWaiter = NULL;
	 }
	  if(m_hThread)
	 {
		 if( WaitForSingleObject(m_hThread,100) == WAIT_TIMEOUT)
		 {
			 TerminateThread(m_hThread,-1);
			 m_hThread=NULL;
		 }
		 CloseHandle(m_hThread);
		 m_hThread=NULL;
	 }

	   if(m_hThreadTCP)
	 {
		 if( WaitForSingleObject(m_hThreadTCP,100) == WAIT_TIMEOUT)
		 {
			 TerminateThread(m_hThreadTCP,-1);
			 m_hThreadTCP=NULL;
		 }
		 CloseHandle(m_hThreadTCP);
		 m_hThreadTCP=NULL;
	 }
}
