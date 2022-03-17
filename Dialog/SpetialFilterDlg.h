#pragma once
#include "afxwin.h"


// CSpetialFilterDlg ��ȭ �����Դϴ�.

class CSpetialFilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSpetialFilterDlg)

public:
	CSpetialFilterDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSpetialFilterDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SPETIAL_FILTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	int m_nWidth;
	int m_nHeight;
	int m_nBorderValue;
	int m_nBorderType;
	CEdit m_ctrlBorderValue;
	afx_msg void OnBnClickedRadioBordertype();
	afx_msg void OnBnClickedRadioGaussianOption();
	CEdit m_ctrlSigmaVal;
	double m_dSigmaVal;
	CEdit m_ctrlNsigma;
	double m_dNsigma;
//	CComboBox m_ctrlComboBox1;
	virtual BOOL OnInitDialog();

	CEdit m_ctrlTitleText;
	CString m_strTitleText;
	int m_nFilterType;
	CEdit m_ctrlHeight;
	CEdit m_ctrlWidth;
	CStatic m_ctrlGaussianOption;
	int m_nGaussianOption;
//	CButton m_ctrlGaussianOptionKernel;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	afx_msg void OnPaint();
	CStatic m_ctrlGrBoxKernel;
	CStatic m_ctrlGrBoxSigma;
	CStatic m_ctrlGaussianOption2;
//	CEdit m_ctrlFilterSize;
	CEdit m_ctrlFilterMaxSize;
//	int m_nFilterSize;
	int m_nFilterMaxSize;

	bool bIsOpencv;
};
