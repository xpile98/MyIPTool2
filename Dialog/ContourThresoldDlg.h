#pragma once
#include "afxwin.h"


// ContourThresoldDlg ��ȭ �����Դϴ�.

class ContourThresoldDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ContourThresoldDlg)

public:
	ContourThresoldDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~ContourThresoldDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONTOUR_THRESOLD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_ctrlEditContourThresold;
	int m_nContourThreshold;
};
