#pragma once


// CHistogramDlg 대화 상자입니다.

class CHistogramDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistogramDlg)

public:
	int m_Histogram[256];
	int m_HistogramOrg[256];
	CHistogramDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CHistogramDlg();
private:
	bool m_bIsMouseDown;
	double m_dMean;
	double m_dStdDest;
	
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HISTOGRAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	void SetImage(MyIPImage &Img);	//8UC1완료 . 16UC1 확인필
	void SetImage(Mat &pMat);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
