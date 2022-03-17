// TranslationDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "TranslationDlg.h"
#include "afxdialogex.h"


// TranslationDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(TranslationDlg, CDialogEx)

TranslationDlg::TranslationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TRANSLATION, pParent)
	, m_nShiftX(100)	, m_nShiftY(0)
	, m_nRoiX(190), m_nRoiY(100)
	, m_nRoiW(200), m_nRoiH(600)
	, m_bRoiMode(FALSE)
{

}

TranslationDlg::~TranslationDlg()
{
}

void TranslationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nShiftX);
	DDX_Text(pDX, IDC_EDIT2, m_nShiftY);
	DDX_Text(pDX, IDC_EDIT3, m_nRoiX);
	DDX_Text(pDX, IDC_EDIT4, m_nRoiY);
	DDX_Text(pDX, IDC_EDIT5, m_nRoiW);
	DDX_Text(pDX, IDC_EDIT6, m_nRoiH);
	DDX_Check(pDX, IDC_CHECK1, m_bRoiMode);
}


BEGIN_MESSAGE_MAP(TranslationDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK1, &TranslationDlg::OnBnClickedCheck1)
END_MESSAGE_MAP()


// TranslationDlg 메시지 처리기입니다.


void TranslationDlg::OnBnClickedCheck1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//UpdateData(FALSE);
	m_bRoiMode = !m_bRoiMode;
	GetDlgItem(IDC_EDIT3)->EnableWindow(m_bRoiMode);
	GetDlgItem(IDC_EDIT4)->EnableWindow(m_bRoiMode);
	GetDlgItem(IDC_EDIT5)->EnableWindow(m_bRoiMode);
	GetDlgItem(IDC_EDIT6)->EnableWindow(m_bRoiMode);
	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(m_bRoiMode);
}


BOOL TranslationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	GetDlgItem(IDC_EDIT3)->EnableWindow(false);
	GetDlgItem(IDC_EDIT4)->EnableWindow(false);
	GetDlgItem(IDC_EDIT5)->EnableWindow(false);
	GetDlgItem(IDC_EDIT6)->EnableWindow(false);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
