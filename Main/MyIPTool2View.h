
// MyIPTool2View.h : CMyIPTool2View 클래스의 인터페이스
//

#pragma once
#include "LineProfileDlg.h"

class CMyIPTool2View : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	CMyIPTool2View();
	DECLARE_DYNCREATE(CMyIPTool2View)

// 특성입니다.
public:
	CMyIPTool2Doc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMyIPTool2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
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

#ifndef _DEBUG  // MyIPTool2View.cpp의 디버그 버전
inline CMyIPTool2Doc* CMyIPTool2View::GetDocument() const
   { return reinterpret_cast<CMyIPTool2Doc*>(m_pDocument); }
#endif

