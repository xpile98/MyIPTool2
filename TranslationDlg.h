#pragma once


// TranslationDlg ��ȭ �����Դϴ�.

class TranslationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(TranslationDlg)

public:
	TranslationDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~TranslationDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRANSLATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
