#pragma once


// CCVThresholdDlg 대화 상자입니다.

class CCVThresholdDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCVThresholdDlg)

public:
	CCVThresholdDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCVThresholdDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_THRESHOLD_CV };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_nType;
	int m_nMethod;
	virtual void OnOK();
	afx_msg void OnBnClickedMethod();
	afx_msg void OnClose();
};
