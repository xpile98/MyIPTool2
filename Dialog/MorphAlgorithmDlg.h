#pragma once
#include "afxwin.h"
#include "MyIPTool2Doc.h"		//190502 YWCho combobox, m_pDoc

// CMorphAlgorithmDlg ��ȭ �����Դϴ�.

class CMorphAlgorithmDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMorphAlgorithmDlg)

public:
	CMorphAlgorithmDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMorphAlgorithmDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MORPH_ALGORITHM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CMyIPTool2Doc *m_pDoc;

	CComboBox m_comboBox;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
