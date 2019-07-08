#pragma once
#include "afxwin.h"
#include "Packdef.h"
#include"MyQQClientDlg.h"
#include<mmeapi.h>
#include<Vfw.h>
//#include"../SDL/SDL.h"
//#include"SDL\SDL.h"
//#pragma comment(lib,"./")
#include"../include/SDL.h"
#include"Myffmpeg.h"

// CVideoDlg �Ի���

class CVideoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVideoDlg)

public:
	CVideoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVideoDlg();

// �Ի�������
	enum { IDD =IDD_VIDEODLG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_VideoYours;
	int m_VideoMine;
	CStatic m_VideoFriend;
	CStatic m_VideoSelf;
	Myffmpeg m_ffmpeg;
	bool    m_bdesktop;
	bool    m_bcamera;
	bool    m_baudio;
public:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
