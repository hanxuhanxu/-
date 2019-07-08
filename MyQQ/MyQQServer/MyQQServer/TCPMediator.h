#pragma once
#include"IMediator.h"
#include"MySql.h"

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
public:
	void DealOnLineRq(char * szbuf , long lRecvIP);
	void DealOffLineRq(char * szbuf , long lRecvIP);
	void DealDataInfo(char * szbuf , long lRecvIP);
	void DealRegisterInfo(char * szbuf , long lRecvIP);
	void FileStreamRq(char * szbuf , SOCKET sock);
	void FileStreamInfoRq(char * szbuf , SOCKET sock);
	void PictureRq(char * szbuf , SOCKET sock);
	void DealQunliaoRq(char * szbuf , long lRecvIP);
	void DealQunliaoRs(char * szbuf , long lRecvIP);
	void DealJoinMsg(char * szbuf , long lRecvIP);
	void DealKeepConnectRq(char * szbuf , long lRecvIP);
public:
	INet * m_pTCPNet;
	CMySql * m_pMySql; 
	PFUN m_protocolMap[_MAX_PROTOCOL_LEN];
	CWnd * m_pWnd;
};

