// MorphologyTestDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "LoopDlg.h"
#include "afxdialogex.h"


// CLoopDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CLoopDlg, CDialogEx)

CLoopDlg::CLoopDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOOP_DLG, pParent)
	, m_nLoopCnt(0), bAddFlag(false)
{

}

CLoopDlg::~CLoopDlg()
{
}

void CLoopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nLoopCnt);
}


BEGIN_MESSAGE_MAP(CLoopDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CLoopDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDOK, &CLoopDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLoopDlg �޽��� ó�����Դϴ�.


void CLoopDlg::OnBnClickedButtonAdd()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	bAddFlag = true;
	CDialogEx::OnOK();
}


void CLoopDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	bAddFlag = false;
	CDialogEx::OnOK();
}
