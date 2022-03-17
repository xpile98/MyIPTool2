// FrequencyFilterDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "FrequencyFilterDlg.h"
#include "afxdialogex.h"


// CFrequencyFilterDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CFrequencyFilterDlg, CDialogEx)

CFrequencyFilterDlg::CFrequencyFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FREQ_FILTER_DLG, pParent)
	, m_nCOFval(50)
	, m_nBPFval(10)
	, m_nBUTval(3)
	, m_nFilterType(0)
	, m_nFilterShape(0)
	, m_bDoScale(FALSE)
	, m_bAddAvr(FALSE)
{

}

CFrequencyFilterDlg::~CFrequencyFilterDlg()
{
}

void CFrequencyFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_COF_VAL, m_nCOFval);
	DDX_Text(pDX, IDC_EDIT_BPF_VAL, m_nBPFval);
	DDX_Text(pDX, IDC_EDIT_BUTTER_VAL, m_nBUTval);
	DDX_CBIndex(pDX, IDC_COMBO_FILTER_TYPE, m_nFilterType);
	DDX_CBIndex(pDX, IDC_COMBO_FILTER_SHAPE, m_nFilterShape);
	DDX_Check(pDX, IDC_CHECK_SCALE, m_bDoScale);
	DDX_Check(pDX, IDC_CHECK_AVR, m_bAddAvr);
}


BEGIN_MESSAGE_MAP(CFrequencyFilterDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_FILTER_TYPE, &CFrequencyFilterDlg::OnCbnSelchangeComboFilterType)
	ON_CBN_SELCHANGE(IDC_COMBO_FILTER_SHAPE, &CFrequencyFilterDlg::OnCbnSelchangeComboFilterShape)
END_MESSAGE_MAP()


// CFrequencyFilterDlg �޽��� ó�����Դϴ�.


void CFrequencyFilterDlg::OnCbnSelchangeComboFilterType()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_COF_VAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_BPF_VAL)->EnableWindow(FALSE);
	if (m_nFilterShape != 2)	//if not butterwidth shape
		GetDlgItem(IDC_EDIT_BUTTER_VAL)->EnableWindow(FALSE);

	switch (m_nFilterType)
	{
	case 0:
	case 1:
		GetDlgItem(IDC_EDIT_COF_VAL)->EnableWindow(TRUE);
		break;
	case 2:
	case 3:
		GetDlgItem(IDC_EDIT_COF_VAL)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_BPF_VAL)->EnableWindow(TRUE);
		break;
	}
}


void CFrequencyFilterDlg::OnCbnSelchangeComboFilterShape()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	GetDlgItem(IDC_EDIT_COF_VAL)->EnableWindow(TRUE);		//cof�� �׻� �ʿ� 
	if (m_nFilterType == 2 || m_nFilterType == 3) //if BPF or BRF
		GetDlgItem(IDC_EDIT_BPF_VAL)->EnableWindow(TRUE);

	switch (m_nFilterShape)
	{
	case 0:
	case 1:
		GetDlgItem(IDC_EDIT_BUTTER_VAL)->EnableWindow(FALSE);
		break;
	case 2:
		GetDlgItem(IDC_EDIT_BUTTER_VAL)->EnableWindow(TRUE);
		break;
	default:
		break;
	}
}


BOOL CFrequencyFilterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	OnCbnSelchangeComboFilterType();
	OnCbnSelchangeComboFilterShape();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
