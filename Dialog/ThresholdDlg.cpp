// ThresholdDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "ThresholdDlg.h"
#include "afxdialogex.h"

#include "MyIPConvert.h"
#include "MyIPEnhance.h"

// CThresholdDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CThresholdDlg, CDialogEx)

CThresholdDlg::CThresholdDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_THRESHOLD, pParent)
	, m_nThreshold(128), m_nHistogram(0), m_nOldPos(0), m_nAutoThresholdingVal(-1)
	, bIs16Bit(false)
{

}

CThresholdDlg::~CThresholdDlg()
{
	delete[] m_HistogramOrg;
}

void CThresholdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_THRESHOLD_SLIDER, m_sliderThreshold);
	//  DDX_Text(pDX, IDC_THRESHOLD_EDIT, m_byteThreshold);
	//  DDV_MinMaxByte(pDX, m_byteThreshold, 0, 65535);
	DDX_Text(pDX, IDC_HISTOGRAM_EDIT, m_nHistogram);
	//  DDX_Check(pDX, IDC_CHECK_16BIT, m_bCheck16Bit);
	DDX_Control(pDX, IDC_CHECK_16BIT, m_ctrlCheck16Bit);
	DDX_Text(pDX, IDC_THRESHOLD_EDIT, m_nThreshold);
	DDV_MinMaxInt(pDX, m_nThreshold, 0, 65535);
}


BEGIN_MESSAGE_MAP(CThresholdDlg, CDialogEx)
	ON_EN_CHANGE(IDC_THRESHOLD_EDIT, &CThresholdDlg::OnEnChangeThresholdEdit)
	ON_EN_CHANGE(IDC_HISTOGRAM_EDIT, &CThresholdDlg::OnEnChangeHistogramEdit)
	ON_WM_HSCROLL()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_16BIT, &CThresholdDlg::OnBnClickedCheck16bit)
END_MESSAGE_MAP()


// CThresholdDlg 메시지 처리기입니다.


BOOL CThresholdDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	if (bIs16Bit == true) {
		m_HistogramOrg = new unsigned int[65536];		//히스토그램 저장용 배열 동적할당 
		m_ctrlCheck16Bit.SetCheck(true);		//16비트 체크박스 체크

		if (m_nAutoThresholdingVal == -1)	//초기값이 지정되지 않으면 127로~ 
			m_nAutoThresholdingVal = 32768;

		m_sliderThreshold.SetRange(0, 65535);
		m_sliderThreshold.SetTicFreq(256);
		m_sliderThreshold.SetPageSize(256);
		m_sliderThreshold.SetPos(m_nAutoThresholdingVal);	//static cast를 쓰면 캐스팅(형변환) 실패시 컴파일 에러를 발생시킴
	}
	else {
		m_HistogramOrg = new unsigned int[256];		//히스토그램 저장용 배열 동적할당 
		m_ctrlCheck16Bit.SetCheck(false);

		if (m_nAutoThresholdingVal == -1)	//초기값이 지정되지 않으면 127로~ 
			m_nAutoThresholdingVal = 128;

		m_sliderThreshold.SetRange(0, 255); 
		m_sliderThreshold.SetTicFreq(32);
		m_sliderThreshold.SetPageSize(32);
		m_sliderThreshold.SetPos(m_nAutoThresholdingVal);	//static cast를 쓰면 캐스팅(형변환) 실패시 컴파일 에러를 발생시킴
	}
	m_ctrlCheck16Bit.EnableWindow(FALSE);		//체크박스 비활성화

	IP_Histogram(m_pDoc->m_MyIPImage, m_HistogramOrg);
	m_nThreshold = m_nAutoThresholdingVal;
	m_nHistogram = m_HistogramOrg[m_nAutoThresholdingVal];	//초기값 세팅

	DisplayImage();
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CThresholdDlg::DisplayImage()
{
#ifdef USE_IPIMAGE
	MyIPImage img;
	img = m_pDoc->m_MyIPImage;
	if (IP_Threshold(img, m_nThreshold)) {
		CClientDC dc(m_pView);	//내가 view줄게 dc가져왕!	view의 dc(그림그리는 툴)을 가져오자 
		img.Draw(dc.m_hDC, 0, 0);
	}
#else
	MyIPImage img;
	if (IpMatToImage(m_pDoc->m_Mat, img)) {
		if (IP_Threshold(img, m_byteThreshold)) {
			CClientDC dc(m_pView);	//내가 view줄게 dc가져왕!	view의 dc(그림그리는 툴)을 가져오자 
			img.Draw(dc.m_hDC, 0, 0);
		}
	}
#endif
}

