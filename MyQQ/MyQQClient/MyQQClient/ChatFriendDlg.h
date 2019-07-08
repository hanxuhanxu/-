#pragma once
#include "afxwin.h"
#include"MyFriend.h"
#include"MyQQClientDlg.h"
// CChatFriendDlg 对话框

class CChatFriendDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChatFriendDlg)

public:
	CChatFriendDlg(CWnd* pParent=NULL);   // 标准构造函数
	virtual ~CChatFriendDlg();

// 对话框数据
	enum { IDD = IDD_CHAT_FRD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
	virtual BOOL OnInitDialog();	
	afx_msg void OnBnClickedButton1();
	afx_msg LRESULT 	OnJoinMsg(WPARAM,LPARAM);  //对方接受邀请，将其加入成员列表
	afx_msg LRESULT 	OnJoinHisMsg(WPARAM,LPARAM);  //接到通知有人加入，将其加入成员列表
	
public:
    CListBox m_lstChatFriend;
	CMyFriend * m_pFriend;
	CMyQQClientDlg * m_pDlg;
	list<CString>m_lstYiYouFriend;  //群聊中成员列表
};
