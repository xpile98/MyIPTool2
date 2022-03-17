// ShapeningFilter.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "ShapeningFilterDlg.h"
#include "afxdialogex.h"


// CShapeningFilterDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CShapeningFilterDlg, CDialogEx)

CShapeningFilterDlg::CShapeningFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SARPENING_FILTER, pParent)
	, m_dSigma(3.0), m_dWeight(1.0)
	, m_nLaplacianType(0)
{
	 
}

CShapeningFilterDlg::~CShapeningFilterDlg()
{
}

void CShapeningFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SIGMA, m_ctrlSigma);
	DDX_Text(pDX, IDC_EDIT_SIGMA, m_dSigma);
	DDX_Control(pDX, IDC_EDIT_WEIGHT, m_ctrlWeight);
	DDX_Text(pDX, IDC_EDIT_WEIGHT, m_dWeight);
	DDX_Control(pDX, IDC_CHECK_UNSHARP_MASKING, m_ctrlCheckUnsharpMasking);
	DDX_Control(pDX, IDC_EDIT1, m_ctrlTitleText);
	DDX_Radio(pDX, IDC_RADIO_LAPLACIAN1, m_nLaplacianType);
}


BEGIN_MESSAGE_MAP(CShapeningFilterDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_UNSHARP_MASKING, &CShapeningFilterDlg::OnBnClickedCheckUnsharpMasking)
	ON_BN_CLICKED(IDC_RADIO_LAPLACIAN1, &CShapeningFilterDlg::OnBnClickedRadioLaplacian)
	ON_BN_CLICKED(IDC_RADIO_LAPLACIAN2, &CShapeningFilterDlg::OnBnClickedRadioLaplacian)
END_MESSAGE_MAP()


// CShapeningFilterDlg �޽��� ó�����Դϴ�.


BOOL CShapeningFilterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	//>>��Ʈ ����
	CFont g_editFont;
	g_editFont.CreatePointFont(200, TEXT("Times New Roman"));
	GetDlgItem(IDC_EDIT1)->SetFont(&g_editFont);
	g_editFont.Detach();

	//>>���� offset ���� 
	CRect r;
	m_ctrlTitleText.GetRect(r);
	r.top += 20;
	m_ctrlTitleText.SetRect(r);
	m_ctrlTitleText.EnableWindow(false);

	//ctrl enablewindow(false)
	m_ctrlWeight.EnableWindow(FALSE);
	m_ctrlSigma.EnableWindow(FALSE);
	m_ctrlCheckUnsharpMasking.EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_LAPLACIAN1)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_LAPLACIAN2)->EnableWindow(FALSE);
	
	if (m_nFilterType == SHP_HIGHBOOST) {
		m_strTitleText = _T("High Boost Filter");
		m_ctrlWeight.EnableWindow(TRUE);
		m_ctrlSigma.EnableWindow(TRUE);
		m_ctrlCheckUnsharpMasking.EnableWindow(TRUE);
	}
	else if (m_nFilterType == SHP_LAPLACIAN) {
		m_strTitleText = _T("Laplacian Sharpening");
		GetDlgItem(IDC_RADIO_LAPLACIAN1)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_LAPLACIAN2)->EnableWindow(TRUE);
	}

	//>>���� ��� 
	m_ctrlTitleText.SetWindowTextW(m_strTitleText);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CShapeningFilterDlg::OnBnClickedCheckUnsharpMasking()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	if (m_ctrlCheckUnsharpMasking.GetCheck()==FALSE) {
		m_ctrlWeight.EnableWindow(TRUE);
		m_strTitleText = _T("High Boost Filter");
	}
	else if (m_ctrlCheckUnsharpMasking.GetCheck()==TRUE) {
		m_dWeight = 1.0;
		m_ctrlWeight.EnableWindow(FALSE);
		m_strTitleText = _T("Unsharp Masking Filter");
	}

	m_ctrlTitleText.SetWindowTextW(m_strTitleText);
	UpdateData(FALSE);
}


void CShapeningFilterDlg::OnBnClickedRadioLaplacian()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	if (m_nLaplacianType == 0) {		//laplacian 4

	}
	else if (m_nLaplacianType == 1) {	//laplacian 8

	}	
}
