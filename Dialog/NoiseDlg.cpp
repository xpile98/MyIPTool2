// NoiseDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "NoiseDlg.h"
#include "afxdialogex.h"


// CNoiseDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CNoiseDlg, CDialogEx)

CNoiseDlg::CNoiseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NOISE_DLG, pParent)
	, m_nRadioNoise(0) , m_dMean(0.) , m_dSigma(3.) , m_dSalt(0.1) , m_dPepper(0.1)
	, m_bCheckRGB(FALSE)
{

}

CNoiseDlg::~CNoiseDlg()
{
}

void CNoiseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nRadioNoise);
	DDX_Control(pDX, IDC_EDIT_G_MEAN, m_ctrlMean);
	DDX_Control(pDX, IDC_EDIT_G_SIGMA, m_ctrlSigma);
	DDX_Control(pDX, IDC_EDIT_SP_SALT, m_ctrlSalt);
	DDX_Control(pDX, IDC_EDIT_SP_PEPPER, m_ctrlPepper);
	DDX_Text(pDX, IDC_EDIT_G_MEAN, m_dMean);
	DDX_Text(pDX, IDC_EDIT_G_SIGMA, m_dSigma);
	DDX_Text(pDX, IDC_EDIT_SP_SALT, m_dSalt);
	DDX_Text(pDX, IDC_EDIT_SP_PEPPER, m_dPepper);
	DDX_Control(pDX, IDC_CHECK1, m_ctrlRGB);
	DDX_Check(pDX, IDC_CHECK1, m_bCheckRGB);
}


BEGIN_MESSAGE_MAP(CNoiseDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO1, &CNoiseDlg::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO2, &CNoiseDlg::OnBnClickedRadio)
END_MESSAGE_MAP()


// CNoiseDlg �޽��� ó�����Դϴ�.


void CNoiseDlg::OnBnClickedRadio()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	if (m_nRadioNoise == 0) {		//Gaussian
		m_ctrlMean.EnableWindow(TRUE);
		m_ctrlSigma.EnableWindow(TRUE);
		m_ctrlSalt.EnableWindow(FALSE);
		m_ctrlPepper.EnableWindow(FALSE);
		m_ctrlRGB.EnableWindow(FALSE);
	}
	else if (m_nRadioNoise == 1) {	//salt pepper
		m_ctrlMean.EnableWindow(FALSE);
		m_ctrlSigma.EnableWindow(FALSE);
		m_ctrlSalt.EnableWindow(TRUE);
		m_ctrlPepper.EnableWindow(TRUE);
		m_ctrlRGB.EnableWindow(TRUE);
		m_ctrlRGB.SetCheck(false);
	}
}




BOOL CNoiseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	OnBnClickedRadio();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
