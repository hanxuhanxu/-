#include "stdafx.h"
#include "CgHeadDlg.h"

#include "MyQQClient.h"
#include "CgHeadDlg.h"
#include "afxdialogex.h"

#include "MyQQClientDlg.h"
#include"MyFriend.h"
#include"SendMsg.h"

//#include "FriDlg.h"
IMPLEMENT_DYNAMIC(CCgHeadDlg, CDialogEx)

CCgHeadDlg::CCgHeadDlg(CWnd* pParent )
	:CDialogEx(CCgHeadDlg::IDD, pParent)
{
}


CCgHeadDlg::~CCgHeadDlg(void)
{
}

BOOL CCgHeadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//���ͷ��
	HBITMAP hBmp;
	hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP3));
	((CButton *)GetDlgItem(IDC_BUTTON1))->SetBitmap(hBmp);

	hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP4));
	((CButton *)GetDlgItem(IDC_BUTTON2))->SetBitmap(hBmp);

	hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP5));
	((CButton *)GetDlgItem(IDC_BUTTON3))->SetBitmap(hBmp);

	hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP6));
	((CButton *)GetDlgItem(IDC_BUTTON4))->SetBitmap(hBmp);

	hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP7));
	((CButton *)GetDlgItem(IDC_BUTTON5))->SetBitmap(hBmp);

	hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP8));
	((CButton *)GetDlgItem(IDC_BUTTON6))->SetBitmap(hBmp);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CCgHeadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CCgHeadDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	//�������ͬ��ͷ��

	CDialogEx::OnCancel();
}

BEGIN_MESSAGE_MAP(CCgHeadDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CCgHeadDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CCgHeadDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CCgHeadDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CCgHeadDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CCgHeadDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CCgHeadDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


void CCgHeadDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	((CButton *)AfxGetMainWnd()->GetDlgItem(IDC_HEAD))->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDB_BITMAP3)));

	((CButton *)GetParent()->GetDlgItem(IDC_BTN_HEAD))->SetIcon(AfxGetApp()->LoadIcon(IDI_ICON2));

	UpdateHeadPic(1);
}


void CCgHeadDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	((CButton *)AfxGetMainWnd()->GetDlgItem(IDC_HEAD))->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDB_BITMAP4)));
	((CButton *)GetParent()->GetDlgItem(IDC_BTN_HEAD))->SetIcon(AfxGetApp()->LoadIcon(IDI_ICON3));

	UpdateHeadPic(2);
}


void CCgHeadDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	((CButton *)AfxGetMainWnd()->GetDlgItem(IDC_HEAD))->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDB_BITMAP5)));
	((CButton *)GetParent()->GetDlgItem(IDC_BTN_HEAD))->SetIcon(AfxGetApp()->LoadIcon(IDI_ICON4));

	UpdateHeadPic(3);
}


void CCgHeadDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	((CButton *)AfxGetMainWnd()->GetDlgItem(IDC_HEAD))->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDB_BITMAP6)));
	((CButton *)GetParent()->GetDlgItem(IDC_BTN_HEAD))->SetIcon(AfxGetApp()->LoadIcon(IDI_ICON5));

	UpdateHeadPic(4);
}


void CCgHeadDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	((CButton *)AfxGetMainWnd()->GetDlgItem(IDC_HEAD))->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDB_BITMAP7)));
	((CButton *)GetParent()->GetDlgItem(IDC_BTN_HEAD))->SetIcon(AfxGetApp()->LoadIcon(IDI_ICON6));

	UpdateHeadPic(5);
}


void CCgHeadDlg::OnBnClickedButton6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	((CButton *)AfxGetMainWnd()->GetDlgItem(IDC_HEAD))->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDB_BITMAP8)));
	((CButton *)GetParent()->GetDlgItem(IDC_BTN_HEAD))->SetIcon(AfxGetApp()->LoadIcon(IDI_ICON7));

	UpdateHeadPic(6);
}

void CCgHeadDlg::UpdateHeadPic(UINT headNum)
{
	//SendMsg sendMsg;
	//CString windowText;
	//((CMyFriend *)GetParent())->GetWindowText(windowText);
	//sendMsg.isHeadPic = TRUE;
	//_tcscpy_s(sendMsg.userName, windowText);
	//sendMsg.headNum = headNum;
	//send(((CMyFriend *)GetParent())->GetSockCli(), (char *)&sendMsg, sizeof(sendMsg), 0);
}