#pragma once
#include "afxcmn.h"
#include "MyIPTool2Doc.h"
#include "MyIPTool2View.h"


// CHoughLineDlg 대화 상자입니다.

class CHoughLineDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHoughLineDlg)

public:
	CHoughLineDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CHoughLineDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HOUGH_LINE_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_sliderLowTh;		int m_nLowTh;
	CSliderCtrl m_sliderHighTh;		int m_nHighTh;
	CSliderCtrl m_sliderHoughTh;	int m_nHoughTh;
	double m_dTheta;		// 각도 간격
	double m_dRho;			// 거리간격
	int m_nMaskHeight;
	int m_nMaskWidth;

	CMyIPTool2Doc *m_pDoc;
	CMyIPTool2View *m_pView; 
	void DisplayImage();

	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
