// MorphologyTestDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "LoopDlg.h"
#include "afxdialogex.h"


// CLoopDlg 대화 상자입니다.

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


// CLoopDlg 메시지 처리기입니다.


void CLoopDlg::OnBnClickedButtonAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	bAddFlag = true;
	CDialogEx::OnOK();
}


void CLoopDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	bAddFlag = false;
	CDialogEx::OnOK();
}
