// SpetialFilterDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "SpetialFilterDlg.h"
#include "afxdialogex.h"


// CSpetialFilterDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CSpetialFilterDlg, CDialogEx)

CSpetialFilterDlg::CSpetialFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SPETIAL_FILTER, pParent)
	, m_nWidth(3), m_nHeight(3), m_nBorderValue(0), m_nBorderType(2)
	, m_dSigmaVal(3.), m_dNsigma(3.), m_nGaussianOption(1)
	, m_nFilterMaxSize(11), bIsOpencv(false)
{

}

CSpetialFilterDlg::~CSpetialFilterDlg()
{
}

void CSpetialFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_KERNEL_WIDTH, m_nWidth);
	DDX_Text(pDX, IDC_EDIT_KERNEL_HEIGHT, m_nHeight);
	DDX_Text(pDX, IDC_EDIT_BORDER_VALUE, m_nBorderValue);
	DDX_Radio(pDX, IDC_RADIO_BORDERTYPE1, m_nBorderType);
	DDX_Control(pDX, IDC_EDIT_BORDER_VALUE, m_ctrlBorderValue);
	DDX_Control(pDX, IDC_EDIT_SIGMA_VALUE, m_ctrlSigmaVal);
	DDX_Text(pDX, IDC_EDIT_SIGMA_VALUE, m_dSigmaVal);
	DDX_Control(pDX, IDC_EDIT_N_SIGMA, m_ctrlNsigma);
	DDX_Text(pDX, IDC_EDIT_N_SIGMA, m_dNsigma);
	//  DDX_Control(pDX, IDC_COMBO1, m_ctrlComboBox1);
	DDX_Control(pDX, IDC_EDIT1, m_ctrlTitleText);
	DDX_Control(pDX, IDC_EDIT_KERNEL_HEIGHT, m_ctrlHeight);
	DDX_Control(pDX, IDC_EDIT_KERNEL_WIDTH, m_ctrlWidth);
	DDX_Control(pDX, IDC_STATIC_GAUSSIAN_OPTION, m_ctrlGaussianOption);
	DDX_Radio(pDX, IDC_RADIO_GAUSSIAN_OPTION1, m_nGaussianOption);
	//  DDX_Control(pDX, IDC_RADIO_GAUSSIAN_OPTION1, m_ctrlGaussianOptionKernel);
	DDX_Control(pDX, IDC_STATIC_GROUPBOX_KERNEL, m_ctrlGrBoxKernel);
	DDX_Control(pDX, IDC_STATIC_GROUPBOX_SIGMA, m_ctrlGrBoxSigma);
	DDX_Control(pDX, IDC_STATIC_GAUSSIAN_OPTION2, m_ctrlGaussianOption2);
	//  DDX_Control(pDX, IDC_EDIT_FILTER_SIZE, m_ctrlFilterSize);
	DDX_Control(pDX, IDC_EDIT_FILTER_MAX_SIZE, m_ctrlFilterMaxSize);
	//  DDX_Text(pDX, IDC_EDIT_FILTER_SIZE, m_nFilterSize);
	DDX_Text(pDX, IDC_EDIT_FILTER_MAX_SIZE, m_nFilterMaxSize);
}


BEGIN_MESSAGE_MAP(CSpetialFilterDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_BORDERTYPE1, &CSpetialFilterDlg::OnBnClickedRadioBordertype)
	ON_BN_CLICKED(IDC_RADIO_BORDERTYPE2, &CSpetialFilterDlg::OnBnClickedRadioBordertype)
	ON_BN_CLICKED(IDC_RADIO_BORDERTYPE3, &CSpetialFilterDlg::OnBnClickedRadioBordertype)
	ON_BN_CLICKED(IDC_RADIO_BORDERTYPE4, &CSpetialFilterDlg::OnBnClickedRadioBordertype)
	ON_BN_CLICKED(IDC_RADIO_GAUSSIAN_OPTION1, &CSpetialFilterDlg::OnBnClickedRadioGaussianOption)
	ON_BN_CLICKED(IDC_RADIO_GAUSSIAN_OPTION2, &CSpetialFilterDlg::OnBnClickedRadioGaussianOption)
	ON_WM_CTLCOLOR()
//	ON_WM_PAINT()
END_MESSAGE_MAP()


// CSpetialFilterDlg �޽��� ó�����Դϴ�.


void CSpetialFilterDlg::OnBnClickedRadioBordertype()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	if (m_nBorderType == IOP_BORDER_CONSTANT) {
		m_ctrlBorderValue.EnableWindow(TRUE);
	}
	else {
		m_ctrlBorderValue.EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}