void CThresholdDlg::SetImage(MyIPImage & Img)
{
	if (Img == NULL) {
		memset(m_Histogram, 0, sizeof(int) * 256);
		return;
	}

	int depth = Img.GetDepth();
	float *histo;
	float max_histo;
	float nhisto[256] = { 0, };

	switch (depth)
	{
	case CV_8U:
		//정규화 된 히스토그램 구하기
		histo = new float[256];
		if (!histo) return;
		IP_Histogram(Img, histo);

		//정규화된 히스토그램에서 최댓값을 구한다.
		max_histo = histo[0];
		for (int i = 1; i < 256; i++) {
			if (histo[i] > max_histo)	max_histo = histo[i];
			//m_HistogramOrg[i] = static_cast<int>(histo[i] * Img.GetWidth()*Img.GetHeight());
		}

		//m_Histogram 배열의 최댓값이 170이 되도록 전체 배열의 값을 조절한다.
		for (int i = 0; i < 256; i++) 
			m_Histogram[i] = static_cast<int>(histo[i] * 170 / max_histo);

// 		//평균 및 표준편차 구하기.
// 		IP_Mean_StandardDeviation(Img);
// 		m_dMean = Img.GetMean();
// 		m_dStdDest = Img.GetStddDest();

		delete[] histo;
		break;
	case CV_16U:
		bIs16Bit = true;
		//정규화 된 히스토그램 구하기
		histo = new float[65536];
		if (!histo) return;
		IP_Histogram(Img, histo);

		//(16비트) 0~255의 히스토그램 값을 nhisto[0]에 저장하고..그런식
		for (int i = 0; i < 65536; i++) {
			nhisto[i / 256] += histo[i];
		}

		//정규화된 히스토그램에서 최댓값을 구한다.
		max_histo = nhisto[0];
		for (int i = 1; i < 256; i++) {
			if (nhisto[i] > max_histo)	max_histo = nhisto[i];
			//m_HistogramOrg[i] = static_cast<int>(nhisto[i] * Img.GetWidth()*Img.GetHeight());
		}

		//m_Histogram 배열의 최댓값이 170이 되도록 전체 배열의 값을 조절한다.
		for (int i = 0; i < 256; i++) {
			m_Histogram[i] = static_cast<int>(nhisto[i] * 170 / max_histo);
		}
		delete[] histo;
		break;
	default:
		memset(m_Histogram, 0, sizeof(int) * 256);
		break;
	}
}

void CThresholdDlg::OnEnChangeThresholdEdit()
{
	UpdateData(TRUE);
	if (bIs16Bit == false) {
		if (m_nThreshold > 255) {
			m_nThreshold = 255;
			AfxMessageBox(_T("0에서 255 사이의 정수를 입력하십시오."));
		}
	}
	else {
		if (m_nThreshold > 65535)
			m_nThreshold = 65535;
	}

	int pos = 0;
	m_sliderThreshold.SetPos(m_nThreshold);
	if (bIs16Bit == false) {
		m_nHistogram = m_HistogramOrg[m_nThreshold];
		pos = m_nThreshold;
	}
	else {
		m_nHistogram = m_HistogramOrg[m_nThreshold];
		pos = m_nThreshold / 256;
	}


	CClientDC dc(this);
	CGdiObject* pOldPen = dc.SelectStockObject(DC_PEN);

	if (m_nOldPos != pos) {
		dc.SetDCPenColor(RGB(240, 240, 240));
		dc.MoveTo(20 + m_nOldPos, 30);
		dc.LineTo(20 + m_nOldPos, 200);
		dc.SetDCPenColor(RGB(0, 0, 0));
		dc.MoveTo(20 + m_nOldPos, 200);
		dc.LineTo(20 + m_nOldPos, 200 - m_Histogram[m_nOldPos]);
	}

	dc.SetDCPenColor(RGB(255, 0, 0));
	dc.MoveTo(20 + pos, 30);
	dc.LineTo(20 + pos, 200);
	m_nOldPos = pos;

	dc.SelectObject(pOldPen);


	UpdateData(false);
	DisplayImage();
}

