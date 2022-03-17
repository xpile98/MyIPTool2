#pragma once


// CLoopDlg ��ȭ �����Դϴ�.

class CLoopDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoopDlg)

public:
	CLoopDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CLoopDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOOP_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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