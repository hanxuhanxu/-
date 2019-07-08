#include "stdafx.h"
#include "TCPMediator.h"
#include"TCPServer.h"
#include"MyQQServerDlg.h"

CTCPMediator::CTCPMediator(CWnd * pWnd)
{
	m_pTCPNet=new CTCPServer(this);
	m_pMySql=new CMySql();
	m_pWnd=pWnd;
}


CTCPMediator::~CTCPMediator(void)
{
	if(m_pTCPNet)
	{
		delete(m_pTCPNet);
		m_pTCPNet=NULL;
	}

	if(m_pMySql)
	{
		delete(m_pMySql);
		m_pMySql=NULL;
	}

}

static const ProtocolMap m_ProtocolMapEntries[] =
{
	{_DEF_PROTOCOL_ONLINE_RQ,&CTCPMediator::DealOnLineRq},
	//{_DEF_PROTOCOL_ONLINE_RS,&UDPMediator::DealOnLineRs},
	{_DEF_PROTOCOL_OFFLINE_RQ,&CTCPMediator::DealOffLineRq},
	{_DEF_PROTOCOL_DATAINFO_RQ,&CTCPMediator::DealDataInfo},
	{_DEF_PROTOCOL_REGISTER_RQ,&CTCPMediator::DealRegisterInfo},
	{_DEF_PROTOCOL_QUNLIAO_RQ,&CTCPMediator::DealQunliaoRq},
	{_DEF_PROTOCOL_QUNLIAO_RS,&CTCPMediator::DealQunliaoRs},
	{_DEF_PROTOCOL_QUNLIAO_INFO_RQ,&CTCPMediator::DealDataInfo},
	{_DEF_PROTOCOL_JOIN_QUNLIAO_MSG,&CTCPMediator::DealJoinMsg},
	{_DEF_PROTOCOL_KEEP_CONNECT_RQ,&CTCPMediator::DealKeepConnectRq},
	{_DEF_PROTOCOL_KEEP_CONNECT_RS,&CTCPMediator::DealKeepConnectRq},
	//{_DEF_PROTOCOL_FILESTREAM_RQ,&CTCPMediator::FileStreamRq},
	{0,0}

};

bool CTCPMediator::open()
{
	if( ! m_pTCPNet->InitNetWork())
		return false;

	return true;
}

bool CTCPMediator::SendData(long lSendIP, char * szbuf ,int nlen)
{
	char* sz=szbuf;
	if( !m_pTCPNet->SendData(lSendIP,szbuf,nlen))
		return false;

	return true;
}

bool CTCPMediator::SendDataTCP(SOCKET sock, char * szbuf ,int nlen)
{
	char* sz=szbuf;
	if( !m_pTCPNet->SendDataTCP(sock,szbuf,nlen))
		return false;
	return true;
}

void CTCPMediator::DealData(char * szbuf , long lRecvIP) 
{
	//拿到包的前四个字节代表包类型
	int * ptype=(int *)szbuf;

	//处理数据 遍历消息映射表
	int i = 0;
	while(1)
	{
		if(m_ProtocolMapEntries[i].m_nType == *ptype)
		{
			(this->*m_ProtocolMapEntries[i].m_pfun)(szbuf,lRecvIP);
			return ;
		}
		else if(m_ProtocolMapEntries[i].m_nType ==0 &&m_ProtocolMapEntries[i].m_pfun ==0 )
			return;

		i++;
	}
}

void CTCPMediator::DealDataTCP(char * szbuf ,SOCKET sock) 
{
	int * ptype=(int *)szbuf;
	switch (* ptype)
	{
	case _DEF_PROTOCOL_FILESTREAM_RQ:
		{ 
			FileStreamRq(szbuf,sock);
		}
			 break;
	case _DEF_PROTOCOL_FILESTREAMINFO_RQ:
		{ 
			FileStreamInfoRq(szbuf,sock);
		}
			 break;

	case _DEF_PROTOCOL_PICTURE_RQ:
		{PictureRq(szbuf,sock);}
		break;
	default:
		break;
	}
	
}

void CTCPMediator::close()
{
	m_pTCPNet->UnInitNetWork();
	
}

void CTCPMediator::DealOnLineRq(char * szbuf , long lRecvIP)
{

	STRU_ONLINE* so=( STRU_ONLINE*)szbuf;
	char szsql[_DEF_SQLLEN] = {0};
	list<string> lststr;
	sprintf_s(szsql,"SELECT userId FROM tb_user where username = '%s' and userpassword = '%s';" ,so->m_username,so->m_password);
	m_pMySql->SelectMySql(szsql,1,lststr);

	if(lststr.size() ==0)
	{
		//用户名或密码错误  登录失败
		STRU_ONLINERS sors;
		sors.m_nType=_DEF_PROTOCOL_ONLINE_RS;
		sors.IfSuccess=FALSE;
		m_pTCPNet->SendData(lRecvIP,(char*)&sors,sizeof(sors));
	}
	else if(lststr.size() >0)
	{
		//登录成功
		STRU_ONLINERS sors;
		sors.m_nType=_DEF_PROTOCOL_ONLINE_RS;
		sors.IfSuccess=TRUE;
		strcpy_s((char*)&sors.m_username,(size_t)sizeof(sors.m_username),(char*)&so->m_username);
		//sors.m_username=so->m_username;
		m_pTCPNet->SendData(lRecvIP,(char*)&sors,sizeof(sors));
		/*CWnd * pwnd=AfxGetMainWnd();*/
	//	::SendMessage(,UM_ONLINE_ADD,(WPARAM)&so->m_username,0);
		STRU_ONLINE* soadd=( STRU_ONLINE*)szbuf;
		m_pWnd->SendMessage(UM_ONLINE_ADD,(WPARAM)soadd,0);
	}
}

