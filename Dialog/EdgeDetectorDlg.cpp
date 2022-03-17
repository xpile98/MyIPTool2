// EdgeDetectorDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "EdgeDetectorDlg.h"
#include "afxdialogex.h"
#include "MyIPEdge.h"


// CEdgeDetectorDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CEdgeDetectorDlg, CDialogEx)

CEdgeDetectorDlg::CEdgeDetectorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EDGE_DIALOG, pParent)
	, m_nEdgeType(0) , m_bNormalize(false)
	, m_nLowTh(100), m_nHighTh(200)
	, m_dLogSigma(1.4), m_dDogSigma1(1.4), m_dDogSigma2(0.9)
{

}

CEdgeDetectorDlg::~CEdgeDetectorDlg()
{
}

void CEdgeDetectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_EDGE1, m_nEdgeType);
	DDX_Check(pDX, IDC_CHECK_NORMALIZE, m_bNormalize);
	DDX_Control(pDX, IDC_EDIT_LOWTH, m_ctrlLowTh);
	DDX_Control(pDX, IDC_EDIT_HIGHTH, m_ctrlHighTh);
	DDX_Text(pDX, IDC_EDIT_LOWTH, m_nLowTh);
	DDX_Text(pDX, IDC_EDIT_HIGHTH, m_nHighTh);
	DDX_Control(pDX, IDC_EDIT_LOG_SIGMA, m_ctrlLogSigma);
	DDX_Control(pDX, IDC_EDIT_DOG_SIGMA1, m_ctrlDogSigma1);
	DDX_Control(pDX, IDC_EDIT_DOG_SIGMA2, m_ctrlDogSigma2);
	DDX_Text(pDX, IDC_EDIT_LOG_SIGMA, m_dLogSigma);
	DDX_Text(pDX, IDC_EDIT_DOG_SIGMA1, m_dDogSigma1);
	DDX_Text(pDX, IDC_EDIT_DOG_SIGMA2, m_dDogSigma2);
	DDX_Control(pDX, IDC_SLIDER1, m_ctrlSlider1);
	DDX_Control(pDX, IDC_SLIDER2, m_ctrlSlider2);
}


BEGIN_MESSAGE_MAP(CEdgeDetectorDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_EDGE1, &CEdgeDetectorDlg::OnBnClickedRadioEdge)
	ON_BN_CLICKED(IDC_RADIO_EDGE2, &CEdgeDetectorDlg::OnBnClickedRadioEdge)
	ON_BN_CLICKED(IDC_RADIO_EDGE3, &CEdgeDetectorDlg::OnBnClickedRadioEdge)
	ON_BN_CLICKED(IDC_RADIO_EDGE4, &CEdgeDetectorDlg::OnBnClickedRadioEdge)
	ON_BN_CLICKED(IDC_RADIO_EDGE5, &CEdgeDetectorDlg::OnBnClickedRadioEdge)
	ON_BN_CLICKED(IDC_RADIO_EDGE6, &CEdgeDetectorDlg::OnBnClickedRadioEdge)
	ON_BN_CLICKED(IDC_RADIO_EDGE7, &CEdgeDetectorDlg::OnBnClickedRadioEdge)
	ON_BN_CLICKED(IDC_RADIO_EDGE8, &CEdgeDetectorDlg::OnBnClickedRadioEdge)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT_LOG_SIGMA, &CEdgeDetectorDlg::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_DOG_SIGMA1, &CEdgeDetectorDlg::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_DOG_SIGMA2, &CEdgeDetectorDlg::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_LOWTH, &CEdgeDetectorDlg::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_HIGHTH, &CEdgeDetectorDlg::OnEnChangeEdit)
END_MESSAGE_MAP()


// CEdgeDetectorDlg �޽��� ó�����Դϴ�.


void CEdgeDetectorDlg::OnBnClickedRadioEdge()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_ctrlHighTh.EnableWindow(FALSE);
	m_ctrlLowTh.EnableWindow(FALSE);
	m_ctrlLogSigma.EnableWindow(FALSE);
	m_ctrlDogSigma1.EnableWindow(FALSE);
	m_ctrlDogSigma2.EnableWindow(FALSE);
	m_ctrlSlider1.EnableWindow(FALSE);
	m_ctrlSlider2.EnableWindow(FALSE);
	m_ctrlSlider1.SetPos(0);
	m_ctrlSlider2.SetPos(0);

	if (m_nEdgeType == EDGE_CANNY) {
		m_ctrlHighTh.EnableWindow(TRUE);
		m_ctrlLowTh.EnableWindow(TRUE);
		m_ctrlSlider1.EnableWindow(TRUE);
		m_ctrlSlider2.EnableWindow(TRUE);
		m_ctrlSlider1.SetRange(0, 255);
		m_ctrlSlider2.SetRange(0, 255);
		m_ctrlSlider1.SetPos(60);
		m_ctrlSlider2.SetPos(180);
	}
	else if (m_nEdgeType == EDGE_LOG) {
		m_ctrlLogSigma.EnableWindow(TRUE);
		m_ctrlSlider1.EnableWindow(TRUE);
		m_ctrlSlider1.SetRange(0, 50);		//double sigma 0.0~5.0
		m_ctrlSlider1.SetPos(14);
	}
	else if (m_nEdgeType == EDGE_DOG) {
		m_ctrlDogSigma1.EnableWindow(TRUE);
		m_ctrlDogSigma2.EnableWindow(TRUE);
		m_ctrlSlider1.EnableWindow(TRUE);
		m_ctrlSlider2.EnableWindow(TRUE);
		m_ctrlSlider1.SetRange(0, 50);		//double sigma 0.0~5.0
		m_ctrlSlider2.SetRange(0, 50);		//double sigma 0.0~5.0
		m_ctrlSlider1.SetPos(9);
		m_ctrlSlider2.SetPos(14);
	}
}



