#pragma once
#include "afxwin.h"


// CFrequencyFilterDlg 대화 상자입니다.

class CFrequencyFilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFrequencyFilterDlg)

public:
	CFrequencyFilterDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFrequencyFilterDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FREQ_FILTER_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboFilterType();
	afx_msg void OnCbnSelchangeComboFilterShape();

	int m_nCOFval;
	int m_nBPFval;
	int m_nBUTval;
	int m_nFilterType;
	int m_nFilterShape;
	BOOL m_bDoScale;
	BOOL m_bAddAvr;
	virtual BOOL OnInitDialog();
};
