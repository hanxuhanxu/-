#ifndef IMEDIATOR_H
#define IMEDIATOR_H
#include"INet.h"
#include"Packdef.h"

class IMediator
{
public:
	IMediator(){};
	virtual ~IMediator(){};
public:
	virtual bool open() = 0;
	virtual void close() = 0;
	virtual bool SendData(long lSendIP, char * szbuf ,int nlen) = 0;
	virtual bool SendDataTCP(SOCKET sock, char * szbuf ,int nlen) = 0;
	virtual void DealData(char * szbuf , long lRecvIP) = 0;
	virtual void DealDataTCP(char * szbuf ,SOCKET sock)=0 ;
};

#endif