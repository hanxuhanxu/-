#ifndef _TCPSERVER_H
#define _TCPSERVER_H
#pragma once
#include"INet.h"
#include"IMediator.h"
#include<list>

class CTCPServer:public INet
{
public:
	CTCPServer(IMediator* pMediator);
	virtual~CTCPServer(void);
public:
	virtual bool InitNetWork() ;
	virtual void UnInitNetWork();
	virtual bool SendData(long lSendIP, char *szbuf ,int nlen);
	virtual bool SendDataTCP(SOCKET sock, char *szbuf ,int nlen) ;
	bool SelectSocket();
public:

public:
	static  unsigned  _stdcall ThreadProc( void * );
	static  unsigned  __stdcall ThreadAccept( void * );
	static  unsigned  __stdcall ThreadRecv( void * );
private:
	SOCKET m_socketListen;
	SOCKET m_sockerTCP;
	SOCKET m_sockSend;
	//static IMediator* m_pMediator;
	std::list<HANDLE> m_lsthThreadRecv;
	HANDLE m_hThread;
	HANDLE m_hThreadTCP;
	//static bool bFlagQuit;
	WSAEVENT m_we;
};

#endif;
