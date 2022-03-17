#pragma once


// CHistogramDlg ��ȭ �����Դϴ�.

class CHistogramDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistogramDlg)

public:
	int m_Histogram[256];
	int m_HistogramOrg[256];
	CHistogramDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CHistogramDlg();
private:
	bool m_bIsMouseDown;
	double m_dMean;
	double m_dStdDest;
	
// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HISTOGRAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	DECLARE_MESSAGE_MAP()

public:
	void SetImage(MyIPImage &Img);	//8UC1�Ϸ� . 16UC1 Ȯ����
	void SetImage(Mat &pMat);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
