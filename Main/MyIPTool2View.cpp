
// MyIPTool2View.cpp : CMyIPTool2View 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MyIPTool2.h"
#endif

#include "MainFrm.h"
#include "MyIPTool2Doc.h"
#include "MyIPTool2View.h"
#include "MyFunction.h"
#include "MyIPConvert.h"

#include <opencv2/opencv.hpp>

using namespace cv;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyIPTool2View

IMPLEMENT_DYNCREATE(CMyIPTool2View, CScrollView)

BEGIN_MESSAGE_MAP(CMyIPTool2View, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMyIPTool2View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_LINE_PROFILE, &CMyIPTool2View::OnLineProfile)
	ON_UPDATE_COMMAND_UI(ID_LINE_PROFILE, &CMyIPTool2View::OnUpdateLineProfile)
	ON_COMMAND(ID_GETROI, &CMyIPTool2View::OnGetroi)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_UPDATE_COMMAND_UI(ID_GETROI, &CMyIPTool2View::OnUpdateGetroi)
END_MESSAGE_MAP()

// CMyIPTool2View 생성/소멸

CMyIPTool2View::CMyIPTool2View() :
	m_bLineProfile(false), m_bLineProfileStart(false),
	m_bGetROI(false), m_bGetROIStart(false)
{

	//Line Progile Dialog
	m_pLineProfileDlg = NULL;
}

CMyIPTool2View::~CMyIPTool2View()
{
}

BOOL CMyIPTool2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CMyIPTool2View 그리기

void CMyIPTool2View::OnDraw(CDC* pDC)
{
	CMyIPTool2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//display image
#ifdef USE_IPIMAGE
	if (pDoc->m_MyIPImage.GetPixels()) {
		pDoc->m_MyIPImage.Draw(pDC->m_hDC, 0,0);
	}
#else
	MyIPImage img;

	if (pDoc->m_Mat.data) {							//m_Mat클래스가 정상적으로 이미지를 불러오면!
		if (IpMatToImage(pDoc->m_Mat, img)) {		//Mat클래스에서 MyIPImage클래스로 변환 성공하면!
			AfxGetMainWnd()->SetWindowTextW(pDoc->m_Title);
			img.Draw(pDC->m_hDC, 0, 0);			// 현재 윈도우의 시작점 0,0에 그려라 
		}
	}
#endif
	//line profile
	if (m_bLineProfile && m_bLineProfileStart) {
		CPen red_pen(PS_SOLID, 2, RGB(255, 0, 0));
		pDC->SelectStockObject(DC_PEN);
		pDC->SelectObject(&red_pen);
		pDC->MoveTo(m_Start);
		pDC->LineTo(m_Start);
		pDC->LineTo(m_End);
		red_pen.DeleteObject();
	}


	//get roi
	if (m_bGetROI && m_bGetROIStart) {
		CPen red_pen(PS_SOLID, 2, RGB(255, 0, 0));
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->SelectObject(&red_pen);
		CPoint asd = GetScrollPosition();
		m_roi += GetScrollPosition();
		pDC->Rectangle(m_roi);
		red_pen.DeleteObject();
	}
}

void CMyIPTool2View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// TODO: 이 뷰의 전체 크기를 계산합니다.
	CSize sizeTotal;
	CMyIPTool2Doc* pDoc = GetDocument();

#ifdef USE_IPIMAGE
	if (pDoc->m_MyIPImage.GetPixels())
	{
		sizeTotal.cx = pDoc->m_MyIPImage.GetWidth();
		sizeTotal.cy = pDoc->m_MyIPImage.GetHeight();
	}
#else
	if (pDoc->m_Mat.data)	//갖고있는영상이정상이면
	{
		sizeTotal.cx = pDoc->m_Mat.cols;
		sizeTotal.cy = pDoc->m_Mat.rows;
	}
#endif
	else
	{
		sizeTotal.cx = sizeTotal.cy = 100;
	}

	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit(TRUE);


}


// CMyIPTool2View 인쇄


void CMyIPTool2View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMyIPTool2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMyIPTool2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMyIPTool2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CMyIPTool2View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMyIPTool2View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMyIPTool2View 진단

#ifdef _DEBUG
void CMyIPTool2View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMyIPTool2View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMyIPTool2Doc* CMyIPTool2View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyIPTool2Doc)));
	return (CMyIPTool2Doc*)m_pDocument;
}
#endif //_DEBUG


// CMyIPTool2View 메시지 처리기


BOOL CMyIPTool2View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CBrush br;
	br.CreateHatchBrush(HS_DIAGCROSS, RGB(200, 200, 200));
	FillOutsideRect(pDC, &br);

	return TRUE;
}