void CThresholdDlg::OnEnChangeHistogramEdit()
{
	//히스토그램 컨트롤박스 값 변경되면 호출되는 함수 
}

void CThresholdDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_sliderThreshold.GetSafeHwnd() == pScrollBar->GetSafeHwnd()) {

		int pos = m_sliderThreshold.GetPos();
		m_nThreshold = pos;

		if (bIs16Bit == true)	//16비트 적용 
			pos /= 256;
		m_nHistogram = m_HistogramOrg[pos];

		CClientDC dc(this);
		CGdiObject* pOldPen = dc.SelectStockObject(DC_PEN);

		if (m_nOldPos != pos) {
			dc.SetDCPenColor(RGB(240, 240, 240));
			dc.MoveTo(20 + m_nOldPos, 30);
			dc.LineTo(20 + m_nOldPos, 200);
			dc.SetDCPenColor(RGB(0, 0, 0));
			dc.MoveTo(20 + m_nOldPos, 200);
			dc.LineTo(20 + m_nOldPos, 200 - m_Histogram[m_nOldPos]);
		}

		dc.SetDCPenColor(RGB(255, 0, 0));
		dc.MoveTo(20+pos, 30);
		dc.LineTo(20+pos, 200);
		m_nOldPos = pos;

		dc.SelectObject(pOldPen);

		UpdateData(FALSE);
		DisplayImage();
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}




void CThresholdDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.


	CGdiObject* pOldPen = dc.SelectStockObject(DC_PEN);

	//히스토그램 박스
	dc.SetDCPenColor(RGB(128, 128, 128));
	dc.MoveTo(19, 29);
	dc.LineTo(19, 200);
	dc.LineTo(276, 200);
	dc.LineTo(276, 29);
	dc.LineTo(19, 29);

	//각 그레이스케일에 해당하는 히스토그램 출력
	dc.SetDCPenColor(RGB(0, 0, 0));
	for (int i = 0; i < 256; i++) {
		dc.MoveTo(20 + i, 200);
		dc.LineTo(20 + i, 200 - m_Histogram[i]);
		//dc.Rectangle(20 + (i * 2), 220, 22 + (i * 2), 220 - m_Histogram[i]);
	}

	//>>시작 빨간줄 표시
	dc.SetDCPenColor(RGB(255, 0, 0));
	if (bIs16Bit == true) {
		dc.MoveTo(20 + m_nAutoThresholdingVal/256, 30);
		dc.LineTo(20 + m_nAutoThresholdingVal/256, 200);
		m_nOldPos = m_nAutoThresholdingVal/256;
	}
	else {
		dc.MoveTo(20 + m_nAutoThresholdingVal, 30);
		dc.LineTo(20 + m_nAutoThresholdingVal, 200);
		m_nOldPos = m_nAutoThresholdingVal;
	}
	//<<

// 		//그레이스케일 레벨 출력
// 		for (int i = 0; i < 256; i++) {
// 			dc.SetDCPenColor(RGB(i, i, i));
// 			dc.MoveTo(20 + i, 130);
// 			dc.LineTo(20 + i, 145);
// 			//dc.Rectangle(20 + (i * 2), 230, 22 + (i * 2), 245);
// 		}

	dc.SelectObject(pOldPen);
}


void CThresholdDlg::OnBnClickedCheck16bit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_ctrlCheck16Bit.GetCheck()==false) {
	}

	else {
	}

	UpdateData(false);
}
