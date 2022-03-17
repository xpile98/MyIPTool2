// ContourThresoldDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "ContourThresoldDlg.h"
#include "afxdialogex.h"


// ContourThresoldDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(ContourThresoldDlg, CDialogEx)

ContourThresoldDlg::ContourThresoldDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CONTOUR_THRESOLD, pParent)
{
	m_nContourThreshold = 100;
}

ContourThresoldDlg::~ContourThresoldDlg()
{
}

void ContourThresoldDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CONTOUR_TH, m_nContourThreshold);
}


BEGIN_MESSAGE_MAP(ContourThresoldDlg, CDialogEx)
END_MESSAGE_MAP()


// ContourThresoldDlg 메시지 처리기입니다.
