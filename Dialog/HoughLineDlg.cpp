// HoughLineDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "HoughLineDlg.h"
#include "afxdialogex.h"

#include "MyIPEdge.h"		//canny edge

// CHoughLineDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CHoughLineDlg, CDialogEx)

CHoughLineDlg::CHoughLineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HOUGH_LINE_DLG, pParent)
	, m_dTheta(1.0) , m_dRho(1.0)
	, m_nMaskHeight(10) , m_nMaskWidth(10)
	, m_nLowTh(10), m_nHighTh(200), m_nHoughTh(60)
{

}

CHoughLineDlg::~CHoughLineDlg()
{
}

void CHoughLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOW_THRESHOLD, m_sliderLowTh);
	DDX_Control(pDX, IDC_HIGH_THRESHOLD, m_sliderHighTh);
	DDX_Control(pDX, IDC_HOUGH_THRESHOLD, m_sliderHoughTh);
	DDX_Text(pDX, IDC_EDIT_THETA, m_dTheta);
	DDV_MinMaxDouble(pDX, m_dTheta, 0.0, 5.0);
	DDX_Text(pDX, IDC_EDIT_RHO, m_dRho);
	DDV_MinMaxDouble(pDX, m_dRho, 0.0, 10.0);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_nMaskHeight);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_nMaskWidth);
}


BEGIN_MESSAGE_MAP(CHoughLineDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CHoughLineDlg 메시지 처리기입니다.



BOOL CHoughLineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_sliderLowTh.SetRange(0, 255);
	m_sliderLowTh.SetTicFreq(32);
	m_sliderLowTh.SetPageSize(32);
	m_sliderLowTh.SetPos(10);

	m_sliderHighTh.SetRange(0, 255);
	m_sliderHighTh.SetTicFreq(32);
	m_sliderHighTh.SetPageSize(32);
	m_sliderHighTh.SetPos(200);

	m_sliderHoughTh.SetRange(0, 255);
	m_sliderHoughTh.SetTicFreq(32);
	m_sliderHoughTh.SetPageSize(32);
	m_sliderHoughTh.SetPos(100);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CHoughLineDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (m_sliderLowTh.GetSafeHwnd() == pScrollBar->GetSafeHwnd())	//지금 선택된 스크롤바 번호랑 m_sliderOffset슬라이더랑 같냐?
	{
		m_nLowTh = m_sliderLowTh.GetPos();
		UpdateData(false);	//변수값을 컨트롤에 써줘라~ 
		DisplayImage();
	}

	else if(m_sliderHighTh.GetSafeHwnd() == pScrollBar->GetSafeHwnd())	//지금 선택된 스크롤바 번호랑 m_sliderOffset슬라이더랑 같냐?
	{
		m_nHighTh = m_sliderHighTh.GetPos();
		UpdateData(false);	//변수값을 컨트롤에 써줘라~ 
		DisplayImage();
	}

	else if (m_sliderHoughTh.GetSafeHwnd() == pScrollBar->GetSafeHwnd())	//지금 선택된 스크롤바 번호랑 m_sliderOffset슬라이더랑 같냐?
	{
		m_nHoughTh = m_sliderHoughTh.GetPos();
		UpdateData(false);	//변수값을 컨트롤에 써줘라~ 
		DisplayImage();
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CHoughLineDlg::DisplayImage()
{
	MyIPImage img;
	img = m_pDoc->m_MyIPImage;
	if (IpCanny(img, m_nLowTh, m_nHighTh)) {
		CClientDC dc(m_pView);	//내가 view줄게 dc가져왕!	view의 dc(그림그리는 툴)을 가져오자 
		img.Draw(dc.m_hDC, 0, 0);
	}
}
