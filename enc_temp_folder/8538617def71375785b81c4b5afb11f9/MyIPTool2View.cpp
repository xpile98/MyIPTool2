
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
	// ǥ�� �μ� �����Դϴ�.
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
	ON_WM_MOUSEWHEEL()
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
		int w = pDoc->m_MyIPImage.GetWidth();
		int h = pDoc->m_MyIPImage.GetHeight();
		//pDoc->m_MyIPImage.Draw(pDC->m_hDC, 0, 0, w*pDoc->m_fZoom, h*pDoc->m_fZoom);
		pDoc->m_MyIPImage.Draw(pDC->m_hDC, 0, 0, w*pDoc->m_fZoom, h*pDoc->m_fZoom, 0, 0, w, h);
	}
#else
	MyIPImage img;

	if (pDoc->m_Mat.data) {							//setscrollsizem_MatŬ������ ���������� �̹����� �ҷ�����!
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
		CPen red_pen(PS_SOLID, 2, RGB(255, 255, 0));
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->SelectObject(&red_pen);
		CPoint asd = GetScrollPosition();
		// �ӻ� m_roi += GetScrollPosition();
		pDC->Rectangle(m_roi);
		red_pen.DeleteObject();
	}

// 	if (1)	//zoom
// 	{
// 		if (pDoc->m_MyIPImage_org.GetPixels())
// 		{
// 			int w = pDoc->m_MyIPImage_org.GetWidth();
// 			int h = pDoc->m_MyIPImage_org.GetHeight();
// 			pDoc->m_MyIPImage_org.Draw(pDC->m_hDC, 0, 0, w*1.2, h*1.2);
// 		}
// 	}
}

void CMyIPTool2View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// TODO: �� ���� ��ü ũ�⸦ ����մϴ�.
	SetScrollSizeToFit();

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
	if (pt.x >= 0 && pt.y >= 0 && pt.x < w && pt.y < h) {
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
		m_End = pt;
		Invalidate(FALSE);
	}


	//GetROI
	if (m_bGetROI && m_bGetROIStart) {
		m_roi.right = pt.x;
		m_roi.bottom = pt.y;
		Invalidate(FALSE);
	}

	CScrollView::OnMouseMove(nFlags, pt);
}


void CMyIPTool2View::OnLineProfile()
{
	// TODO: ���⿡ ���� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ���� ó���� �ڵ带 �߰��մϴ�.
	if(!m_bGetROI) m_bGetROI = true;
	else m_bGetROI = false;
	m_Start = m_End = CPoint(0, 0);
	Invalidate(FALSE);
}


void CMyIPTool2View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CPoint pt = point + GetScrollPosition();
	if (m_bLineProfile) {
		m_bLineProfileStart = true;
		m_Start = pt;
	}



	if (m_bGetROI) {		//GetROI����϶��� ���� 

		m_bGetROIStart = true;
		m_roi.left = pt.x;
		m_roi.top = pt.y;
	}
	CScrollView::OnLButtonDown(nFlags, point);
}


void CMyIPTool2View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CPoint pt = point + GetScrollPosition();
	if (m_bLineProfile) {
		m_bLineProfile = false;
		m_bLineProfileStart = false;
	}

	if (m_bGetROI) {	//GetROI����϶��� ���� 

		m_bGetROI = false;
		m_bGetROIStart = false;

		CPoint TRpt = m_roi.TopLeft();
		CPoint BRpt = m_roi.BottomRight();
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
	// TODO: ���⿡ ���� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	pCmdUI->SetCheck(m_bLineProfile);
}


void CMyIPTool2View::SetScrollSizeToFit()
{
	CSize sizeTotal;
	CMyIPTool2Doc* pDoc = GetDocument();

#ifdef USE_IPIMAGE
	if (pDoc->m_MyIPImage.GetPixels())
	{
		sizeTotal.cx = pDoc->m_MyIPImage.GetWidth() * pDoc->m_fZoom;
		sizeTotal.cy = pDoc->m_MyIPImage.GetHeight() * pDoc->m_fZoom;
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
	CPoint ct;
	ct = GetScrollPosition();

// 	int min = 0;
// 	int max = 0;
// 	GetScrollRange(SB_HORZ, &min, &max);
// 	GetScrollRange(SB_VERT, &min, &max);
// 
// 	int lim;
// 	lim = GetScrollLimit(SB_HORZ);
// 	lim = GetScrollLimit(SB_VERT);

	ResizeParentToFit(TRUE);
}

void CMyIPTool2View::OnUpdateGetroi(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ���� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	pCmdUI->SetCheck(m_bGetROI);
}


BOOL CMyIPTool2View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	
	POINT ptClient;
	ptClient.x = pt.x;
	ptClient.y = pt.y;
	ScreenToClient(&ptClient);
	/*
	CMyIPTool2Doc *pDoc = GetDocument();
	MyIPImage img = pDoc->m_MyIPImage;
	MyIPImage img_zoom = pDoc->m_MyIPImage;

	double zoom_scale = 1.2;
	double new_width, new_height;
	if (zDelta > 0)
	{
		new_width = img_zoom.GetWidth() / zoom_scale;
		new_height = img_zoom.GetHeight() / zoom_scale;
	}
	else
	{
		new_width = img_zoom.GetWidth()*zoom_scale;
		new_height = img_zoom.GetHeight()*zoom_scale;
	}

	if (new_width > img.GetWidth() || new_height > img.GetHeight())
	{
		new_width = img.GetWidth();
		new_height = img.GetHeight();
	}

	if (new_width < 10 && new_height < 10)
	{
		return CWnd::OnMouseWheel(nFlags, zDelta, pt);
	}

	double new_center_x = new_width/2;
	double new_center_y = new_height/2;

	MyIPImage img_zoom_new, img_zoom_new2;
	img_zoom_new.CreateImage(new_height, new_width, img.GetDepth(), img.GetChannels());
	
	CRect roi;
 	roi.left = new_center_x - new_width / 2;
 	roi.right = new_center_x + new_width / 2;
 	roi.top = new_center_y - new_height / 2;
 	roi.bottom = new_center_y + new_height / 2;

 	if (roi.left < 0)
 	{
		roi.right = roi.Width();
 		roi.left = 0;
 	}
 
 	if (roi.top < 0)
 	{
		roi.bottom = roi.Height();
		roi.top = 0;
 	}
 
 	if (roi.right >= img.GetWidth())
 	{
		roi.right = img.GetWidth();
 	}
 
 	if (roi.bottom >= img.GetHeight())
 	{
		roi.bottom = img.GetHeight();
 	}

	img_zoom_new.GetROI(img, roi);
	pDoc->m_MyIPImage_org.Copy(img_zoom_new);

	
	//test1
	//AfxNewImage(img_zoom_new);

	//try2
	//CClientDC dc(this);	//���� view�ٰ� dc������!	view�� dc(�׸��׸��� ��)�� �������� 
	// 	img_zoom_new.Draw(dc.m_hDC, 0, 0);
	// 	UpdateData(FALSE);

	
	*/

	CMyIPTool2Doc *pDoc = GetDocument();
	if (zDelta > 0)
	{
		pDoc->m_fZoom *= 1.2;
	}
	else
	{
		pDoc->m_fZoom /= 1.2;
		if (pDoc->m_fZoom < 1)
			pDoc->m_fZoom = 1.0;
	}
	SetScrollSizeToFit();
	Invalidate(TRUE);

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}