void CMyIPTool2View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CPoint pt = point + GetScrollPosition();
	CClientDC dc(this);

	CMainFrame* pFrame = (CMainFrame *)AfxGetMainWnd();
	CMyIPTool2Doc* pDoc = GetDocument();

	if (!pDoc->m_MyIPImage.GetPixels())
		return;

	int w = pDoc->m_MyIPImage.GetWidth();
	int h = pDoc->m_MyIPImage.GetHeight();
	int depth = pDoc->m_MyIPImage.GetDepth();
	int channel = pDoc->m_MyIPImage.GetChannels();
	byte** pb_Pixel = (byte**)pDoc->m_MyIPImage.GetPixels();
	unsigned short** pus_Pixel = (unsigned short**)pDoc->m_MyIPImage.GetPixels();
	RGBBYTE** prgb_Pixel = (RGBBYTE**)pDoc->m_MyIPImage.GetPixels();
	RGBWORD** prgbw_Pixel = (RGBWORD**)pDoc->m_MyIPImage.GetPixels();
	
	int x = pt.x;
	int y = pt.y;

	CString message;
	if (point.x >= 0 && point.y >= 0 && point.x < w && point.y < h) {
		switch (channel) {
		case 1:
			if (depth == CV_8U)
				message.Format(_T("(%d, %d) %d"), x, y, pb_Pixel[y][x]);
			else if (depth == CV_16U)
				message.Format(_T("(%d, %d) %d"), x, y, pus_Pixel[y][x]);
			pFrame->m_wndStatusBar.SetPaneText(0, message);
			break;
		case 3:
			if (depth == CV_8U)
				message.Format(_T("(%d, %d) R:%d G:%d B:%d"), x, y, prgb_Pixel[y][x].r, prgb_Pixel[y][x].g, prgb_Pixel[y][x].b);	//012순서대로 BGR임! 
			else if (depth == CV_16U)
				message.Format(_T("(%d, %d) R:%d G:%d B:%d"), x, y, prgbw_Pixel[y][x].r, prgbw_Pixel[y][x].g, prgbw_Pixel[y][x].b);	//012순서대로 BGR임! 
			pFrame->m_wndStatusBar.SetPaneText(0, message);
			break;
		}

		message.Format(_T("H: %d, W: %d, DEPTH: %d, CHANNEL: %d"), h, w, depth, channel);
		pFrame->m_wndStatusBar.SetPaneText(1, message);
	}

	//LineProfile
	if (m_bLineProfile && m_bLineProfileStart) {
		m_End = point;
		Invalidate(FALSE);
	}


	//GetROI
	if (m_bGetROI && m_bGetROIStart) {
		m_roi.right = point.x;
		m_roi.bottom = point.y;
		Invalidate(FALSE);
	}

	CScrollView::OnMouseMove(nFlags, point);
}


void CMyIPTool2View::OnLineProfile()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_bLineProfile = true;
	Invalidate(FALSE);

	if (m_pLineProfileDlg != NULL) {
		m_pLineProfileDlg->ShowWindow(SW_SHOW);
		m_pLineProfileDlg->SetFocus();
	}
	else {
		m_pLineProfileDlg = new CLineProfileDlg;
		CMyIPTool2Doc* pDoc = GetDocument();
		m_pLineProfileDlg->Create(IDD_LINE_PROFILE, this);
		m_pLineProfileDlg->ShowWindow(SW_SHOW);
		m_pLineProfileDlg->m_pWnd = this;
		m_pLineProfileDlg->m_pImage = &(pDoc->m_MyIPImage);
	}
}




void CMyIPTool2View::OnGetroi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if(!m_bGetROI) m_bGetROI = true;
	else m_bGetROI = false;
	m_Start = m_End = CPoint(0, 0);
	Invalidate(FALSE);
}


void CMyIPTool2View::OnLButtonDown(UINT nFlags, CPoint point)
{
// 	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bLineProfile) {
		m_bLineProfileStart = true;
		m_Start = point;
	}



	if (m_bGetROI) {		//GetROI모드일때만 실행 

		m_bGetROIStart = true;
		m_roi.left = point.x;
		m_roi.top = point.y;
	}
	CScrollView::OnLButtonDown(nFlags, point);
}


void CMyIPTool2View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bLineProfile) {
		m_bLineProfile = false;
		m_bLineProfileStart = false;
	}

	if (m_bGetROI) {	//GetROI모드일때만 실행 

		m_bGetROI = false;
		m_bGetROIStart = false;

		CPoint TRpt = m_roi.TopLeft() + GetScrollPosition();
		CPoint BRpt = point + GetScrollPosition();
		CRect rect(TRpt,BRpt);
		if (rect.bottom < rect.top) {
			rect.top = m_roi.bottom;
			rect.bottom = m_roi.top;
		}
		if (rect.right < rect.left) {
			rect.left = m_roi.right;
			rect.right = m_roi.left;
		}

		CMyIPTool2Doc *pDoc = GetDocument();

		MyIPImage img;
		CString title;
		title.Format(pDoc->GetTitle() + _T("_ROI(%d,%d %d,%d)"), rect.left, rect.top, rect.right, rect.bottom);
		if (img.GetROI(pDoc->m_MyIPImage, rect)) {
			AfxNewImage(title, img);
		}
		Invalidate(FALSE);
	}
	CScrollView::OnLButtonUp(nFlags, point);
}









//tool bar check 부분 

void CMyIPTool2View::OnUpdateLineProfile(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_bLineProfile);
}


void CMyIPTool2View::OnUpdateGetroi(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_bGetROI);
}
