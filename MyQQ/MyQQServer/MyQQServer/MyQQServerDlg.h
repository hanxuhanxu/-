
// MyQQServerDlg.h : 头文件
//

#pragma once
#include"IMediator.h"
#include"MySql.h"
#include"resource.h"
#include "afxcmn.h"
#include "afxwin.h"

// CMyQQServerDlg 对话框
class CMyQQServerDlg : public CDialogEx
{
// 构造
public:
	CMyQQServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MYQQSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
private:
	IMediator* m_pTCPMediator;//中介者的指针
	CMySql * m_pMySql;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg LRESULT OnUserAdd(WPARAM,LPARAM);
	afx_msg LRESULT OnUserOffAdd(WPARAM,LPARAM);
	afx_msg HCURSOR OnQueryDragIcon();
	//afx_msg LRESULT OnLineMsg(char *  , long );
	DECLARE_MESSAGE_MAP()
public:
	IMediator* GetMediator()
	{
		return m_pTCPMediator;
	}
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnBegin();
	afx_msg void OnBnClickedBtnMin();
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBnClickedBtnEnd();
	CListCtrl m_lstUser;
	CListBox m_lstMsg;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
