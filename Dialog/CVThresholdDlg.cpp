// CVThresholdDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "CVThresholdDlg.h"
#include "afxdialogex.h"


// CCVThresholdDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCVThresholdDlg, CDialogEx)

CCVThresholdDlg::CCVThresholdDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_THRESHOLD_CV, pParent)
	, m_nType(0)
	, m_nMethod(0)
{

}

CCVThresholdDlg::~CCVThresholdDlg()
{
}

void CCVThresholdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_TYPE1, m_nType);
	DDX_Radio(pDX, IDC_METHOD1, m_nMethod);
}


BEGIN_MESSAGE_MAP(CCVThresholdDlg, CDialogEx)
	ON_BN_CLICKED(IDC_METHOD1, &CCVThresholdDlg::OnBnClickedMethod)
	ON_BN_CLICKED(IDC_METHOD2, &CCVThresholdDlg::OnBnClickedMethod)
	ON_BN_CLICKED(IDC_METHOD3, &CCVThresholdDlg::OnBnClickedMethod)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CCVThresholdDlg �޽��� ó�����Դϴ�.


void CCVThresholdDlg::OnOK()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	CDialogEx::OnOK();
}


void CCVThresholdDlg::OnBnClickedMethod()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CCVThresholdDlg::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnClose();
}
