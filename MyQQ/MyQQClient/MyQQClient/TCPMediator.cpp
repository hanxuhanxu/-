#include "stdafx.h"
#include "TCPMediator.h"
#include"TCPClient.h"
#include "afxdialogex.h"
#include"ChatFriendDlg.h"
#include"MyFriend.h"


CTCPMediator::CTCPMediator(CWnd * pWnd)
{
	m_pTCPNetClient=new CTCPClient(this);
	m_pWnd=pWnd;
	//m_pCQunLiaoDlg=new CChatFriendDlg;
}


CTCPMediator::~CTCPMediator(void)
{
	if(m_pTCPNetClient)
	{
		delete(m_pTCPNetClient);
		m_pTCPNetClient=NULL;
	}
}

static const ProtocolMap m_ProtocolMapEntries[] =
{
	//{_DEF_PROTOCOL_ONLINE_RQ,&CTCPMediator::DealOnLineRq},
	{_DEF_PROTOCOL_ONLINE_RS,&CTCPMediator::DealOnLineRs},
	{_DEF_PROTOCOL_OFFLINE_RQ,&CTCPMediator::DealOffLineRq},
	{_DEF_PROTOCOL_DATAINFO_RS,&CTCPMediator::DealDataInfoRs},
	{_DEF_PROTOCOL_REGISTER_RS,&CTCPMediator::DealRegisterInfoRs},
	{_DEF_PROTOCOL_QUNLIAO_RQ,&CTCPMediator::DealQunliaoRq},
	{_DEF_PROTOCOL_QUNLIAO_RS,&CTCPMediator::DealQunliaoRs},
	{_DEF_PROTOCOL_QUNLIAO_INFO_RQ,&CTCPMediator::DealDataInfoRs},
	{_DEF_PROTOCOL_JOIN_QUNLIAO_MSG,&CTCPMediator::DealJoinMsg},
	{_DEF_PROTOCOL_KEEP_CONNECT_RQ,&CTCPMediator::DealKeepConnectRq},
	{_DEF_PROTOCOL_KEEP_CONNECT_RS,&CTCPMediator::DealKeepConnectRs},
	//{_DEF_PROTOCOL_FILESTREAM_RS,&CTCPMediator::DealFileInfoRs},
	{0,0}

};

bool CTCPMediator::open()
{
	if( ! m_pTCPNetClient->InitNetWork())
	{
		TRACE("%d\n",GetLastError());
		return false;
	}
	return true;
}
void CTCPMediator::close()
{
	m_pTCPNetClient->UnInitNetWork();
}

bool CTCPMediator::SendData(long lSendIP, char * szbuf ,int nlen)
{
	in_addr ia;
	ia.S_un.S_addr=lSendIP;
	int * ptype=(int *)szbuf;
		
	switch ( * ptype)
	{
	case _DEF_PROTOCOL_ONLINE_RS:
		{
			STRU_DATAINFO* sd=(STRU_DATAINFO*)szbuf;
			if( !m_pTCPNetClient->SendData(lSendIP,szbuf,nlen))
			return false;
			break;
		}

	case _DEF_PROTOCOL_KEEP_CONNECT_RQ: //请求保持连接
		{
			STRU_KEEP_CONNECT * sk=(STRU_KEEP_CONNECT*)szbuf;
			if( !m_pTCPNetClient->SendData(lSendIP,szbuf,nlen))
			return false;
			break;
		}

	case _DEF_PROTOCOL_FILESTREAM_RQ:
		{
			STRU_FILE * sf=(STRU_FILE*)szbuf;
			if( !m_pTCPNetClient->SendDataTCP(lSendIP,szbuf,nlen))
			return false;
		}
		break;
	case _DEF_PROTOCOL_DATAINFO_RQ:
		{
			STRU_DATAINFO* sd=(STRU_DATAINFO*)szbuf;
			if( !m_pTCPNetClient->SendData(lSendIP,szbuf,nlen))
			return false;
		}
		break;
	case _DEF_PROTOCOL_QUNLIAO_INFO_RQ:
		{
			STRU_DATAINFO* sd=(STRU_DATAINFO*)szbuf;
			if( !m_pTCPNetClient->SendData(lSendIP,szbuf,nlen))
			return false;
		}
		break;
	case _DEF_PROTOCOL_OFFLINE_RQ:
		{
			STRU_OFFLINE* sd=(STRU_OFFLINE*)szbuf;
			if( !m_pTCPNetClient->SendData(lSendIP,szbuf,nlen))
			return false;
		}
		break;
	case _DEF_PROTOCOL_QUNLIAO_RQ:
		{
			STRU_QUNLIAO_RQ * sd=(STRU_QUNLIAO_RQ*)szbuf;
			if( !m_pTCPNetClient->SendData(lSendIP,szbuf,nlen))
			return false;
		}
		break;

	default:
			STRU_ONLINE *so=(STRU_ONLINE *)szbuf;
			if( !m_pTCPNetClient->SendData(lSendIP,szbuf,nlen))
			return false;
		break;
	}
	return true;
}

