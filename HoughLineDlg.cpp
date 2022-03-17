// HoughLineDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "HoughLineDlg.h"
#include "afxdialogex.h"

#include "MyIPEdge.h"		//canny edge

// CHoughLineDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CHoughLineDlg, CDialogEx)

CHoughLineDlg::CHoughLineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HOUGH_LINE_DLG, pParent)
	, m_dTheta(1.0) , m_dRho(1.0)
	, m_nMaskHeight(10) , m_nMaskWidth(10)
	, m_nLowTh(10), m_nHighTh(200), m_nHoughTh(60)
{

}

CHoughLineDlg::~CHoughLineDlg()
{
}

void CHoughLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOW_THRESHOLD, m_sliderLowTh);
	DDX_Control(pDX, IDC_HIGH_THRESHOLD, m_sliderHighTh);
	DDX_Control(pDX, IDC_HOUGH_THRESHOLD, m_sliderHoughTh);
	DDX_Text(pDX, IDC_EDIT_THETA, m_dTheta);
	DDV_MinMaxDouble(pDX, m_dTheta, 0.0, 5.0);
	DDX_Text(pDX, IDC_EDIT_RHO, m_dRho);
	DDV_MinMaxDouble(pDX, m_dRho, 0.0, 10.0);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_nMaskHeight);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_nMaskWidth);
}


BEGIN_MESSAGE_MAP(CHoughLineDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CHoughLineDlg �޽��� ó�����Դϴ�.



BOOL CHoughLineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	m_sliderLowTh.SetRange(0, 255);
	m_sliderLowTh.SetTicFreq(32);
	m_sliderLowTh.SetPageSize(32);
	m_sliderLowTh.SetPos(10);

	m_sliderHighTh.SetRange(0, 255);
	m_sliderHighTh.SetTicFreq(32);
	m_sliderHighTh.SetPageSize(32);
	m_sliderHighTh.SetPos(200);

	m_sliderHoughTh.SetRange(0, 255);
	m_sliderHoughTh.SetTicFreq(32);
	m_sliderHoughTh.SetPageSize(32);
	m_sliderHoughTh.SetPos(100);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CHoughLineDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	if (m_sliderLowTh.GetSafeHwnd() == pScrollBar->GetSafeHwnd())	//���� ���õ� ��ũ�ѹ� ��ȣ�� m_sliderOffset�����̴��� ����?
	{
		m_nLowTh = m_sliderLowTh.GetPos();
		UpdateData(false);	//�������� ��Ʈ�ѿ� �����~ 
		DisplayImage();
	}

	else if(m_sliderHighTh.GetSafeHwnd() == pScrollBar->GetSafeHwnd())	//���� ���õ� ��ũ�ѹ� ��ȣ�� m_sliderOffset�����̴��� ����?
	{
		m_nHighTh = m_sliderHighTh.GetPos();
		UpdateData(false);	//�������� ��Ʈ�ѿ� �����~ 
		DisplayImage();
	}

	else if (m_sliderHoughTh.GetSafeHwnd() == pScrollBar->GetSafeHwnd())	//���� ���õ� ��ũ�ѹ� ��ȣ�� m_sliderOffset�����̴��� ����?
	{
		m_nHoughTh = m_sliderHoughTh.GetPos();
		UpdateData(false);	//�������� ��Ʈ�ѿ� �����~ 
		DisplayImage();
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CHoughLineDlg::DisplayImage()
{
	MyIPImage img;
	img = m_pDoc->m_MyIPImage;
	if (IpCanny(img, m_nLowTh, m_nHighTh)) {
		CClientDC dc(m_pView);	//���� view�ٰ� dc������!	view�� dc(�׸��׸��� ��)�� �������� 
		img.Draw(dc.m_hDC, 0, 0);
	}
}
