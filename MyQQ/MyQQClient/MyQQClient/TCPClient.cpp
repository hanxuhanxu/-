#include "stdafx.h"
#include "TCPClient.h"
#include"resource.h"

bool bFlagQuit;
CTCPClient::CTCPClient(IMediator* pMediator)
{
	bFlagQuit=true;
	m_socketClient=NULL;
	m_TCPsockClient=NULL;
	m_pMediator=pMediator;
	m_hThread=NULL;
	m_hThreadRecv=NULL;
	m_sockSend=NULL;
}


CTCPClient::~CTCPClient(void)
{
}

bool CTCPClient::InitNetWork()
{
	WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {

		UnInitNetWork();
        return false;
    }


    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {

       UnInitNetWork();
        return false;
    }

//2.�͸���  �ӿ� �׽���
	m_socketClient=socket(AF_INET ,SOCK_DGRAM ,IPPROTO_UDP );
	m_TCPsockClient=socket(AF_INET ,SOCK_STREAM ,IPPROTO_TCP );
	m_sockSend=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	//u_long iMode = 0;
	BOOL bval = TRUE;
	setsockopt(m_sockSend,SOL_SOCKET,SO_BROADCAST,(const char*)&bval,sizeof(bval));
	//ioctlsocket(m_socketClient,FIONBIO,&iMode);
	//ioctlsocket(m_TCPsockClient,FIONBIO,&iMode);

	if(INVALID_SOCKET==m_socketClient|| INVALID_SOCKET==m_sockSend )
	{
		UnInitNetWork();
        return false;
	}
	if(INVALID_SOCKET==m_TCPsockClient)
	{
		UnInitNetWork();
        return false;
	}
	TRACE("%d\n",GetLastError());
//3.ѡַ--����
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr =GetValidIp();/*inet_addr("10.51.211.173");*/
    addr.sin_port = htons(8000);
	if(SOCKET_ERROR == bind(m_socketClient,(const sockaddr*)&addr,sizeof(addr)))
	{
		UnInitNetWork();
			TRACE("%d\n",GetLastError());
		return false;
	}

	/*if(SOCKET_ERROR == connect(m_TCPsockClient,(const sockaddr*)&addr,sizeof(addr)))
	{
		TRACE("%d\n",GetLastError());
		UnInitNetWork();
		TRACE("%d\n",GetLastError());
		return false;
	}*/

	//	//��ռ���
	//FD_ZERO(&fdsets);
	////��socket����
	//FD_SET(m_socketClient,&fdsets);
	//FD_SET(m_TCPsockClient,&fdsets);
	////����select����
	// TIMEVAL  timeval;
	// select(0,&fdsets,&fdsets,NULL,&timeval);
	// //У�鼯��
	// if( !FD_ISSET(m_socketClient,&fdsets))
	// {
	//	 return false;
	// }

	//  if( !FD_ISSET(m_TCPsockClient,&fdsets))
	// {
	//	 return false;
	// }

	//�����߳�
	m_hThread=(HANDLE)_beginthreadex(NULL,0,&ThreadProc,this,0,0);
	m_hThreadRecv = (HANDLE)_beginthreadex(NULL,0,&ThreadRecv,this,0,0);
	TRACE("%d\n",GetLastError());
	return true;
}

unsigned  __stdcall  CTCPClient::ThreadProc( void * lpvoid)
{
	CTCPClient * pthis=(CTCPClient*)lpvoid;
	//recv
	char RecvBuf[_DEF_BUFFERSIZE]={0};
	sockaddr_in addrClient;
	int nClientSize=sizeof(sockaddr_in);
	int nRelReadNum=0;
	
	while(/*pthis->*/bFlagQuit)
	{
		//if( ! pthis->SelectSocket())
		//{
		//	continue;
		//}
		nRelReadNum=recvfrom(pthis->m_socketClient,RecvBuf,_DEF_BUFFERSIZE,0,(sockaddr *)&addrClient,&nClientSize);

		  TRACE("%d\n",GetLastError());

		if(nRelReadNum>0)
		{
			//����
			pthis->m_pMediator->DealData(RecvBuf,addrClient.sin_addr.S_un.S_addr);
		}
	}
	return 0;
}

bool CTCPClient::SelectSocket()
{
	timeval tv;
	tv.tv_sec=0;
	tv.tv_usec=100;

	fd_set fdsets;
	FD_ZERO(&fdsets);
	FD_SET(m_socketClient,&fdsets);
	FD_SET(m_TCPsockClient,&fdsets);
	select(NULL,&fdsets,NULL,NULL,&tv);
	if( ! FD_ISSET(m_socketClient,&fdsets))
	{
		return false;
	}
	 //if( !FD_ISSET(m_TCPsockClient,&fdsets))
	 //{
		// return false;
	 //}
	return true;

}

