// CVThresholdDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "CVThresholdDlg.h"
#include "afxdialogex.h"


// CCVThresholdDlg 대화 상자입니다.

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


// CCVThresholdDlg 메시지 처리기입니다.


void CCVThresholdDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CDialogEx::OnOK();
}


void CCVThresholdDlg::OnBnClickedMethod()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CCVThresholdDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnClose();
}
