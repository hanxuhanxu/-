#pragma once
#include"IMediator.h"
//#include"ChatFriendDlg.h"

class CTCPMediator;
typedef void (CTCPMediator::*PFUN) (char *  , long);

struct ProtocolMap
{
	PackdefType m_nType;
	PFUN        m_pfun;
};

class CTCPMediator:public IMediator
{
public:
	CTCPMediator(CWnd * pWnd);
	virtual~CTCPMediator(void);
public:
	bool open();
	void close();
	bool SendData(long lSendIP, char * szbuf ,int nlen);
	bool SendDataTCP(SOCKET sock, char * szbuf ,int nlen);
	void DealData(char * szbuf , long lRecvIP) ;
	void DealDataTCP(char * szbuf ,SOCKET sock) ;
	void DealDataInfoRs(char * szbuf , long lRecvIP) ;
	void DealRegisterInfoRs(char * szbuf , long lRecvIP) ;
	void DealFileInfoRs(char * szbuf ,long lRecvIP) ;
	void DealFileInfo(char * szbuf ,long lRecvIP) ; //只接收大小 
	void DealPicRs(char * szbuf ,long lRecvIP) ;
public:
	INet * m_pTCPNetClient;
	INet * m_pTCPNet;
	PFUN m_protocolMap[_MAX_PROTOCOL_LEN];
	CWnd * m_pWnd;
	//CChatFriendDlg * m_pCQunLiaoDlg;
public:
	void DealOnLineRs(char * szbuf , long lRecvIP);//DealOffLineRq
	void DealOffLineRq(char * szbuf , long lRecvIP);
	void DealQunliaoRq(char * szbuf , long lRecvIP);
	void DealQunliaoRs(char * szbuf , long lRecvIP);
	void DealJoinMsg(char * szbuf , long lRecvIP);
	void DealKeepConnectRq(char * szbuf , long lRecvIP);
	void DealKeepConnectRs(char * szbuf , long lRecvIP);
};

