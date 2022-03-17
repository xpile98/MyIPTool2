// Gamma.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "GammaDlg.h"
#include "afxdialogex.h"

#include "MyIPConvert.h"
#include "MyIPEnhance.h"

// CGammaDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CGammaDlg, CDialogEx)

CGammaDlg::CGammaDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GAMMA_DLG, pParent)
	, m_fGamma(2.2f), bIs16Bit(false)
{

}

CGammaDlg::~CGammaDlg()
{
}

void CGammaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_GAMMA_EDIT, m_fGamma);
	DDV_MinMaxFloat(pDX, m_fGamma, 0.20f, 5.00f);
	DDX_Control(pDX, IDC_GAMMA_SLIDER, m_sliderGamma);
	DDX_Control(pDX, IDC_CHECK_16BIT, m_ctrlCheck16Bit);
}


BEGIN_MESSAGE_MAP(CGammaDlg, CDialogEx)
	ON_EN_CHANGE(IDC_GAMMA_EDIT, &CGammaDlg::OnEnChangeGammaEdit)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CGammaDlg �޽��� ó�����Դϴ�.


BOOL CGammaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	if (bIs16Bit == false)		//16��Ʈ ����
		m_ctrlCheck16Bit.SetCheck(false);
	else
		m_ctrlCheck16Bit.SetCheck(true);
	m_ctrlCheck16Bit.EnableWindow(false);


	m_sliderGamma.SetRange(10, 250); //�������� 0.2~5.0 �̹Ƿ� 50�� ���Ѱ����� ������ ����
	m_sliderGamma.SetTicFreq(20);
	m_sliderGamma.SetPageSize(20);
	m_sliderGamma.SetPos(static_cast<int>(m_fGamma * 50));	//static cast�� ���� ĳ����(����ȯ) ���н� ������ ������ �߻���Ŵ


	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CGammaDlg::OnEnChangeGammaEdit()
{
	UpdateData(TRUE);
	m_sliderGamma.SetPos(static_cast<int>(m_fGamma * 50));
	DisplayImage();
}


void CGammaDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (m_sliderGamma.GetSafeHwnd() == pScrollBar->GetSafeHwnd()) {
		int pos = m_sliderGamma.GetPos();
		m_fGamma = pos / 50.f;
		UpdateData(FALSE);
		DisplayImage();
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CGammaDlg::DisplayImage()
{
#ifdef USE_IPIMAGE
	MyIPImage img;
	img = m_pDoc->m_MyIPImage;
	if (IP_GammaCorrection(img, m_fGamma)) {
		CClientDC dc(m_pView);	//���� view�ٰ� dc������!	view�� dc(�׸��׸��� ��)�� �������� 
		img.Draw(dc.m_hDC, 0, 0);
	}
#else
	MyIPImage img;
	if (IpMatToImage(m_pDoc->m_Mat, img)) {
		if (IP_GammaCorrection(img, m_fGamma)) {
			CClientDC dc(m_pView);	//���� view�ٰ� dc������!	view�� dc(�׸��׸��� ��)�� �������� 
			img.Draw(dc.m_hDC, 0, 0);
		}
	}
#endif
}
