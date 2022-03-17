#pragma once
#include "afxwin.h"
#include "MyIPTool2Doc.h"		//190513 YWCho combobox, m_pDoc


// CPatternMatchingDlg 대화 상자입니다.

class CPatternMatchingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPatternMatchingDlg)

public:
	CPatternMatchingDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPatternMatchingDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATTERNMATCHING_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CMyIPTool2Doc *m_pDoc;

	CComboBox m_PatternImage;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	int m_nObjects;
	float m_fNCCThreshold;
	int m_ctrlRadioPmMode;
	afx_msg void OnBnClickedRadioPmMode(UINT msg);
};
