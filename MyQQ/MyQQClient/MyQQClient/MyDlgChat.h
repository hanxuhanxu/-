#pragma once
#include"Packdef.h"
#include"resource.h"
#include "afxwin.h"
#include"afxole.h" 
#include"richole.h"
#include "afxcmn.h"

// CMyDlgChat 对话框

class CMyDlgChat : public CDialogEx
{
	DECLARE_DYNAMIC(CMyDlgChat)

public:
	CMyDlgChat(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyDlgChat();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	CString m_edtContent;
	afx_msg void OnBnClickedButton1();
	CListBox m_lstContent;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	BOOL InsertObject( LPCTSTR pszFileName);
	IOleObject* m_pPicObj;
	afx_msg void OnBnClickedButton4();
	//CRichEditCtrl m_reRecv;
	//CRichEditCtrl m_reSend;
	afx_msg void OnBnClickedButton5();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButton6();
	CProgressCtrl m_tranProgress;
};