BOOL CEdgeDetectorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	m_ctrlSlider1.SetTicFreq(32);
	m_ctrlSlider1.SetPageSize(32);
	m_ctrlSlider2.SetTicFreq(32);
	m_ctrlSlider2.SetPageSize(32);

	OnBnClickedRadioEdge();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CEdgeDetectorDlg::DisplayImage()
{
	MyIPImage img;
	img = m_pDoc->m_MyIPImage;

	if (m_nEdgeType == EDGE_CANNY) {
		if (IpCanny(img, m_nLowTh, m_nHighTh)) {
			CClientDC dc(m_pView);	//���� view�ٰ� dc������!	view�� dc(�׸��׸��� ��)�� �������� 
			img.Draw(dc.m_hDC, 0, 0);
		}
	}
	else if (m_nEdgeType == EDGE_LOG) {
		if (IpLoG(img, m_dLogSigma)) {
			CClientDC dc(m_pView);	//���� view�ٰ� dc������!	view�� dc(�׸��׸��� ��)�� �������� 
			img.Draw(dc.m_hDC, 0, 0);
		}
	}
	else if (m_nEdgeType == EDGE_DOG) {
		if (IpDoG(img, m_dDogSigma1, m_dDogSigma2)) {
			CClientDC dc(m_pView);	//���� view�ٰ� dc������!	view�� dc(�׸��׸��� ��)�� �������� 
			img.Draw(dc.m_hDC, 0, 0);
		}
	}
}



void CEdgeDetectorDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	UpdateData(TRUE);
	if (m_ctrlSlider1.GetSafeHwnd() == pScrollBar->GetSafeHwnd())	//���� ���õ� ��ũ�ѹ� ��ȣ�� m_sliderOffset�����̴��� ����?
	{
		if (m_nEdgeType == EDGE_CANNY) {
			m_nLowTh = m_ctrlSlider1.GetPos();
		}
		else if (m_nEdgeType == EDGE_LOG) {
			m_dLogSigma = m_ctrlSlider1.GetPos() * 0.1;
		}
		else if (m_nEdgeType == EDGE_DOG) {
			m_dDogSigma1 = m_ctrlSlider1.GetPos() * 0.1;
		}
		UpdateData(false);	//�������� ��Ʈ�ѿ� �����~ 
		DisplayImage();
	}

	else if (m_ctrlSlider2.GetSafeHwnd() == pScrollBar->GetSafeHwnd())	//���� ���õ� ��ũ�ѹ� ��ȣ�� m_sliderOffset�����̴��� ����?
	{
		if (m_nEdgeType == EDGE_CANNY) {
			m_nHighTh = m_ctrlSlider2.GetPos();
		}
		//no log
		else if (m_nEdgeType == EDGE_DOG) {
			m_dDogSigma2 = m_ctrlSlider2.GetPos() * 0.1;
		}
		UpdateData(false);	//�������� ��Ʈ�ѿ� �����~ 
		DisplayImage();
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CEdgeDetectorDlg::OnEnChangeEdit()
{
	UpdateData(TRUE);
	if (m_nEdgeType == EDGE_CANNY) {
		if (m_nLowTh < 0 || m_nLowTh > 255)
		{
			if (m_nLowTh < 0)			m_nLowTh = 0;
			else if (m_nLowTh > 255)	m_nLowTh = 255;
			AfxMessageBox(_T("0���� 255 ������ ������ �Է��Ͻʽÿ�."));
		}
		if (m_nHighTh < 0 || m_nHighTh > 255)
		{
			if (m_nHighTh < 0)			m_nHighTh = 0;
			else if (m_nHighTh > 255)	m_nHighTh = 255;
			AfxMessageBox(_T("0���� 255 ������ ������ �Է��Ͻʽÿ�."));
		}
		m_ctrlSlider1.SetPos(m_nLowTh);
		m_ctrlSlider2.SetPos(m_nHighTh);
	}
	else if (m_nEdgeType == EDGE_LOG) {
		if (m_dLogSigma < 0 || m_dLogSigma > 5.0)
		{
			if (m_dLogSigma < 0)			m_dLogSigma = 0;
			else if (m_dLogSigma > 5.0)	m_dLogSigma = 5.0;
			AfxMessageBox(_T("0���� 5.0 ������ �Ǽ��� �Է��Ͻʽÿ�."));
		}
		m_ctrlSlider1.SetPos((int)(m_dLogSigma * 10));
	}
	else if (m_nEdgeType == EDGE_DOG) {
		if (m_dDogSigma1 < 0 || m_dDogSigma1 > 5.0)
		{
			if (m_dDogSigma1 < 0)			m_dDogSigma1 = 0;
			else if (m_dDogSigma1 > 5.0)	m_dDogSigma1 = 5.0;
			AfxMessageBox(_T("0���� 5.0 ������ �Ǽ��� �Է��Ͻʽÿ�."));
		}
		if (m_dDogSigma2 < 0 || m_dDogSigma2 > 5.0)
		{
			if (m_dDogSigma2 < 0)			m_dDogSigma2 = 0;
			else if (m_dDogSigma2 > 5.0)	m_dDogSigma2 = 5.0;
			AfxMessageBox(_T("0���� 5.0 ������ �Ǽ��� �Է��Ͻʽÿ�."));
		}
		m_ctrlSlider1.SetPos((int)(m_dDogSigma1 * 10));
		m_ctrlSlider2.SetPos((int)(m_dDogSigma2 * 10));
	}
	UpdateData(FALSE);
	DisplayImage();
}