void  CTCPMediator::DealQunliaoRq(char * szbuf , long lRecvIP)
{
	STRU_QUNLIAO_RQ * sq=(STRU_QUNLIAO_RQ * )szbuf;
	STRU_QUNLIAO_RQ sqrq;
	sqrq.m_nType=_DEF_PROTOCOL_QUNLIAO_RQ;
	strcpy_s((char*)&sqrq.m_ToIP,(size_t)sizeof(sqrq.m_ToIP),(char*)&sq->m_ToIP);
	strcpy_s((char*)&sqrq.m_MyIP,(size_t)sizeof(sqrq.m_MyIP),(char*)&sq->m_MyIP);
	//auto ite=sq->m_lstFriends.begin();
	//while(ite!=sq->m_lstFriends.begin())
	//{
	//	sqrq.m_lstFriends.push_back(*ite);
	//}
	strcpy_s((char*)&sqrq.m_HadFriends,(size_t)sizeof(sqrq.m_HadFriends),(char*)&sq->m_HadFriends);
	m_pTCPNet->SendData(lRecvIP,(char*)&sqrq,sizeof(sqrq));

}

void CTCPMediator::DealQunliaoRs(char * szbuf , long lRecvIP)
{
	STRU_QUNLIAO_RS * sq=(STRU_QUNLIAO_RS * )szbuf;
	STRU_QUNLIAO_RS sqrs;
	sqrs.m_nType=_DEF_PROTOCOL_QUNLIAO_RS;
	sqrs.IsOK=true;
	strcpy_s((char*)sqrs.m_ToIP,(size_t)sizeof(sqrs.m_ToIP),(char*)sq->m_ToIP);
	strcpy_s((char*)sqrs.m_MyIP,(size_t)sizeof(sqrs.m_MyIP),(char*)sq->m_MyIP);
	m_pTCPNet->SendData(lRecvIP,(char*)&sqrs,sizeof(sqrs));
}

void CTCPMediator::DealJoinMsg(char * szbuf , long lRecvIP)
{
	STRU_JOIN_QUNLIAO* sj=(STRU_JOIN_QUNLIAO*)szbuf;
	STRU_JOIN_QUNLIAO sjq;
	sjq.m_nType=_DEF_PROTOCOL_JOIN_QUNLIAO_MSG;
	strcpy_s((char*)sjq.m_HisIP,(size_t)sizeof(sjq.m_HisIP),(char*)sj->m_HisIP);
	strcpy_s((char*)sjq.m_MyIP,(size_t)sizeof(sjq.m_MyIP),(char*)sj->m_MyIP);
	m_pTCPNet->SendData(lRecvIP,(char*)&sjq,sizeof(sjq));
}

void CTCPMediator::DealOffLineRq(char * szbuf , long lRecvIP)
{
	STRU_OFFLINE * so=(STRU_OFFLINE * )szbuf;
	STRU_OFFLINE sors;
	sors.m_nType=_DEF_PROTOCOL_OFFLINE_RQ;
	strcpy_s((char*)&sors.m_MyIP,(size_t)sizeof(sors.m_MyIP),(char*)&so->m_MyIP);
	strcpy_s((char*)&sors.m_username,(size_t)sizeof(sors.m_username),(char*)&so->m_username);
	m_pTCPNet->SendData(lRecvIP,(char*)&sors,sizeof(sors));
		

	m_pWnd->SendMessage(UM_OFFLINE_ADD,(WPARAM)so,0);
}

void CTCPMediator::DealKeepConnectRq(char * szbuf , long lRecvIP) //请求保持连接
{
	STRU_KEEP_CONNECT * sk=(STRU_KEEP_CONNECT*)szbuf;
	STRU_KEEP_CONNECT skrq;
	skrq.m_nType=_DEF_PROTOCOL_KEEP_CONNECT_RQ;
	strcpy_s((char*)&skrq.m_MyIP,(size_t)sizeof(skrq.m_MyIP),(char*)sk->m_MyIP);
	strcpy_s((char*)&skrq.m_HisIP,(size_t)sizeof(skrq.m_HisIP),(char*)sk->m_HisIP);

	m_pTCPNet->SendData(lRecvIP,(char*)&skrq,sizeof(skrq));
}

