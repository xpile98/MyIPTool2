#pragma once

// CLineProfileDlg ��ȭ �����Դϴ�.

class CLineProfileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLineProfileDlg)

public:
	CLineProfileDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CLineProfileDlg();
	Point start, end;
	MyIPImage *m_pImage;
	CWnd *m_pWnd;

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LINE_PROFILE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

protected:
	bool bisStartSelected;

public:
	//CMyIPTool2View m_pView;

	void SetStartPoint(int x, int y);
	void SetEndPoint(int x, int y);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();

};
