
// MyIPTool2View.cpp : CMyIPTool2View Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
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

// CMyIPTool2View ����/�Ҹ�

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
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

// CMyIPTool2View �׸���

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

	if (pDoc->m_Mat.data) {							//m_MatŬ������ ���������� �̹����� �ҷ�����!
		if (IpMatToImage(pDoc->m_Mat, img)) {		//MatŬ�������� MyIPImageŬ������ ��ȯ �����ϸ�!
			AfxGetMainWnd()->SetWindowTextW(pDoc->m_Title);
			img.Draw(pDC->m_hDC, 0, 0);			// ���� �������� ������ 0,0�� �׷��� 
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

	// TODO: �� ���� ��ü ũ�⸦ ����մϴ�.
	CSize sizeTotal;
	CMyIPTool2Doc* pDoc = GetDocument();

#ifdef USE_IPIMAGE
	if (pDoc->m_MyIPImage.GetPixels())
	{
		sizeTotal.cx = pDoc->m_MyIPImage.GetWidth();
		sizeTotal.cy = pDoc->m_MyIPImage.GetHeight();
	}
#else
	if (pDoc->m_Mat.data)	//�����ִ¿����������̸�
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


// CMyIPTool2View �μ�


void CMyIPTool2View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMyIPTool2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMyIPTool2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMyIPTool2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
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


// CMyIPTool2View ����

#ifdef _DEBUG
void CMyIPTool2View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMyIPTool2View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMyIPTool2Doc* CMyIPTool2View::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyIPTool2Doc)));
	return (CMyIPTool2Doc*)m_pDocument;
}
#endif //_DEBUG


// CMyIPTool2View �޽��� ó����


BOOL CMyIPTool2View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CBrush br;
	br.CreateHatchBrush(HS_DIAGCROSS, RGB(200, 200, 200));
	FillOutsideRect(pDC, &br);

	return TRUE;
}


void CMyIPTool2View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
				message.Format(_T("(%d, %d) R:%d G:%d B:%d"), x, y, prgb_Pixel[y][x].r, prgb_Pixel[y][x].g, prgb_Pixel[y][x].b);	//012������� BGR��! 
			else if (depth == CV_16U)
				message.Format(_T("(%d, %d) R:%d G:%d B:%d"), x, y, prgbw_Pixel[y][x].r, prgbw_Pixel[y][x].g, prgbw_Pixel[y][x].b);	//012������� BGR��! 
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	if(!m_bGetROI) m_bGetROI = true;
	else m_bGetROI = false;
	m_Start = m_End = CPoint(0, 0);
	Invalidate(FALSE);
}


void CMyIPTool2View::OnLButtonDown(UINT nFlags, CPoint point)
{
// 	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (m_bLineProfile) {
		m_bLineProfileStart = true;
		m_Start = point;
	}



	if (m_bGetROI) {		//GetROI����϶��� ���� 

		m_bGetROIStart = true;
		m_roi.left = point.x;
		m_roi.top = point.y;
	}
	CScrollView::OnLButtonDown(nFlags, point);
}


void CMyIPTool2View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (m_bLineProfile) {
		m_bLineProfile = false;
		m_bLineProfileStart = false;
	}

	if (m_bGetROI) {	//GetROI����϶��� ���� 

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









//tool bar check �κ� 

void CMyIPTool2View::OnUpdateLineProfile(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	pCmdUI->SetCheck(m_bLineProfile);
}


void CMyIPTool2View::OnUpdateGetroi(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	pCmdUI->SetCheck(m_bGetROI);
}
