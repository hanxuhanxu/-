
// MyQQClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMyQQClientApp:
// �йش����ʵ�֣������ MyQQClient.cpp
//

class CMyQQClientApp : public CWinApp
{
public:
	CMyQQClientApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMyQQClientApp theApp;