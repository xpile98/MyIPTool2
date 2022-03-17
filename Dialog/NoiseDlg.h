#pragma once
#include "afxwin.h"


// CNoiseDlg ��ȭ �����Դϴ�.

class CNoiseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNoiseDlg)

public:
	CNoiseDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CNoiseDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NOISE_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	int m_nRadioNoise;
	CEdit m_ctrlMean;
	CEdit m_ctrlSigma;
	CEdit m_ctrlSalt;
	CEdit m_ctrlPepper;
	double m_dMean;
	double m_dSigma;
	double m_dSalt;
	double m_dPepper;
	afx_msg void OnBnClickedRadio();
	virtual BOOL OnInitDialog();
	CButton m_ctrlRGB;
	BOOL m_bCheckRGB;
};