bool CTCPMediator::SendDataTCP(SOCKET sock, char * szbuf ,int nlen)
{
	int * ptype=(int *)szbuf;
		
	switch ( * ptype)
	{
	case _DEF_PROTOCOL_FILESTREAM_RQ:
		{
			STRU_FILE * sf=(STRU_FILE*)szbuf;
			if( !m_pTCPNetClient->SendDataTCP(sock,szbuf,nlen))
			return false;
		}
		break;
	case _DEF_PROTOCOL_FILESTREAMINFO_RQ:
		{
			STRU_FILE * sf=(STRU_FILE*)szbuf;
			if( !m_pTCPNetClient->SendDataTCP(sock,szbuf,nlen))
			return false;
		}
		break;
	case _DEF_PROTOCOL_PICTURE_RQ:
		{
			STRU_PIC_RQ * sp=(STRU_PIC_RQ*)szbuf;
			if( !m_pTCPNetClient->SendDataTCP(sock,szbuf,nlen))
			return false;
		}
		break;
	default:
		break;
	}
	return true;
}

void CTCPMediator::DealData(char * szbuf , long lRecvIP) 
{
	//拿到包的前四个字节代表包类型
	int * ptype=(int *)szbuf;
	int nn=sizeof(szbuf);
	//处理数据 遍历消息映射表
	int i = 0;
	while(1)
	{
		if(m_ProtocolMapEntries[i].m_nType == *ptype)
		{
			char * mm=(char *)m_ProtocolMapEntries[i].m_nType;
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
			case _DEF_PROTOCOL_FILESTREAM_RS:
				{
					 STRU_FILE_RS * sf=( STRU_FILE_RS *)szbuf;
                     DealFileInfoRs(szbuf,sock);
				}
					 break;
			case _DEF_PROTOCOL_FILESTREAMINFO_RS:
				{
					 STRU_FILE_RS * sf=( STRU_FILE_RS *)szbuf;
                     DealFileInfo(szbuf,sock);
				}
					 break;

			case _DEF_PROTOCOL_PICTURE_RS:
				{
				      STRU_PIC_RQ * sprq=(STRU_PIC_RQ*) szbuf;
					  DealPicRs(szbuf,sock);
				}
				break;
			default:
				break;
			}
	//STRU_FILE_RS*sfrs=(STRU_FILE_RS*)szbuf;
	

}

void CTCPMediator::DealRegisterInfoRs(char * szbuf , long lRecvIP) 
{
	//STRU_REGISTER_RS
	STRU_REGISTER_RS* sr=(STRU_REGISTER_RS*)szbuf;
	if(sr->IfSuccess==TRUE)
	{
		MessageBox((HWND)m_pWnd->m_hWnd,_T("注册成功"),_T("提示"),NULL);
		m_pWnd->PostMessage(UM_ONLINEMSG,0,lRecvIP);

	}
	else if(sr->IfSuccess==FALSE)
	{
		MessageBox((HWND)m_pWnd->m_hWnd,_T("用户名已存在"),_T("提示"),NULL);
		return;
	}
}

void CTCPMediator::DealOnLineRs(char * szbuf , long lRecvIP)
{
	STRU_ONLINERS* so=(STRU_ONLINERS*)szbuf;
	if(so->IfSuccess==TRUE)
	{
		m_pWnd->PostMessage(UM_ONLINEMSG,(WPARAM)so,lRecvIP);

	}
	else if(so->IfSuccess==FALSE)
	{
		MessageBox((HWND)m_pWnd->m_hWnd,_T("用户名或密码错误"),_T("提示"),NULL);
		return;
	}
}

void CTCPMediator::DealQunliaoRq(char * szbuf , long lRecvIP)
{
	STRU_QUNLIAO_RQ * sq=(STRU_QUNLIAO_RQ * )szbuf;
	if(MessageBox((HWND)m_pWnd->m_hWnd,_T("是否加入群聊"),_T("提示"),MB_YESNO)==IDYES)
	{
		//回应邀请者
		STRU_QUNLIAO_RS sqrs;
		sqrs.m_nType=_DEF_PROTOCOL_QUNLIAO_RS;
		sqrs.IsOK=true;
		strcpy_s((char*)sqrs.m_ToIP,(size_t)sizeof(sqrs.m_ToIP),(char*)sq->m_MyIP);
		strcpy_s((char*)sqrs.m_MyIP,(size_t)sizeof(sqrs.m_MyIP),(char*)_MY_IP_STR);
		m_pTCPNetClient->SendData(_HOST_IP,(char*)&sqrs,sizeof(sqrs));

		m_pWnd->PostMessage(UM_INVITEMSG,(WPARAM)sq,lRecvIP);
	}
	else
		return;

}

void CTCPMediator::DealQunliaoRs(char * szbuf , long lRecvIP)
{
	STRU_QUNLIAO_RS*sd=(STRU_QUNLIAO_RS*)szbuf;
	if(sd->IsOK==true)
	{
		//向所有群聊中的好友发送消息：添加新朋友
		STRU_JOIN_QUNLIAO sj;
		sj.m_nType=_DEF_PROTOCOL_JOIN_QUNLIAO_MSG;
		strcpy_s((char*)sj.m_HisIP,(size_t)sizeof(sj.m_HisIP),(char*)sd->m_MyIP);
		strcpy_s((char*)sj.m_MyIP,(size_t)sizeof(sj.m_MyIP),(char*)_MY_IP_STR);
		m_pTCPNetClient->SendData(_HOST_IP,(char*)&sj,sizeof(sj));

		HWND  pwnd=FindWindow(NULL,_T("添加好友"));
		CChatFriendDlg* pWnd= (CChatFriendDlg*)CChatFriendDlg::FromHandle(pwnd);
		//把对方ip加入到聊天室成员列表
		pWnd->SendMessage(UM_JOINMSG,(WPARAM)sd,lRecvIP);
	}
	else
	{
		MessageBox((HWND)m_pWnd->m_hWnd,_T("对方拒绝加入群聊"),_T("提示"),MB_OK);
	}
}

void CTCPMediator::DealJoinMsg(char * szbuf , long lRecvIP)
{
	STRU_JOIN_QUNLIAO* sj=(STRU_JOIN_QUNLIAO*)szbuf;
	HWND  pwnd=FindWindow(NULL,_T("添加好友"));
	CChatFriendDlg* pWnd= (CChatFriendDlg*)CChatFriendDlg::FromHandle(pwnd);
	//把对方ip加入到聊天室成员列表
	pWnd->SendMessage(UM_JOIN_HIS_MSG,(WPARAM)sj,lRecvIP);
}

void CTCPMediator::DealKeepConnectRq(char * szbuf , long lRecvIP)
{
	//收到保持连接请求 回复他
	STRU_KEEP_CONNECT*sk=(STRU_KEEP_CONNECT*)szbuf;
	STRU_KEEP_CONNECT skrs;
	skrs.m_nType=_DEF_PROTOCOL_KEEP_CONNECT_RS;
	strcpy_s((char*)&skrs.m_MyIP,(size_t)sizeof(skrs.m_MyIP),(char*)_MY_IP_STR);
	strcpy_s((char*)&skrs.m_HisIP,(size_t)sizeof(skrs.m_HisIP),(char*)sk->m_MyIP);
	m_pTCPNetClient->SendData(_HOST_IP,(char*)&skrs,sizeof(skrs));
}

void CTCPMediator::DealKeepConnectRs(char * szbuf , long lRecvIP)
{
	//收到保持连接回复 将其从临时好友列表中删除
	STRU_KEEP_CONNECT*sk=(STRU_KEEP_CONNECT*)szbuf;
   m_pWnd->PostMessage(UM_ONKEEPCONNECTMSG,(WPARAM)sk,lRecvIP);

}

void CTCPMediator::DealOffLineRq(char * szbuf , long lRecvIP)
{
	//遍历列表 删除该好友
	STRU_OFFLINE * so=(STRU_OFFLINE *)szbuf;
	HWND  pwnd=FindWindow(NULL,_T("好友列表"));

   CMyFriend* pWnd= (CMyFriend*)CMyFriend::FromHandle(pwnd);

   pWnd->SendMessage(UM_OFFLINEMSG,(WPARAM)so,lRecvIP);

}

void CTCPMediator::DealDataInfoRs(char * szbuf , long lRecvIP) 
{
	STRU_DATAINFO_RS*sd=(STRU_DATAINFO_RS*)szbuf;
	
	m_pWnd->PostMessage(UM_DATAINFOMSG,(WPARAM)sd,inet_addr(sd->m_SourceIP));
}

void CTCPMediator::DealFileInfoRs(char * szbuf ,long lRecvIP) 
{
	STRU_FILE_RS * sfrs=(STRU_FILE_RS* )szbuf;
	m_pWnd->SendMessage(UM_FILEINFOMSG,(WPARAM)sfrs,lRecvIP);

}

void CTCPMediator::DealFileInfo(char * szbuf ,long lRecvIP) 
{
	STRU_FILE_RS * sfrs=(STRU_FILE_RS* )szbuf;
	m_pWnd->SendMessage(UM_FILEINFO,(WPARAM)sfrs,lRecvIP);
}

void CTCPMediator::DealPicRs(char * szbuf ,long lRecvIP) 
{
	 STRU_PIC_RQ * sprq=(STRU_PIC_RQ*) szbuf;
	 m_pWnd->SendMessage(UM_PICINFOMSG,(WPARAM)sprq,lRecvIP);
}