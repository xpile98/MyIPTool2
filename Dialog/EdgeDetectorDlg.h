#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "MyIPTool2Doc.h"
#include "MyIPTool2View.h"


// CEdgeDetectorDlg ��ȭ �����Դϴ�.

class CEdgeDetectorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEdgeDetectorDlg)

public:
	CEdgeDetectorDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CEdgeDetectorDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDGE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bNormalize;

	int m_nEdgeType;
	int m_nLowTh;
	int m_nHighTh;

	double m_dLogSigma;
	double m_dDogSigma1;
	double m_dDogSigma2;

	CEdit m_ctrlLowTh;
	CEdit m_ctrlHighTh;
	CEdit m_ctrlLogSigma;
	CEdit m_ctrlDogSigma1;
	CEdit m_ctrlDogSigma2;

	CSliderCtrl m_ctrlSlider1;
	CSliderCtrl m_ctrlSlider2;

	CMyIPTool2Doc *m_pDoc;		//for DisplayImage Function
	CMyIPTool2View *m_pView;	//for DisplayImage Function

	afx_msg void OnBnClickedRadioEdge();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeEdit();
	virtual BOOL OnInitDialog();

	void DisplayImage();

};
