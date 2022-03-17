#pragma once
#include "afxcmn.h"

#include "MyIPTool2Doc.h"
#include "MyIPTool2View.h"
#include "afxwin.h"

// CThresholdDlg ��ȭ �����Դϴ�.

class CThresholdDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CThresholdDlg)

public:
	CThresholdDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CThresholdDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_THRESHOLD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CMyIPTool2Doc *m_pDoc;
	CMyIPTool2View *m_pView;
	CSliderCtrl m_sliderThreshold;
	CButton m_ctrlCheck16Bit;
	int m_nHistogram;
	int m_nThreshold;
	int m_nAutoThresholdingVal;		//�ʱⰪ �����ϱ� ���� ���� 

	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeThresholdEdit();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeHistogramEdit();
	afx_msg void OnBnClickedCheck16bit();
	afx_msg void OnPaint();


	void DisplayImage();
	void SetImage(MyIPImage &Img);	//8UC1�Ϸ� . 16UC1 Ȯ����
	int m_Histogram[256];
	unsigned int *m_HistogramOrg;
	int m_nOldPos;

	bool bIs16Bit;
};
