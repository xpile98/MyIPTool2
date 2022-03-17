#pragma once
#include "afxwin.h"
#include "MyIPTool2Doc.h"		//190513 YWCho combobox, m_pDoc


// CPatternMatchingDlg ��ȭ �����Դϴ�.

class CPatternMatchingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPatternMatchingDlg)

public:
	CPatternMatchingDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPatternMatchingDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATTERNMATCHING_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
