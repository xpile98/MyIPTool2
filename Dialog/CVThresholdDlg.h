#pragma once


// CCVThresholdDlg ��ȭ �����Դϴ�.

class CCVThresholdDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCVThresholdDlg)

public:
	CCVThresholdDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCVThresholdDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_THRESHOLD_CV };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	int m_nType;
	int m_nMethod;
	virtual void OnOK();
	afx_msg void OnBnClickedMethod();
	afx_msg void OnClose();
};
