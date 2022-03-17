#pragma once


// CLoopDlg 대화 상자입니다.

class CLoopDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoopDlg)

public:
	CLoopDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLoopDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOOP_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAdd();
	int m_nLoopCnt;
	afx_msg void OnBnClickedOk();
	bool bAddFlag;
};

typedef struct _MorphTest
{
	int nMorphType;
	int nSEType;
	int nRadius;
	int nHeight;
	int nWidth;

	int nLoopCnt;
}MORPHTEST;