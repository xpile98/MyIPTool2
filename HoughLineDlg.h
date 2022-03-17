#pragma once
#include "afxcmn.h"
#include "MyIPTool2Doc.h"
#include "MyIPTool2View.h"


// CHoughLineDlg ��ȭ �����Դϴ�.

class CHoughLineDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHoughLineDlg)

public:
	CHoughLineDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CHoughLineDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HOUGH_LINE_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_sliderLowTh;		int m_nLowTh;
	CSliderCtrl m_sliderHighTh;		int m_nHighTh;
	CSliderCtrl m_sliderHoughTh;	int m_nHoughTh;
	double m_dTheta;		// ���� ����
	double m_dRho;			// �Ÿ�����
	int m_nMaskHeight;
	int m_nMaskWidth;

	CMyIPTool2Doc *m_pDoc;
	CMyIPTool2View *m_pView; 
	void DisplayImage();

	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
