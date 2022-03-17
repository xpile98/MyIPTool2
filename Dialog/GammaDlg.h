#pragma once
#include "afxcmn.h"

#include "MyIPTool2Doc.h"
#include "MyIPTool2View.h"
#include "afxwin.h"

// CGammaDlg 대화 상자입니다.

class CGammaDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGammaDlg)

public:
	CGammaDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGammaDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAMMA_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CMyIPTool2Doc *m_pDoc;
	CMyIPTool2View *m_pView;
	// float 0.20~5.00
	float m_fGamma;
	CSliderCtrl m_sliderGamma;
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeGammaEdit();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void DisplayImage();
	CButton m_ctrlCheck16Bit;
	bool bIs16Bit;
};
