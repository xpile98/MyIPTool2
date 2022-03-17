#pragma once
#include "afxwin.h"
#include "MyIPTool2Doc.h"	//190430 YWCho Conditional Dialation
#include "MyIPTool2View.h"


// CMorphologyDlg ��ȭ �����Դϴ�.

class CMorphologyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMorphologyDlg)

public:
	CMorphologyDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMorphologyDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MORPHOLOGY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
