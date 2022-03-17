// BrightnessContrast.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "BrightnessContrastDlg.h"
#include "afxdialogex.h"

#include "MyIPConvert.h"
#include "MyIPEnhance.h"

// CBrightnessContrastDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CBrightnessContrastDlg, CDialogEx)

CBrightnessContrastDlg::CBrightnessContrastDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BRIGHTNESS_CONTRAST_DLG, pParent)
	, m_nOffset(0), m_fGain(1.), bIs16Bit(false)
{

}

CBrightnessContrastDlg::~CBrightnessContrastDlg()
{
}

void CBrightnessContrastDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_OFFSET_EDIT, m_nOffset);
	//DDV_MinMaxInt(pDX, m_nOffset, -255, 255);	//-> event �Լ����� ó�� 
	DDX_Text(pDX, IDC_GAIN_EDIT, m_fGain);
	DDV_MinMaxFloat(pDX, m_fGain, -100, 100);
	DDX_Control(pDX, IDC_GAIN_SLIDER, m_slliderGain);
	DDX_Control(pDX, IDC_OFFSET_SLIDER, m_sliderOffset);
	DDX_Control(pDX, IDC_CHECK_16BIT, m_ctrlCheck16Bit);
}


BEGIN_MESSAGE_MAP(CBrightnessContrastDlg, CDialogEx)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_OFFSET_EDIT, &CBrightnessContrastDlg::OnEnChangeOffsetEdit)
	ON_EN_CHANGE(IDC_GAIN_EDIT, &CBrightnessContrastDlg::OnEnChangeGainEdit)
END_MESSAGE_MAP()


// CBrightnessContrastDlg �޽��� ó�����Դϴ�.


BOOL CBrightnessContrastDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	//��� ���� �����̴� �ʱ�ȭ
	if (bIs16Bit == false) {
		m_ctrlCheck16Bit.SetCheck(false);
		m_sliderOffset.SetRange(-255, 255);
		m_sliderOffset.SetTicFreq(32);
		m_sliderOffset.SetPageSize(32);
	}
	else {		//16��Ʈ 
		m_ctrlCheck16Bit.SetCheck(true);
		m_sliderOffset.SetRange(-65535, 65535);
		m_sliderOffset.SetTicFreq(8192);
		m_sliderOffset.SetPageSize(8192);
	}
	m_ctrlCheck16Bit.EnableWindow(FALSE);

	//��Ϻ� ���� �����̴� �ʱ�ȭ
	m_slliderGain.SetRange(-100, 100);
	m_slliderGain.SetTicFreq(20);
	m_slliderGain.SetPageSize(20);
	m_slliderGain.SetPos(10);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CBrightnessContrastDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (m_sliderOffset.GetSafeHwnd() == pScrollBar->GetSafeHwnd())	//���� ���õ� ��ũ�ѹ� ��ȣ�� m_sliderOffset�����̴��� ����?
	{
		m_nOffset = m_sliderOffset.GetPos();
		UpdateData(false);	//�������� ��Ʈ�ѿ� �����~ 
		DisplayImage();
	}

	else if (m_slliderGain.GetSafeHwnd() == pScrollBar->GetSafeHwnd())	//���� ���õ� ��ũ�ѹ� ��ȣ�� m_sliderOffset�����̴��� ����?
	{
		m_fGain = m_slliderGain.GetPos()*0.1f;
		UpdateData(false);	//�������� ��Ʈ�ѿ� �����~ 
		DisplayImage();
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CBrightnessContrastDlg::OnEnChangeOffsetEdit()
{
	UpdateData(true);	//�� ���� �о�Ͷ� 

	if (bIs16Bit == false) {
		if (m_nOffset < -255 || m_nOffset > 255) {
			if (m_nOffset < -255)		m_nOffset = -255;
			else if (m_nOffset > 255)	m_nOffset = 255;
			AfxMessageBox(_T("-255���� 255 ������ ������ �Է��Ͻʽÿ�."));
		}
	}

	else {
		if (m_nOffset < -65535 || m_nOffset > 65535) {
			if (m_nOffset < -65535)		m_nOffset = -65535;
			else if (m_nOffset > 65535)	m_nOffset = 65535;
			AfxMessageBox(_T("-65535���� 65535 ������ ������ �Է��Ͻʽÿ�."));
		}
	}
	m_sliderOffset.SetPos(m_nOffset);
	DisplayImage();
}


void CBrightnessContrastDlg::OnEnChangeGainEdit()
{
	UpdateData(true);	//�� ���� �о�Ͷ� 
	m_slliderGain.SetPos((int)(m_fGain*10.f));
	DisplayImage();
}

void CBrightnessContrastDlg::DisplayImage()
{
#ifdef USE_IPIMAGE	//190315 YWCho mat -> image
	MyIPImage img;
	img = m_pDoc->m_MyIPImage;
		if (IP_BrightnessContrast(img, m_nOffset, m_fGain)) {
			CClientDC dc(m_pView);	//���� view�ٰ� dc������!	view�� dc(�׸��׸��� ��)�� �������� 
			img.Draw(dc.m_hDC, 0, 0);
		}
#else
	MyIPImage img;
	if (IpMatToImage(m_pDoc->m_Mat, img)) {
		if (IP_BrightnessContrast(img, m_nOffset, m_fGain)) {
			CClientDC dc(m_pView);	//���� view�ٰ� dc������!	view�� dc(�׸��׸��� ��)�� �������� 
			img.Draw(dc.m_hDC, 0, 0);
		}
	}
#endif
}
