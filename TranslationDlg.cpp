// TranslationDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "TranslationDlg.h"
#include "afxdialogex.h"


// TranslationDlg ��ȭ �����Դϴ�.

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


// TranslationDlg �޽��� ó�����Դϴ�.


void TranslationDlg::OnBnClickedCheck1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	GetDlgItem(IDC_EDIT3)->EnableWindow(false);
	GetDlgItem(IDC_EDIT4)->EnableWindow(false);
	GetDlgItem(IDC_EDIT5)->EnableWindow(false);
	GetDlgItem(IDC_EDIT6)->EnableWindow(false);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
