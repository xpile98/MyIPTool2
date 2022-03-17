// EdgeDetectorDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "EdgeDetectorDlg.h"
#include "afxdialogex.h"
#include "MyIPEdge.h"


// CEdgeDetectorDlg 대화 상자입니다.

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


// CEdgeDetectorDlg 메시지 처리기입니다.


void CEdgeDetectorDlg::OnBnClickedRadioEdge()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_ctrlSlider1.SetTicFreq(32);
	m_ctrlSlider1.SetPageSize(32);
	m_ctrlSlider2.SetTicFreq(32);
	m_ctrlSlider2.SetPageSize(32);

	OnBnClickedRadioEdge();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CEdgeDetectorDlg::DisplayImage()
{
	MyIPImage img;
	img = m_pDoc->m_MyIPImage;

	if (m_nEdgeType == EDGE_CANNY) {
		if (IpCanny(img, m_nLowTh, m_nHighTh)) {
			CClientDC dc(m_pView);	//내가 view줄게 dc가져왕!	view의 dc(그림그리는 툴)을 가져오자 
			img.Draw(dc.m_hDC, 0, 0);
		}
	}
	else if (m_nEdgeType == EDGE_LOG) {
		if (IpLoG(img, m_dLogSigma)) {
			CClientDC dc(m_pView);	//내가 view줄게 dc가져왕!	view의 dc(그림그리는 툴)을 가져오자 
			img.Draw(dc.m_hDC, 0, 0);
		}
	}
	else if (m_nEdgeType == EDGE_DOG) {
		if (IpDoG(img, m_dDogSigma1, m_dDogSigma2)) {
			CClientDC dc(m_pView);	//내가 view줄게 dc가져왕!	view의 dc(그림그리는 툴)을 가져오자 
			img.Draw(dc.m_hDC, 0, 0);
		}
	}
}



void CEdgeDetectorDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	UpdateData(TRUE);
	if (m_ctrlSlider1.GetSafeHwnd() == pScrollBar->GetSafeHwnd())	//지금 선택된 스크롤바 번호랑 m_sliderOffset슬라이더랑 같냐?
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
		UpdateData(false);	//변수값을 컨트롤에 써줘라~ 
		DisplayImage();
	}

	else if (m_ctrlSlider2.GetSafeHwnd() == pScrollBar->GetSafeHwnd())	//지금 선택된 스크롤바 번호랑 m_sliderOffset슬라이더랑 같냐?
	{
		if (m_nEdgeType == EDGE_CANNY) {
			m_nHighTh = m_ctrlSlider2.GetPos();
		}
		//no log
		else if (m_nEdgeType == EDGE_DOG) {
			m_dDogSigma2 = m_ctrlSlider2.GetPos() * 0.1;
		}
		UpdateData(false);	//변수값을 컨트롤에 써줘라~ 
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
			AfxMessageBox(_T("0에서 255 사이의 정수를 입력하십시오."));
		}
		if (m_nHighTh < 0 || m_nHighTh > 255)
		{
			if (m_nHighTh < 0)			m_nHighTh = 0;
			else if (m_nHighTh > 255)	m_nHighTh = 255;
			AfxMessageBox(_T("0에서 255 사이의 정수를 입력하십시오."));
		}
		m_ctrlSlider1.SetPos(m_nLowTh);
		m_ctrlSlider2.SetPos(m_nHighTh);
	}
	else if (m_nEdgeType == EDGE_LOG) {
		if (m_dLogSigma < 0 || m_dLogSigma > 5.0)
		{
			if (m_dLogSigma < 0)			m_dLogSigma = 0;
			else if (m_dLogSigma > 5.0)	m_dLogSigma = 5.0;
			AfxMessageBox(_T("0에서 5.0 사이의 실수를 입력하십시오."));
		}
		m_ctrlSlider1.SetPos((int)(m_dLogSigma * 10));
	}
	else if (m_nEdgeType == EDGE_DOG) {
		if (m_dDogSigma1 < 0 || m_dDogSigma1 > 5.0)
		{
			if (m_dDogSigma1 < 0)			m_dDogSigma1 = 0;
			else if (m_dDogSigma1 > 5.0)	m_dDogSigma1 = 5.0;
			AfxMessageBox(_T("0에서 5.0 사이의 실수를 입력하십시오."));
		}
		if (m_dDogSigma2 < 0 || m_dDogSigma2 > 5.0)
		{
			if (m_dDogSigma2 < 0)			m_dDogSigma2 = 0;
			else if (m_dDogSigma2 > 5.0)	m_dDogSigma2 = 5.0;
			AfxMessageBox(_T("0에서 5.0 사이의 실수를 입력하십시오."));
		}
		m_ctrlSlider1.SetPos((int)(m_dDogSigma1 * 10));
		m_ctrlSlider2.SetPos((int)(m_dDogSigma2 * 10));
	}
	UpdateData(FALSE);
	DisplayImage();
}

