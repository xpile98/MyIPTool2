#pragma once
#include "afxwin.h"


// ContourThresoldDlg 대화 상자입니다.

class ContourThresoldDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ContourThresoldDlg)

public:
	ContourThresoldDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~ContourThresoldDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONTOUR_THRESOLD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_ctrlEditContourThresold;
	int m_nContourThreshold;
};
