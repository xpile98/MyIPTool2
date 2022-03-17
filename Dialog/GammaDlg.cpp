// Gamma.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "GammaDlg.h"
#include "afxdialogex.h"

#include "MyIPConvert.h"
#include "MyIPEnhance.h"

// CGammaDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGammaDlg, CDialogEx)

CGammaDlg::CGammaDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GAMMA_DLG, pParent)
	, m_fGamma(2.2f), bIs16Bit(false)
{

}

CGammaDlg::~CGammaDlg()
{
}

void CGammaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_GAMMA_EDIT, m_fGamma);
	DDV_MinMaxFloat(pDX, m_fGamma, 0.20f, 5.00f);
	DDX_Control(pDX, IDC_GAMMA_SLIDER, m_sliderGamma);
	DDX_Control(pDX, IDC_CHECK_16BIT, m_ctrlCheck16Bit);
}


BEGIN_MESSAGE_MAP(CGammaDlg, CDialogEx)
	ON_EN_CHANGE(IDC_GAMMA_EDIT, &CGammaDlg::OnEnChangeGammaEdit)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CGammaDlg 메시지 처리기입니다.


BOOL CGammaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	if (bIs16Bit == false)		//16비트 구분
		m_ctrlCheck16Bit.SetCheck(false);
	else
		m_ctrlCheck16Bit.SetCheck(true);
	m_ctrlCheck16Bit.EnableWindow(false);


	m_sliderGamma.SetRange(10, 250); //감마값이 0.2~5.0 이므로 50을 곱한값으로 범위를 지정
	m_sliderGamma.SetTicFreq(20);
	m_sliderGamma.SetPageSize(20);
	m_sliderGamma.SetPos(static_cast<int>(m_fGamma * 50));	//static cast를 쓰면 캐스팅(형변환) 실패시 컴파일 에러를 발생시킴


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CGammaDlg::OnEnChangeGammaEdit()
{
	UpdateData(TRUE);
	m_sliderGamma.SetPos(static_cast<int>(m_fGamma * 50));
	DisplayImage();
}


void CGammaDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_sliderGamma.GetSafeHwnd() == pScrollBar->GetSafeHwnd()) {
		int pos = m_sliderGamma.GetPos();
		m_fGamma = pos / 50.f;
		UpdateData(FALSE);
		DisplayImage();
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CGammaDlg::DisplayImage()
{
#ifdef USE_IPIMAGE
	MyIPImage img;
	img = m_pDoc->m_MyIPImage;
	if (IP_GammaCorrection(img, m_fGamma)) {
		CClientDC dc(m_pView);	//내가 view줄게 dc가져왕!	view의 dc(그림그리는 툴)을 가져오자 
		img.Draw(dc.m_hDC, 0, 0);
	}
#else
	MyIPImage img;
	if (IpMatToImage(m_pDoc->m_Mat, img)) {
		if (IP_GammaCorrection(img, m_fGamma)) {
			CClientDC dc(m_pView);	//내가 view줄게 dc가져왕!	view의 dc(그림그리는 툴)을 가져오자 
			img.Draw(dc.m_hDC, 0, 0);
		}
	}
#endif
}
