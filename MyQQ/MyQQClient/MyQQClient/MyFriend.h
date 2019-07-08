#pragma once
#include"Packdef.h"
#include"resource.h"
#include "afxwin.h"
#include"MyDlgChat.h"
#include<list>
#include<map>
#include"SendMsg.h"

using namespace std;

 #define WM_NC WM_USER+15

// CMyFriend 对话框

class CMyFriend : public CDialogEx
{
	DECLARE_DYNAMIC(CMyFriend)

public:
	CMyFriend(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyFriend();

// 对话框数据
	enum { IDD =IDD_DIALOG1};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void InitListCtr(void);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();	
	afx_msg void OnLbnDblclkList1(); 
	afx_msg void OnBnClickedHead();
	afx_msg void OnBnClickedBtnMin();
	afx_msg void OnBnClickedBtnClose();
	CMyDlgChat* GetDlg(CString strIP);
	SOCKET GetSockCli(void);
private: // 托盘实现
	NOTIFYICONDATA NotifyIcon;
	SOCKET m_sockCli;
	HICON m_tHIcon;
public:
	map<CString ,CMyDlgChat*> m_mapIpToDlg;
    CListBox m_lstMyFriend;
	CWnd * m_pWnd;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButton1();
	afx_msg LRESULT 	OnOffLineMsg(WPARAM,LPARAM);
};
