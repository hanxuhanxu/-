
// MyQQServerDlg.h : ͷ�ļ�
//

#pragma once
#include"IMediator.h"
#include"MySql.h"
#include"resource.h"
#include "afxcmn.h"
#include "afxwin.h"

// CMyQQServerDlg �Ի���
class CMyQQServerDlg : public CDialogEx
{
// ����
public:
	CMyQQServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MYQQSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
private:
	IMediator* m_pTCPMediator;//�н��ߵ�ָ��
	CMySql * m_pMySql;

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
