// ChatFriendDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyQQClient.h"
#include "ChatFriendDlg.h"
#include "afxdialogex.h"


// CChatFriendDlg 对话框

IMPLEMENT_DYNAMIC(CChatFriendDlg, CDialogEx)

CChatFriendDlg::CChatFriendDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatFriendDlg::IDD, pParent)
{
	m_pDlg=new CMyQQClientDlg;
}

CChatFriendDlg::~CChatFriendDlg()
{
}

void CChatFriendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTCHAT, m_lstChatFriend);
}


BEGIN_MESSAGE_MAP(CChatFriendDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CChatFriendDlg::OnBnClickedButton1)
	ON_MESSAGE(UM_JOINMSG,&CChatFriendDlg::OnJoinMsg)
	ON_MESSAGE(UM_JOIN_HIS_MSG,&CChatFriendDlg::OnJoinHisMsg)
END_MESSAGE_MAP()


// CChatFriendDlg 消息处理程序


BOOL CChatFriendDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString str;
	 CMyQQClientDlg* pwnd  =(CMyQQClientDlg* ) AfxGetMainWnd();
	 pwnd->m_lstFriend;
	
	 auto ite=(pwnd-> m_lstFriend).begin();
	while(ite!=(pwnd->  m_lstFriend).end())
	{
		str=*ite;
		m_lstChatFriend.AddString(str);
		m_lstYiYouFriend.push_back(str);
		++ite;
	}
	str="";
	str.Empty();
	return TRUE;  // return TRUE unless you set the focus to a control
}


void CChatFriendDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex=0;
	CString str;
	nIndex=m_lstChatFriend.GetCurSel();
	m_lstChatFriend.GetText(nIndex,str);
	STRU_QUNLIAO_RQ sq;
	sq.m_nType=_DEF_PROTOCOL_QUNLIAO_RQ;
	WideCharToMultiByte(CP_ACP,0,str,-1,sq.m_ToIP,sizeof(sq.m_ToIP),0,0);
    
	strcpy_s((char*)sq.m_MyIP,(size_t)sizeof(sq.m_MyIP),(char*)_MY_IP_STR);
	auto ite=m_lstYiYouFriend.begin();
	while( ite!=m_lstYiYouFriend.end())
	{
	    //将链表中的人员ip存到char【】中
		WideCharToMultiByte(CP_ACP,0,(*ite),-1,sq.m_HadFriends,sizeof(sq.m_HadFriends),0,0);	
		++ite;
	}
	((CMyQQClientDlg*)GetParent())->GetMediator()->SendData(_HOST_IP,((char *)&sq),sizeof(sq));
}

LRESULT 	CChatFriendDlg::OnJoinMsg(WPARAM wparam,LPARAM lparam)
{
	STRU_QUNLIAO_RS * sq=(STRU_QUNLIAO_RS*)wparam;
	m_lstYiYouFriend.push_back(sq->m_MyIP);
	return 0;
}

LRESULT 	CChatFriendDlg::OnJoinHisMsg(WPARAM wparam,LPARAM lparam)
{
	 STRU_JOIN_QUNLIAO* sj=(STRU_JOIN_QUNLIAO*)wparam;
	 m_lstYiYouFriend.push_back(sj->m_HisIP);
	return 0;
}