unsigned  __stdcall CTCPClient::ThreadRecv( void * lpvoid)
{
	CTCPClient* pthis  = (CTCPClient*)lpvoid;
	char* pszbuf = NULL;
	int nRelReadNum;
	int nPackSize;
	while(/*CTCPClient::*/bFlagQuit)
	{
		//if( ! pthis->SelectSocket())
		//{
		//	continue;
		//}
		//���հ���С
		nRelReadNum =  recv(pthis->m_TCPsockClient,(char*)&nPackSize,sizeof(int),0);
		if(nRelReadNum <=0)
		{
			continue;
		}
		HWND  mainHwnd = ::GetForegroundWindow();//��ǰ��˴��ھ��			
		//�����������ݰ�
		 pszbuf = new char[nPackSize];
		 int noffset = 0;
		 while(nPackSize)
		 {
			nRelReadNum =  recv(pthis->m_TCPsockClient,pszbuf+noffset,nPackSize,0);
			noffset += nRelReadNum;
			nPackSize -= nRelReadNum;
	 
		 }

		 	int * ptype=(int *)pszbuf;
			switch (* ptype)
			{
			case _DEF_PROTOCOL_FILESTREAM_RS:
				{
					// STRU_FILE_RS * sf=( STRU_FILE_RS *)pszbuf;
					// CString str="�Ƿ�����ļ���";
					// str+=sf->m_FileName;
					//if(IDOK== MessageBox(mainHwnd,str,_T("��ʾ"),MB_OKCANCEL))
					//{
						//�������Ĵ����ദ��
						 pthis->m_pMediator->DealDataTCP((char*)pszbuf,pthis->m_TCPsockClient);
					/*}
					else
					{
						return 0;
					}		
					 str="";
		             str.Empty();*/
				}
					 break;
			 case _DEF_PROTOCOL_FILESTREAMINFO_RS:
				{
					 STRU_FILE_RS * sf=( STRU_FILE_RS *)pszbuf;
					 CString str="�Ƿ�����ļ���";
					 str+=sf->m_FileName;
					if(IDOK== MessageBox(mainHwnd,str,_T("��ʾ"),MB_OKCANCEL))
					{
						//�������Ĵ����ദ��
						 pthis->m_pMediator->DealDataTCP((char*)pszbuf,pthis->m_TCPsockClient);
					}
					else
					{
						return 0;
					}		
					 str="";
		             str.Empty();
				}
					 break;

			case _DEF_PROTOCOL_PICTURE_RS:
				{
				      pthis->m_pMediator->DealDataTCP((char*)pszbuf,pthis->m_TCPsockClient);
				}
				break;
			default:
				break;
			}

		 delete []pszbuf;
		 pszbuf =  NULL;
	}
	return 0;
}

bool CTCPClient::SendData(long lSendIP, char *szbuf ,int nlen)
{
	int len=0;
	sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(/*_DEF_PORT*/8003);
	addr.sin_addr.S_un.S_addr=lSendIP;

	if(!szbuf || nlen <=0)
		return false;
	int * ptype=(int *)szbuf;
	if(* ptype==_DEF_PROTOCOL_QUNLIAO_INFO_RQ || * ptype==_DEF_PROTOCOL_JOIN_QUNLIAO_MSG)  //Ⱥ�� ��m_sockSend
	{

		if(sendto(m_sockSend,( char *)szbuf,nlen,0,(sockaddr *)&addr,sizeof(addr))<=0)
		{
			TRACE("%d\n",WSAGetLastError());
	         return false;
	    }
	}
	else
	{
		
		if((len=sendto(m_socketClient,( char *)szbuf,nlen,0,(sockaddr *)&addr,sizeof(addr)))<=0)
		{
			TRACE("%d\n",WSAGetLastError());
	         return false;
	    }
	}	
	return true;
}

bool CTCPClient::SendDataTCP(SOCKET sock,char* szbuf,int nlen)
{
		if(!szbuf || nlen <=0)
		return false;

		//���Ͱ���С
		if(send(m_TCPsockClient,(const char*)&nlen,sizeof(int),0)<0)
		return false;

		//������
		if(send(m_TCPsockClient,szbuf,nlen,0)<0)
			return false;
	
		return true;
}

void CTCPClient::UnInitNetWork()
{
	bFlagQuit=false;
	WSACleanup();
	if(m_hThreadRecv )
	{
		if(WAIT_TIMEOUT == WaitForSingleObject(m_hThreadRecv,100))
			TerminateThread(m_hThreadRecv,-1);
		
		CloseHandle(m_hThreadRecv);
		m_hThreadRecv = NULL;
	
	}
	 if(m_socketClient)
	 {
		 closesocket(m_socketClient);
		 m_socketClient=INVALID_SOCKET;
		 m_socketClient = NULL;
	 }
	  if(m_TCPsockClient)
	 {
		 shutdown(m_TCPsockClient,SD_BOTH);
		 closesocket(m_TCPsockClient);
		 m_TCPsockClient = NULL;
	 }
	 if(m_hThread)
	 {
		  if( WaitForSingleObject(m_hThread,100) == WAIT_TIMEOUT)
		 {
			 TerminateThread(m_hThread,-1);
		 }
		 CloseHandle(m_hThread);
		 m_hThread=NULL;
	 }

}