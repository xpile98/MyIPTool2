// BrightnessContrast.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "BrightnessContrastDlg.h"
#include "afxdialogex.h"

#include "MyIPConvert.h"
#include "MyIPEnhance.h"

// CBrightnessContrastDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CBrightnessContrastDlg, CDialogEx)

CBrightnessContrastDlg::CBrightnessContrastDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BRIGHTNESS_CONTRAST_DLG, pParent)
	, m_nOffset(0), m_fGain(1.), bIs16Bit(false)
{

}

CBrightnessContrastDlg::~CBrightnessContrastDlg()
{
}

void CBrightnessContrastDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_OFFSET_EDIT, m_nOffset);
	//DDV_MinMaxInt(pDX, m_nOffset, -255, 255);	//-> event 함수에서 처리 
	DDX_Text(pDX, IDC_GAIN_EDIT, m_fGain);
	DDV_MinMaxFloat(pDX, m_fGain, -100, 100);
	DDX_Control(pDX, IDC_GAIN_SLIDER, m_slliderGain);
	DDX_Control(pDX, IDC_OFFSET_SLIDER, m_sliderOffset);
	DDX_Control(pDX, IDC_CHECK_16BIT, m_ctrlCheck16Bit);
}


BEGIN_MESSAGE_MAP(CBrightnessContrastDlg, CDialogEx)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_OFFSET_EDIT, &CBrightnessContrastDlg::OnEnChangeOffsetEdit)
	ON_EN_CHANGE(IDC_GAIN_EDIT, &CBrightnessContrastDlg::OnEnChangeGainEdit)
END_MESSAGE_MAP()


// CBrightnessContrastDlg 메시지 처리기입니다.


BOOL CBrightnessContrastDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	//밝기 조정 슬라이더 초기화
	if (bIs16Bit == false) {
		m_ctrlCheck16Bit.SetCheck(false);
		m_sliderOffset.SetRange(-255, 255);
		m_sliderOffset.SetTicFreq(32);
		m_sliderOffset.SetPageSize(32);
	}
	else {		//16비트 
		m_ctrlCheck16Bit.SetCheck(true);
		m_sliderOffset.SetRange(-65535, 65535);
		m_sliderOffset.SetTicFreq(8192);
		m_sliderOffset.SetPageSize(8192);
	}
	m_ctrlCheck16Bit.EnableWindow(FALSE);

	//명암비 조절 슬라이더 초기화
	m_slliderGain.SetRange(-100, 100);
	m_slliderGain.SetTicFreq(20);
	m_slliderGain.SetPageSize(20);
	m_slliderGain.SetPos(10);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CBrightnessContrastDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_sliderOffset.GetSafeHwnd() == pScrollBar->GetSafeHwnd())	//지금 선택된 스크롤바 번호랑 m_sliderOffset슬라이더랑 같냐?
	{
		m_nOffset = m_sliderOffset.GetPos();
		UpdateData(false);	//변수값을 컨트롤에 써줘라~ 
		DisplayImage();
	}

	else if (m_slliderGain.GetSafeHwnd() == pScrollBar->GetSafeHwnd())	//지금 선택된 스크롤바 번호랑 m_sliderOffset슬라이더랑 같냐?
	{
		m_fGain = m_slliderGain.GetPos()*0.1f;
		UpdateData(false);	//변수값을 컨트롤에 써줘라~ 
		DisplayImage();
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CBrightnessContrastDlg::OnEnChangeOffsetEdit()
{
	UpdateData(true);	//쓴 값을 읽어와라 

	if (bIs16Bit == false) {
		if (m_nOffset < -255 || m_nOffset > 255) {
			if (m_nOffset < -255)		m_nOffset = -255;
			else if (m_nOffset > 255)	m_nOffset = 255;
			AfxMessageBox(_T("-255에서 255 사이의 정수를 입력하십시오."));
		}
	}

	else {
		if (m_nOffset < -65535 || m_nOffset > 65535) {
			if (m_nOffset < -65535)		m_nOffset = -65535;
			else if (m_nOffset > 65535)	m_nOffset = 65535;
			AfxMessageBox(_T("-65535에서 65535 사이의 정수를 입력하십시오."));
		}
	}
	m_sliderOffset.SetPos(m_nOffset);
	DisplayImage();
}


void CBrightnessContrastDlg::OnEnChangeGainEdit()
{
	UpdateData(true);	//쓴 값을 읽어와라 
	m_slliderGain.SetPos((int)(m_fGain*10.f));
	DisplayImage();
}

void CBrightnessContrastDlg::DisplayImage()
{
#ifdef USE_IPIMAGE	//190315 YWCho mat -> image
	MyIPImage img;
	img = m_pDoc->m_MyIPImage;
		if (IP_BrightnessContrast(img, m_nOffset, m_fGain)) {
			CClientDC dc(m_pView);	//내가 view줄게 dc가져왕!	view의 dc(그림그리는 툴)을 가져오자 
			img.Draw(dc.m_hDC, 0, 0);
		}
#else
	MyIPImage img;
	if (IpMatToImage(m_pDoc->m_Mat, img)) {
		if (IP_BrightnessContrast(img, m_nOffset, m_fGain)) {
			CClientDC dc(m_pView);	//내가 view줄게 dc가져왕!	view의 dc(그림그리는 툴)을 가져오자 
			img.Draw(dc.m_hDC, 0, 0);
		}
	}
#endif
}
