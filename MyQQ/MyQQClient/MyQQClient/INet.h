#ifndef _INET_H
#define _INET_H
#include <Winsock2.h>
#include"Packdef.h"
#include<process.h>


#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"oleaut32.lib")

class INet
{
public:
	INet(){};
	virtual ~INet(){};
public:
	virtual bool InitNetWork() = 0;
	virtual void UnInitNetWork() = 0;
	virtual bool SendData(long lSendIP, char *szbuf ,int nlen) = 0;
	virtual bool SendDataTCP(SOCKET sock, char *szbuf ,int nlen) = 0;
};

#endif