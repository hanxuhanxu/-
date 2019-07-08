#pragma once
#include "afxwin.h"
#include"MyFriend.h"
#include"MyQQClientDlg.h"
// CChatFriendDlg �Ի���

class CChatFriendDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChatFriendDlg)

public:
	CChatFriendDlg(CWnd* pParent=NULL);   // ��׼���캯��
	virtual ~CChatFriendDlg();

// �Ի�������
	enum { IDD = IDD_CHAT_FRD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	
	virtual BOOL OnInitDialog();	
	afx_msg void OnBnClickedButton1();
	afx_msg LRESULT 	OnJoinMsg(WPARAM,LPARAM);  //�Է��������룬��������Ա�б�
	afx_msg LRESULT 	OnJoinHisMsg(WPARAM,LPARAM);  //�ӵ�֪ͨ���˼��룬��������Ա�б�
	
public:
    CListBox m_lstChatFriend;
	CMyFriend * m_pFriend;
	CMyQQClientDlg * m_pDlg;
	list<CString>m_lstYiYouFriend;  //Ⱥ���г�Ա�б�
};
