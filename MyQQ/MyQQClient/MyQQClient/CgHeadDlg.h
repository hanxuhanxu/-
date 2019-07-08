#pragma once
#include"resource.h"

class CCgHeadDlg:public CDialogEx
{
	DECLARE_DYNAMIC(CCgHeadDlg)
public:
	CCgHeadDlg(CWnd* pParent = NULL);
	~CCgHeadDlg(void);


// 对话框数据
	enum { IDD = IDD_CGHEADDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	void UpdateHeadPic(UINT headNum);
	afx_msg void OnBnClickedBtnClose();
	//afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
};

