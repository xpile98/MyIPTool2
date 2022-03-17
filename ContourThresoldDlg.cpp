// ContourThresoldDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "ContourThresoldDlg.h"
#include "afxdialogex.h"


// ContourThresoldDlg ��ȭ �����Դϴ�.

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


// ContourThresoldDlg �޽��� ó�����Դϴ�.
