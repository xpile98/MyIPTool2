#pragma once
#include "afxcmn.h"
#include "MyIPTool2Doc.h"
#include "MyIPTool2View.h"
#include "afxwin.h"


// CBrightnessContrastDlg ��ȭ �����Դϴ�.

class CBrightnessContrastDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBrightnessContrastDlg)

public:
	CBrightnessContrastDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CBrightnessContrastDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BRIGHTNESS_CONTRAST_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CMyIPTool2Doc *m_pDoc;
	CMyIPTool2View *m_pView;
	int m_nOffset;
	float m_fGain;
	CSliderCtrl m_slliderGain;
	CSliderCtrl m_sliderOffset;
	CButton m_ctrlCheck16Bit;
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeOffsetEdit();
	afx_msg void OnEnChangeGainEdit();

	void DisplayImage();
	bool bIs16Bit;
};
