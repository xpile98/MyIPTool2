#pragma once
#include "afxwin.h"


// CFrequencyFilterDlg ��ȭ �����Դϴ�.

class CFrequencyFilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFrequencyFilterDlg)

public:
	CFrequencyFilterDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CFrequencyFilterDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FREQ_FILTER_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