BOOL CSpetialFilterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//>>��Ʈ ����
	CFont g_editFont;
	g_editFont.CreatePointFont(200, TEXT("Times New Roman"));
	GetDlgItem(IDC_EDIT1)->SetFont(&g_editFont);
	g_editFont.Detach();

	//>>���� ��ġ offset
	CRect r;
	m_ctrlTitleText.GetRect(r);
	r.top += 20;
	m_ctrlTitleText.SetRect(r);
	m_ctrlTitleText.EnableWindow(false);

	GetDlgItem(IDC_STATIC_GAUSSIAN_OPTION)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_GAUSSIAN_OPTION2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO_GAUSSIAN_OPTION1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO_GAUSSIAN_OPTION2)->ShowWindow(SW_HIDE);
	m_ctrlFilterMaxSize.EnableWindow(FALSE);		//adaptive median filter �� ��� 


	if (m_nFilterType == FILTER_AVR){ 
		m_strTitleText = _T("Average Filter");
		m_ctrlNsigma.EnableWindow(FALSE);
		m_ctrlSigmaVal.EnableWindow(FALSE);
	}
	else if (m_nFilterType == FILTER_GAUSSIAN) {
		m_strTitleText = _T("Gaussian Filter");

		GetDlgItem(IDC_STATIC_GAUSSIAN_OPTION)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_GAUSSIAN_OPTION2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO_GAUSSIAN_OPTION1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO_GAUSSIAN_OPTION2)->ShowWindow(SW_SHOW);
		OnBnClickedRadioGaussianOption();
	}
	else if (m_nFilterType == FILTER_MAX) {
		m_strTitleText = _T("Maximum Filter");
		m_ctrlNsigma.EnableWindow(FALSE);
		m_ctrlSigmaVal.EnableWindow(FALSE);
	}
	else if (m_nFilterType == FILTER_MIN) {
		m_strTitleText = _T("Minimum Filter");
		m_ctrlNsigma.EnableWindow(FALSE);
		m_ctrlSigmaVal.EnableWindow(FALSE);
	}
	else if (m_nFilterType == FILTER_MED) {
		m_strTitleText = _T("Median Filter");
		m_ctrlNsigma.EnableWindow(FALSE);
		m_ctrlSigmaVal.EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_BORDERTYPE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_BORDERTYPE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_BORDERTYPE3)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_BORDERTYPE4)->EnableWindow(FALSE);
	}
	else if (m_nFilterType == FILTER_MED_ADP) {
		m_strTitleText = _T("Adaptive Median Filter");
		m_ctrlFilterMaxSize.EnableWindow(TRUE);
		m_ctrlHeight.EnableWindow(FALSE);
		m_ctrlWidth.EnableWindow(FALSE);
		m_ctrlNsigma.EnableWindow(FALSE);
		m_ctrlSigmaVal.EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_BORDERTYPE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_BORDERTYPE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_BORDERTYPE3)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_BORDERTYPE4)->EnableWindow(FALSE);
	}
// 	else 
// 		return false;

	//>>���� ��� 
	if (bIsOpencv == true)
		m_strTitleText += _T(" OpenCV");
	m_ctrlTitleText.SetWindowTextW(m_strTitleText);

	//Radiobutton Check() 
	OnBnClickedRadioBordertype();




	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CSpetialFilterDlg::OnBnClickedRadioGaussianOption()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	if (m_nGaussianOption == 0) {	//Do with Kernel Size
		m_ctrlHeight.EnableWindow(TRUE);
		m_ctrlWidth.EnableWindow(TRUE);
		m_ctrlNsigma.EnableWindow(FALSE);
		m_ctrlSigmaVal.EnableWindow(FALSE); 
	}
	else {							//Do with Sigma Value
		m_ctrlHeight.EnableWindow(FALSE);
		m_ctrlWidth.EnableWindow(FALSE);
		m_ctrlNsigma.EnableWindow(TRUE);
		m_ctrlSigmaVal.EnableWindow(TRUE);
	}

	UpdateData(FALSE);
}


HBRUSH CSpetialFilterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.

// 	switch (nCtlColor) {
// 	case CTLCOLOR_DLG:   /// ���̾�α� ������ white��.
// 	{
// 		return (HBRUSH)GetStockObject(WHITE_BRUSH);
// 		break;
// 	}
// 	case CTLCOLOR_BTN:    // ��ư�� ������ ��������...
// 	{
// 		pDC->SetBkMode(TRANSPARENT);
// 		return (HBRUSH)::GetStockObject(NULL_BRUSH);
// 	}
// 	case CTLCOLOR_STATIC:
// 	{
// 		pDC->SetTextColor(RGB(0, 255, 255));  // static text ���ڻ� ����
// 		pDC->SetBkMode(TRANSPARENT);   // static text ���� ����
// 		return (HBRUSH)::GetStockObject(NULL_BRUSH);
// 	}
	
	//>> Do Gaussian Whith TEXT color change 
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_GAUSSIAN_OPTION ||
		pWnd->GetDlgCtrlID() == IDC_STATIC_GAUSSIAN_OPTION2) {
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->SetBkMode(TRANSPARENT);

		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

// 	if (m_nFilterType == FILTER_GAUSSIAN) {
// 		if (m_nGaussianOption == 0) {
// 			if (pWnd->GetDlgCtrlID() == IDC_STATIC_GROUPBOX_KERNEL) {
// 				pDC->SetTextColor(RGB(255, 0, 0));
// 				pDC->SetBkMode(TRANSPARENT);
// 
// 				return (HBRUSH)::GetStockObject(NULL_BRUSH);
// 			}
// 		}
// 
// 		else if (m_nGaussianOption == 1) {
// 			if (pWnd->GetDlgCtrlID() == IDC_STATIC_GROUPBOX_SIGMA) {
// 				pDC->SetTextColor(RGB(255, 0, 0));
// 				pDC->SetBkMode(TRANSPARENT);
// 
// 				return (HBRUSH)::GetStockObject(NULL_BRUSH);
// 			}
// 		}
// 	}


	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}


//void CSpetialFilterDlg::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
//					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
//
//}
