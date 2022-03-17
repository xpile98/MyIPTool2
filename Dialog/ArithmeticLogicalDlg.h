#pragma once
#include "afxwin.h"
#include "MyIPTool2Doc.h"
#include "MyIPTool2View.h"

// CArithmeticLogicalDlg 대화 상자입니다.

class CArithmeticLogicalDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CArithmeticLogicalDlg)

public:
	CArithmeticLogicalDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CArithmeticLogicalDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ARITHMETIC_LOGICAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboImage1;
	CComboBox m_comboImage2;
	int m_nFunction;
	CMyIPTool2Doc* m_pDoc1;
	CMyIPTool2Doc* m_pDoc2;
	CMyIPTool2View* m_pView;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
//	int m_ctrlConstant;
	CButton m_UseConstant;
	afx_msg void OnBnClickedCheck();
	CEdit m_ctrlConstant;
	int m_editConstant;

	bool m_bUseConstant;
	CButton m_ctrlCheck16Bit;
	bool bIs16Bit;
	afx_msg void OnEnChangeEditConstant();
	afx_msg void OnCbnSelchangeComboImage1();
	afx_msg void OnCbnSelchangeComboImage2();
	afx_msg void OnPaint();

	bool DisplayPreviewImage();
	bool DisplayResultImage();
	bool bResultFLAG;
	afx_msg void OnBnClickedButtonResult();
};
