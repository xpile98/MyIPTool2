#pragma once
#include "afxwin.h"


// CShapeningFilterDlg ��ȭ �����Դϴ�.

class CShapeningFilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CShapeningFilterDlg)

public:
	CShapeningFilterDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CShapeningFilterDlg();

	// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SARPENING_FILTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_ctrlSigma;
	double m_dSigma;
	CEdit m_ctrlWeight;
	double m_dWeight;
	int m_nFilterType;
	CButton m_ctrlCheckUnsharpMasking;
	CEdit m_ctrlTitleText;
	CString m_strTitleText;
	int m_nLaplacianType;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckUnsharpMasking();
	afx_msg void OnBnClickedRadioLaplacian();
};