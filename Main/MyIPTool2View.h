
// MyIPTool2View.h : CMyIPTool2View Ŭ������ �������̽�
//

#pragma once
#include "LineProfileDlg.h"

class CMyIPTool2View : public CScrollView
{
protected: // serialization������ ��������ϴ�.
	CMyIPTool2View();
	DECLARE_DYNCREATE(CMyIPTool2View)

// Ư���Դϴ�.
public:
	CMyIPTool2Doc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CMyIPTool2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLineProfile();
	afx_msg void OnUpdateLineProfile(CCmdUI *pCmdUI);
	afx_msg void OnGetroi();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	//Line Profile 
	CLineProfileDlg *m_pLineProfileDlg;
	bool m_bLineProfile;
	bool m_bLineProfileStart;
	CPoint m_Start, m_End;

	//Get ROI
	bool m_bGetROI;
	bool m_bGetROIStart;
	CRect m_roi;

	//Zoom 
	void SetScrollSizeToFit();
	afx_msg void OnUpdateGetroi(CCmdUI *pCmdUI);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

#ifndef _DEBUG  // MyIPTool2View.cpp�� ����� ����
inline CMyIPTool2Doc* CMyIPTool2View::GetDocument() const
   { return reinterpret_cast<CMyIPTool2Doc*>(m_pDocument); }
#endif

