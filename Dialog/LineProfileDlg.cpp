// LineProfileDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "LineProfileDlg.h"
#include "afxdialogex.h"
//#include "MyIPTool2View.h"

// CLineProfileDlg ��ȭ �����Դϴ�.

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


// CLineProfileDlg �޽��� ó�����Դϴ�.

BOOL CLineProfileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CLineProfileDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
	CGdiObject* pOldPen = dc.SelectStockObject(DC_PEN);

	//������׷� �ڽ�
	dc.SetDCPenColor(RGB(0, 0, 0));
	dc.MoveTo(20, 20);
	dc.LineTo(20, 220);
	dc.LineTo(420, 220);
	dc.LineTo(420, 20);
	dc.LineTo(20, 20);
	/*
	//�� �׷��̽����Ͽ� �ش��ϴ� ������׷� ���
	dc.SetDCPenColor(RGB(0, 0, 0));
	for (int i = 0; i < 256; i++) {
		// 		dc.MoveTo(20 + i, 120);
		// 		dc.LineTo(20 + i, 120 - m_Histogram[i]);
		dc.Rectangle(20 + (i * 2), 220, 22 + (i * 2), 220 - m_Histogram[i]);
	}

	//�׷��̽����� ���� ���
	for (int i = 0; i < 256; i++) {
		dc.SetDCPenColor(RGB(i, i, i));
		// 		dc.MoveTo(20 + i, 130);
		// 		dc.LineTo(20 + i, 145);
		dc.Rectangle(20 + (i * 2), 230, 22 + (i * 2), 245);
	}

	dc.SelectObject(pOldPen);

	CString strPoint;
	dc.SetBkColor(RGB(240, 240, 240));
	strPoint.Format(L"���:         %.3f", m_dMean);
	dc.TextOut(540, 30, strPoint);
	strPoint.Format(L"ǥ������: %.3f", m_dStdDest);
	dc.TextOut(540, 50, strPoint);
	*/
}




void CLineProfileDlg::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	//CDialogEx::OnClose();
	//DestroyWindow();
	this->ShowWindow(SW_HIDE);
	
// 	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
// 	CMyIPTool2View* pView = (CMyIPTool2View*)pFrame->GetActiveView();D
	//m_pView->Invalidate(FALSE);

}

