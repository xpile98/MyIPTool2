#pragma once
#include "afxwin.h"
#include "MyIPTool2Doc.h"	//190430 YWCho Conditional Dialation
#include "MyIPTool2View.h"


// CMorphologyDlg 대화 상자입니다.

class CMorphologyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMorphologyDlg)

public:
	CMorphologyDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMorphologyDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MORPHOLOGY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_nMorphologyType;
	int m_nSEType;
	int m_nRadius;
	int m_nRectHeight;
	int m_nRectWidth;

	afx_msg void OnBnClickedRadioMorphType();
	afx_msg void OnBnClickedRadioSeType();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	
	CComboBox m_ctrlComboBox;
	CMyIPTool2Doc* m_pDoc1;

	bool bGrayMorphFlag;
};
