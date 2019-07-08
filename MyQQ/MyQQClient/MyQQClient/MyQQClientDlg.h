
// MyQQClientDlg.h : ͷ�ļ�
//

#pragma once
#include"IMediator.h"
#include"afxwin.h"
#include"MyFriend.h"
#include<fstream>
#include<list>
//#include"Myffmpeg.h"

// CMyQQClientDlg �Ի���
class CMyQQClientDlg : public CDialogEx
{
// ����
public:
	CMyQQClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MYQQCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
private:
	IMediator* m_pTCPMediator;//�н��ߵ�ָ��

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
    afx_msg void OnDestroy();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnLineMsg(WPARAM,LPARAM);
	afx_msg LRESULT 	OffLineMsg(WPARAM,LPARAM);
	afx_msg LRESULT DataInfoMsg(WPARAM,LPARAM);
	afx_msg LRESULT DestroyMsg(WPARAM,LPARAM);
	afx_msg LRESULT OnFileMsg(WPARAM,LPARAM);
	afx_msg LRESULT OnFileInfoMsg(WPARAM,LPARAM);
	afx_msg LRESULT 	OnPicMsg(WPARAM,LPARAM);
	afx_msg LRESULT 	OnInviteMsg(WPARAM,LPARAM);
	afx_msg LRESULT 	OnKeepConnectMsg(WPARAM,LPARAM);
	CString OnBnClickedDecgen() ;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedBtnAbout();
	afx_msg void OnBnClickedBtnMin();
	afx_msg void OnBnClickedBtnClose();
    DECLARE_MESSAGE_MAP()
	void LoadHeadPic(UINT headNum);
	UINT WhichHeadPic(void);
	HICON GetHICONByHeadNum(UINT headNum);
public:
	IMediator* GetMediator()
	{
		return m_pTCPMediator;
	}
public:
	CString m_UserName;
	CString m_Password;
	CMyFriend* m_pMyFriend;
	CMyDlgChat * m_pMyChatDlg;
	list<CString>m_lstFriend; //�ҵĺ����б� ��ʽ
	list<CString>m_strlstKeepConnent; //��ʱ�����б�
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