void CTCPMediator::DealRegisterInfo(char * szbuf , long lRecvIP)
{
	//注册
	STRU_ONLINE* so=( STRU_ONLINE*)szbuf;
	char szsql[_DEF_SQLLEN] = {0};
	list<string> lststr;
	sprintf_s(szsql,"SELECT userId FROM tb_user where username = '%s' and userpassword = '%s';" ,so->m_username,so->m_password);
	m_pMySql->SelectMySql(szsql,1,lststr);

	if(lststr.size() ==0)
	{
			 //将此人插入到数据库中
			sprintf_s(szsql,"insert into tb_user(username,userpassword) values('%s','%s');",so->m_username,so->m_password);
			m_pMySql->UpdateMySql(szsql);
			//注册成功
	        STRU_REGISTER_RS sgrs;
			sgrs.m_nType=_DEF_PROTOCOL_REGISTER_RS;
			sgrs.IfSuccess=TRUE;
			m_pTCPNet->SendData(lRecvIP,(char*)&sgrs,sizeof(sgrs));
	}
	else if(lststr.size() >0)
	{
		    STRU_REGISTER_RS sgrs;
			sgrs.m_nType=_DEF_PROTOCOL_REGISTER_RS;
			sgrs.IfSuccess=FALSE;
			m_pTCPNet->SendData(lRecvIP,(char*)&sgrs,sizeof(sgrs));
	}
}

void CTCPMediator::DealDataInfo(char * szbuf , long lRecvIP)
{
	//接到聊天数据包

	in_addr ia;
	ia.S_un.S_addr=lRecvIP;
	CString ip=inet_ntoa(ia);

	STRU_DATAINFO*sd=(STRU_DATAINFO*)szbuf;
	STRU_DATAINFO_RS sdr;
	if(sd->m_nType==_DEF_PROTOCOL_QUNLIAO_INFO_RQ)
	{
		sdr.m_nType=_DEF_PROTOCOL_QUNLIAO_INFO_RQ;
	}
	else
	{
		sdr.m_nType=_DEF_PROTOCOL_DATAINFO_RS;
	}
	strcpy_s((char*)&sdr.m_szBuf,(size_t)sizeof(sdr.m_szBuf),(char*)&sd->m_szBuf);
	strcpy_s((char*)&sdr.m_SourceIP,(size_t)sizeof(sdr.m_SourceIP),ip);

	m_pTCPNet->SendData(  inet_addr(sd->m_ToIP),(char*)&sdr,sizeof(sdr));
	ip="";
	ip.Empty();
}

void  CTCPMediator::FileStreamRq(char * szbuf , SOCKET sock)
{
	//接收文件包
	STRU_FILE *sf=(STRU_FILE *)szbuf;
    STRU_FILE_RS sfrs;
	sfrs.m_nType=_DEF_PROTOCOL_FILESTREAM_RS;
	sfrs.m_FileSize=sf->m_FileSize;
	sfrs.m_TranSize=sf->m_TranSize;
	sfrs.offset=sf->offset;
	//int offset=0;
	strcpy_s((char*)&sfrs.m_SourceIP,(size_t)sizeof(sfrs.m_SourceIP),(char*)&sf->m_ToIP);
	strcpy_s((char*)&sfrs.m_FileName,(size_t)sizeof(sfrs.m_FileName),(char*)&sf->m_FileName);
	//while(1)
	//{
		memcpy_s(sfrs.mFileContent,sizeof(sfrs.mFileContent),sf->mFileContent,sf->m_TranSize);
		m_pTCPNet->SendDataTCP(sock,(char*)&sfrs,sizeof(sfrs));
	/*}*/
}

void CTCPMediator::FileStreamInfoRq(char * szbuf , SOCKET sock)
{
	STRU_FILE *sf=(STRU_FILE *)szbuf;
    STRU_FILE_RS sfrs;
	sfrs.m_nType=_DEF_PROTOCOL_FILESTREAMINFO_RS;
	sfrs.m_FileSize=sf->m_FileSize;
	strcpy_s((char*)&sfrs.m_SourceIP,(size_t)sizeof(sfrs.m_SourceIP),(char*)&sf->m_ToIP);
	strcpy_s((char*)&sfrs.m_FileName,(size_t)sizeof(sfrs.m_FileName),(char*)&sf->m_FileName);
	m_pTCPNet->SendDataTCP(sock,(char*)&sfrs,sizeof(sfrs));
}

void  CTCPMediator::PictureRq(char * szbuf , SOCKET sock)
{
	STRU_PIC_RQ *sp=(STRU_PIC_RQ *)szbuf;
	STRU_PIC_RQ spr;
	spr.m_nType=_DEF_PROTOCOL_PICTURE_RS;
	spr.m_FileSize=sp->m_FileSize;
	strcpy_s((char*)&spr.m_SourceIP,(size_t)sizeof(spr.m_SourceIP),(char*)&sp->m_SourceIP);
	strcpy_s((char*)&spr.mFilePath,(size_t)sizeof(spr.mFilePath),(char*)&sp->mFilePath);
	memcpy_s(spr.mFileContent,sizeof(spr.mFileContent),sp->mFileContent,sp->m_FileSize);

	m_pTCPNet->SendDataTCP(sock,(char*)&spr,sizeof(spr));
}

