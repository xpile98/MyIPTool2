// LineProfileDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "LineProfileDlg.h"
#include "afxdialogex.h"
//#include "MyIPTool2View.h"

// CLineProfileDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLineProfileDlg, CDialogEx)

CLineProfileDlg::CLineProfileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LINE_PROFILE, pParent)
	, bisStartSelected(false)
{

}

CLineProfileDlg::~CLineProfileDlg()
{
}

void CLineProfileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLineProfileDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CLineProfileDlg 메시지 처리기입니다.

BOOL CLineProfileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CLineProfileDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	CGdiObject* pOldPen = dc.SelectStockObject(DC_PEN);

	//히스토그램 박스
	dc.SetDCPenColor(RGB(0, 0, 0));
	dc.MoveTo(20, 20);
	dc.LineTo(20, 220);
	dc.LineTo(420, 220);
	dc.LineTo(420, 20);
	dc.LineTo(20, 20);
	/*
	//각 그레이스케일에 해당하는 히스토그램 출력
	dc.SetDCPenColor(RGB(0, 0, 0));
	for (int i = 0; i < 256; i++) {
		// 		dc.MoveTo(20 + i, 120);
		// 		dc.LineTo(20 + i, 120 - m_Histogram[i]);
		dc.Rectangle(20 + (i * 2), 220, 22 + (i * 2), 220 - m_Histogram[i]);
	}

	//그레이스케일 레벨 출력
	for (int i = 0; i < 256; i++) {
		dc.SetDCPenColor(RGB(i, i, i));
		// 		dc.MoveTo(20 + i, 130);
		// 		dc.LineTo(20 + i, 145);
		dc.Rectangle(20 + (i * 2), 230, 22 + (i * 2), 245);
	}

	dc.SelectObject(pOldPen);

	CString strPoint;
	dc.SetBkColor(RGB(240, 240, 240));
	strPoint.Format(L"평균:         %.3f", m_dMean);
	dc.TextOut(540, 30, strPoint);
	strPoint.Format(L"표준편차: %.3f", m_dStdDest);
	dc.TextOut(540, 50, strPoint);
	*/
}




void CLineProfileDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//CDialogEx::OnClose();
	//DestroyWindow();
	this->ShowWindow(SW_HIDE);
	
// 	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
// 	CMyIPTool2View* pView = (CMyIPTool2View*)pFrame->GetActiveView();D
	//m_pView->Invalidate(FALSE);

}

