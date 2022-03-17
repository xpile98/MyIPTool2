#pragma once
#include "afxwin.h"
#include "MyIPTool2Doc.h"		//190502 YWCho combobox, m_pDoc

// CMorphAlgorithmDlg 대화 상자입니다.

class CMorphAlgorithmDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMorphAlgorithmDlg)

public:
	CMorphAlgorithmDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMorphAlgorithmDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MORPH_ALGORITHM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CMyIPTool2Doc *m_pDoc;

	CComboBox m_comboBox;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
