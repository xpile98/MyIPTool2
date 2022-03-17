#pragma once


// TranslationDlg 대화 상자입니다.

class TranslationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(TranslationDlg)

public:
	TranslationDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~TranslationDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRANSLATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_nShiftX;
	int m_nShiftY;
	int m_nRoiX;
	int m_nRoiY;
	int m_nRoiW;
	int m_nRoiH;
	BOOL m_bRoiMode;
	afx_msg void OnBnClickedCheck1();
	virtual BOOL OnInitDialog();
};
