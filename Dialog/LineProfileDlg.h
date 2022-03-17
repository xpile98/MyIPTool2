#pragma once

// CLineProfileDlg 대화 상자입니다.

class CLineProfileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLineProfileDlg)

public:
	CLineProfileDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLineProfileDlg();
	Point start, end;
	MyIPImage *m_pImage;
	CWnd *m_pWnd;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LINE_PROFILE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
