#pragma once
#include "afxwin.h"


// CShapeningFilterDlg 대화 상자입니다.

class CShapeningFilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CShapeningFilterDlg)

public:
	CShapeningFilterDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CShapeningFilterDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SARPENING_FILTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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