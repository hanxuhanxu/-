#pragma once
#include"INet.h"
#include"IMediator.h"

class CTCPClient:public INet
{
public:
	CTCPClient(IMediator* pMediator);
	~CTCPClient(void);
public:
	virtual bool InitNetWork();
	virtual void UnInitNetWork();
	virtual bool SendData(long lSendIP, char *szbuf ,int nlen);
	virtual bool SendDataTCP(SOCKET sock,char* szbuf,int nlen);
	bool SelectSocket();
public:
	static  unsigned  __stdcall ThreadProc( void * );
	static  unsigned  __stdcall ThreadRecv( void * );
public:
	SOCKET m_socketClient;  //udp
	SOCKET m_TCPsockClient;  //tcp
	SOCKET m_sockSend; //ÈºÁÄ¹ã²¥µÄudp socket
	IMediator*m_pMediator;
	HANDLE m_hThread;
	HANDLE m_hThreadRecv;
};

