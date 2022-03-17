// HistogramDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "HistogramDlg.h"
#include "afxdialogex.h"

#include "MyIPEnhance.h"

#include "MainFrm.h"
#include "MyIPTool2Doc.h"
#include "MyIPTool2View.h"

// CHistogramDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CHistogramDlg, CDialogEx)

CHistogramDlg::CHistogramDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HISTOGRAM, pParent)
{
	memset(m_Histogram, 0, sizeof(int) * 256);
	m_bIsMouseDown = false;
}

CHistogramDlg::~CHistogramDlg()
{
}

void CHistogramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CHistogramDlg::SetImage(MyIPImage &Img)
{
	if (Img == NULL) {
		memset(m_Histogram, 0, sizeof(int) * 256);
		return;
	}

	int depth = Img.GetDepth();
// 	float histo256[256] = { 0.f, };
// 	float histo65536[65536] = { 0.f, };
	float *histo;
	float max_histo;
	float nhisto[256] = {0,};

	switch (depth)
	{
	case CV_8U:
		//정규화 된 히스토그램 구하기
		histo = new float[256];
		if (!histo) return;
		IP_Histogram(Img, histo);

		//정규화된 히스토그램에서 최댓값을 구한다.
		max_histo = histo[0];
		for (int i = 1; i < 256; i++)
			if (histo[i] > max_histo)	max_histo = histo[i];

		//m_Histogram 배열의 최댓값이 100이 되도록 전체 배열의 값을 조절한다.
		for (int i = 0; i < 256; i++) {
			m_Histogram[i] = static_cast<int>(histo[i] * 200 / max_histo);
			m_HistogramOrg[i] = static_cast<int>(histo[i] * Img.GetWidth()*Img.GetHeight());
		}
		//평균 및 표준편차 구하기.
		IP_Mean_StandardDeviation(Img);
		m_dMean = Img.GetMean();
		m_dStdDest = Img.GetStddDest();
		delete[] histo;
		break;
	case CV_16U:
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
		for (int i = 1; i < 256; i++)
			if (nhisto[i] > max_histo)	max_histo = nhisto[i];

		//m_Histogram 배열의 최댓값이 170이 되도록 전체 배열의 값을 조절한다.
		for (int i = 0; i < 256; i++) {
			m_Histogram[i] = static_cast<int>(nhisto[i] * 200 / max_histo);
			m_HistogramOrg[i] = static_cast<int>(nhisto[i] * Img.GetWidth()*Img.GetHeight());
		}

		//평균 및 표준편차 구하기.
		IP_Mean_StandardDeviation(Img);
		m_dMean = Img.GetMean();
		m_dStdDest = Img.GetStddDest();
		delete[] histo;
		break;
	default:
		memset(m_Histogram, 0, sizeof(int) * 256);
		break;
	}
}

void CHistogramDlg::SetImage(Mat &mat)
{
// 	if (mat.data != NULL) {
// 		if (mat.depth() == CV_8U) {
// 			//정규화 된 히스토그램 구하기
// 			float histo[256] = { 0.f, };
// 			IP_Histogram(mat, histo);
// 
// 			//정규화된 히스토그램에서 최댓값을 구한다.
// 			float max_histo = histo[0];
// 			for (int i = 1; i < 256; i++)
// 				if (histo[i] > max_histo)	max_histo = histo[i];
// 
// 			//m_Histogram 배열의 최댓값이 100이 되도록 전체 배열의 값을 조절한다.
// 			for (int i = 0; i < 256; i++)
// 				m_Histogram[i] = static_cast<int>(histo[i] * 100 / max_histo);
// 		}
// 		else
// 			memset(m_Histogram, 0, sizeof(int) * 256);
// 	}
}


BEGIN_MESSAGE_MAP(CHistogramDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CHistogramDlg 메시지 처리기입니다.


void CHistogramDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.


	CGdiObject* pOldPen = dc.SelectStockObject(DC_PEN);

	//히스토그램 박스
	dc.SetDCPenColor(RGB(128, 128, 128));
	dc.MoveTo(20, 20);
	dc.LineTo(20, 220);
	dc.LineTo(530, 220);
	dc.LineTo(530, 20);

	//각 그레이스케일에 해당하는 히스토그램 출력
	dc.SetDCPenColor(RGB(0, 0, 0));
	for (int i = 0; i < 256; i++) {
// 		dc.MoveTo(20 + i, 120);
// 		dc.LineTo(20 + i, 120 - m_Histogram[i]);
		dc.Rectangle(20 + (i*2), 220, 22 + (i*2), 220 - m_Histogram[i]);
	}

	//그레이스케일 레벨 출력
	for (int i = 0; i < 256; i++) {
		dc.SetDCPenColor(RGB(i, i, i));
// 		dc.MoveTo(20 + i, 130);
// 		dc.LineTo(20 + i, 145);
		dc.Rectangle(20 + (i*2), 230, 22 + (i*2), 245);
	}

	dc.SelectObject(pOldPen);

	CString strPoint;
	dc.SetBkColor(RGB(240, 240, 240));
	strPoint.Format(L"평균:         %.3f", m_dMean);
	dc.TextOut(540, 30, strPoint);
	strPoint.Format(L"표준편차: %.3f", m_dStdDest);
	dc.TextOut(540, 50, strPoint);
}

void CHistogramDlg::OnMouseMove(UINT nFlags, CPoint point)	//190308	좌표당 히스토그램 값 표시 
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CClientDC dc(this);
	CString strPoint;
	int level = point.x - 20;

	if (level >= 0 && level <= 510 && m_bIsMouseDown == true) {
		dc.SetBkColor(RGB(240, 240, 240));
		strPoint.Format(L"그레이스케일: %03d", level/2);
		dc.TextOut(540, 70, strPoint);
		strPoint.Format(L"히스토그램:     %03d", m_HistogramOrg[level / 2]);
		dc.TextOut(540, 90, strPoint);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}



void CHistogramDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (m_bIsMouseDown == false)
		m_bIsMouseDown = true;

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CHistogramDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (m_bIsMouseDown == true)
		m_bIsMouseDown = false;

	CDialogEx::OnLButtonUp(nFlags, point);
}
