// ThresholdDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "ThresholdDlg.h"
#include "afxdialogex.h"

#include "MyIPConvert.h"
#include "MyIPEnhance.h"

// CThresholdDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CThresholdDlg, CDialogEx)

CThresholdDlg::CThresholdDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_THRESHOLD, pParent)
	, m_nThreshold(128), m_nHistogram(0), m_nOldPos(0), m_nAutoThresholdingVal(-1)
	, bIs16Bit(false)
{

}

CThresholdDlg::~CThresholdDlg()
{
	delete[] m_HistogramOrg;
}

void CThresholdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_THRESHOLD_SLIDER, m_sliderThreshold);
	//  DDX_Text(pDX, IDC_THRESHOLD_EDIT, m_byteThreshold);
	//  DDV_MinMaxByte(pDX, m_byteThreshold, 0, 65535);
	DDX_Text(pDX, IDC_HISTOGRAM_EDIT, m_nHistogram);
	//  DDX_Check(pDX, IDC_CHECK_16BIT, m_bCheck16Bit);
	DDX_Control(pDX, IDC_CHECK_16BIT, m_ctrlCheck16Bit);
	DDX_Text(pDX, IDC_THRESHOLD_EDIT, m_nThreshold);
	DDV_MinMaxInt(pDX, m_nThreshold, 0, 65535);
}


BEGIN_MESSAGE_MAP(CThresholdDlg, CDialogEx)
	ON_EN_CHANGE(IDC_THRESHOLD_EDIT, &CThresholdDlg::OnEnChangeThresholdEdit)
	ON_EN_CHANGE(IDC_HISTOGRAM_EDIT, &CThresholdDlg::OnEnChangeHistogramEdit)
	ON_WM_HSCROLL()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_16BIT, &CThresholdDlg::OnBnClickedCheck16bit)
END_MESSAGE_MAP()


// CThresholdDlg �޽��� ó�����Դϴ�.


BOOL CThresholdDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	if (bIs16Bit == true) {
		m_HistogramOrg = new unsigned int[65536];		//������׷� ����� �迭 �����Ҵ� 
		m_ctrlCheck16Bit.SetCheck(true);		//16��Ʈ üũ�ڽ� üũ

		if (m_nAutoThresholdingVal == -1)	//�ʱⰪ�� �������� ������ 127��~ 
			m_nAutoThresholdingVal = 32768;

		m_sliderThreshold.SetRange(0, 65535);
		m_sliderThreshold.SetTicFreq(256);
		m_sliderThreshold.SetPageSize(256);
		m_sliderThreshold.SetPos(m_nAutoThresholdingVal);	//static cast�� ���� ĳ����(����ȯ) ���н� ������ ������ �߻���Ŵ
	}
	else {
		m_HistogramOrg = new unsigned int[256];		//������׷� ����� �迭 �����Ҵ� 
		m_ctrlCheck16Bit.SetCheck(false);

		if (m_nAutoThresholdingVal == -1)	//�ʱⰪ�� �������� ������ 127��~ 
			m_nAutoThresholdingVal = 128;

		m_sliderThreshold.SetRange(0, 255); 
		m_sliderThreshold.SetTicFreq(32);
		m_sliderThreshold.SetPageSize(32);
		m_sliderThreshold.SetPos(m_nAutoThresholdingVal);	//static cast�� ���� ĳ����(����ȯ) ���н� ������ ������ �߻���Ŵ
	}
	m_ctrlCheck16Bit.EnableWindow(FALSE);		//üũ�ڽ� ��Ȱ��ȭ

	IP_Histogram(m_pDoc->m_MyIPImage, m_HistogramOrg);
	m_nThreshold = m_nAutoThresholdingVal;
	m_nHistogram = m_HistogramOrg[m_nAutoThresholdingVal];	//�ʱⰪ ����

	DisplayImage();
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CThresholdDlg::DisplayImage()
{
#ifdef USE_IPIMAGE
	MyIPImage img;
	img = m_pDoc->m_MyIPImage;
	if (IP_Threshold(img, m_nThreshold)) {
		CClientDC dc(m_pView);	//���� view�ٰ� dc������!	view�� dc(�׸��׸��� ��)�� �������� 
		img.Draw(dc.m_hDC, 0, 0);
	}
#else
	MyIPImage img;
	if (IpMatToImage(m_pDoc->m_Mat, img)) {
		if (IP_Threshold(img, m_byteThreshold)) {
			CClientDC dc(m_pView);	//���� view�ٰ� dc������!	view�� dc(�׸��׸��� ��)�� �������� 
			img.Draw(dc.m_hDC, 0, 0);
		}
	}
#endif
}

void CThresholdDlg::SetImage(MyIPImage & Img)
{
	if (Img == NULL) {
		memset(m_Histogram, 0, sizeof(int) * 256);
		return;
	}

	int depth = Img.GetDepth();
	float *histo;
	float max_histo;
	float nhisto[256] = { 0, };

	switch (depth)
	{
	case CV_8U:
		//����ȭ �� ������׷� ���ϱ�
		histo = new float[256];
		if (!histo) return;
		IP_Histogram(Img, histo);

		//����ȭ�� ������׷����� �ִ��� ���Ѵ�.
		max_histo = histo[0];
		for (int i = 1; i < 256; i++) {
			if (histo[i] > max_histo)	max_histo = histo[i];
			//m_HistogramOrg[i] = static_cast<int>(histo[i] * Img.GetWidth()*Img.GetHeight());
		}

		//m_Histogram �迭�� �ִ��� 170�� �ǵ��� ��ü �迭�� ���� �����Ѵ�.
		for (int i = 0; i < 256; i++) 
			m_Histogram[i] = static_cast<int>(histo[i] * 170 / max_histo);

// 		//��� �� ǥ������ ���ϱ�.
// 		IP_Mean_StandardDeviation(Img);
// 		m_dMean = Img.GetMean();
// 		m_dStdDest = Img.GetStddDest();

		delete[] histo;
		break;
	case CV_16U:
		bIs16Bit = true;
		//����ȭ �� ������׷� ���ϱ�
		histo = new float[65536];
		if (!histo) return;
		IP_Histogram(Img, histo);

		//(16��Ʈ) 0~255�� ������׷� ���� nhisto[0]�� �����ϰ�..�׷���
		for (int i = 0; i < 65536; i++) {
			nhisto[i / 256] += histo[i];
		}

		//����ȭ�� ������׷����� �ִ��� ���Ѵ�.
		max_histo = nhisto[0];
		for (int i = 1; i < 256; i++) {
			if (nhisto[i] > max_histo)	max_histo = nhisto[i];
			//m_HistogramOrg[i] = static_cast<int>(nhisto[i] * Img.GetWidth()*Img.GetHeight());
		}

		//m_Histogram �迭�� �ִ��� 170�� �ǵ��� ��ü �迭�� ���� �����Ѵ�.
		for (int i = 0; i < 256; i++) {
			m_Histogram[i] = static_cast<int>(nhisto[i] * 170 / max_histo);
		}
		delete[] histo;
		break;
	default:
		memset(m_Histogram, 0, sizeof(int) * 256);
		break;
	}
}

void CThresholdDlg::OnEnChangeThresholdEdit()
{
	UpdateData(TRUE);
	if (bIs16Bit == false) {
		if (m_nThreshold > 255) {
			m_nThreshold = 255;
			AfxMessageBox(_T("0���� 255 ������ ������ �Է��Ͻʽÿ�."));
		}
	}
	else {
		if (m_nThreshold > 65535)
			m_nThreshold = 65535;
	}

	int pos = 0;
	m_sliderThreshold.SetPos(m_nThreshold);
	if (bIs16Bit == false) {
		m_nHistogram = m_HistogramOrg[m_nThreshold];
		pos = m_nThreshold;
	}
	else {
		m_nHistogram = m_HistogramOrg[m_nThreshold];
		pos = m_nThreshold / 256;
	}


	CClientDC dc(this);
	CGdiObject* pOldPen = dc.SelectStockObject(DC_PEN);

	if (m_nOldPos != pos) {
		dc.SetDCPenColor(RGB(240, 240, 240));
		dc.MoveTo(20 + m_nOldPos, 30);
		dc.LineTo(20 + m_nOldPos, 200);
		dc.SetDCPenColor(RGB(0, 0, 0));
		dc.MoveTo(20 + m_nOldPos, 200);
		dc.LineTo(20 + m_nOldPos, 200 - m_Histogram[m_nOldPos]);
	}

	dc.SetDCPenColor(RGB(255, 0, 0));
	dc.MoveTo(20 + pos, 30);
	dc.LineTo(20 + pos, 200);
	m_nOldPos = pos;

	dc.SelectObject(pOldPen);


	UpdateData(false);
	DisplayImage();
}

void CThresholdDlg::OnEnChangeHistogramEdit()
{
	//������׷� ��Ʈ�ѹڽ� �� ����Ǹ� ȣ��Ǵ� �Լ� 
}

void CThresholdDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (m_sliderThreshold.GetSafeHwnd() == pScrollBar->GetSafeHwnd()) {

		int pos = m_sliderThreshold.GetPos();
		m_nThreshold = pos;

		if (bIs16Bit == true)	//16��Ʈ ���� 
			pos /= 256;
		m_nHistogram = m_HistogramOrg[pos];

		CClientDC dc(this);
		CGdiObject* pOldPen = dc.SelectStockObject(DC_PEN);

		if (m_nOldPos != pos) {
			dc.SetDCPenColor(RGB(240, 240, 240));
			dc.MoveTo(20 + m_nOldPos, 30);
			dc.LineTo(20 + m_nOldPos, 200);
			dc.SetDCPenColor(RGB(0, 0, 0));
			dc.MoveTo(20 + m_nOldPos, 200);
			dc.LineTo(20 + m_nOldPos, 200 - m_Histogram[m_nOldPos]);
		}

		dc.SetDCPenColor(RGB(255, 0, 0));
		dc.MoveTo(20+pos, 30);
		dc.LineTo(20+pos, 200);
		m_nOldPos = pos;

		dc.SelectObject(pOldPen);

		UpdateData(FALSE);
		DisplayImage();
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}




void CThresholdDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.


	CGdiObject* pOldPen = dc.SelectStockObject(DC_PEN);

	//������׷� �ڽ�
	dc.SetDCPenColor(RGB(128, 128, 128));
	dc.MoveTo(19, 29);
	dc.LineTo(19, 200);
	dc.LineTo(276, 200);
	dc.LineTo(276, 29);
	dc.LineTo(19, 29);

	//�� �׷��̽����Ͽ� �ش��ϴ� ������׷� ���
	dc.SetDCPenColor(RGB(0, 0, 0));
	for (int i = 0; i < 256; i++) {
		dc.MoveTo(20 + i, 200);
		dc.LineTo(20 + i, 200 - m_Histogram[i]);
		//dc.Rectangle(20 + (i * 2), 220, 22 + (i * 2), 220 - m_Histogram[i]);
	}

	//>>���� ������ ǥ��
	dc.SetDCPenColor(RGB(255, 0, 0));
	if (bIs16Bit == true) {
		dc.MoveTo(20 + m_nAutoThresholdingVal/256, 30);
		dc.LineTo(20 + m_nAutoThresholdingVal/256, 200);
		m_nOldPos = m_nAutoThresholdingVal/256;
	}
	else {
		dc.MoveTo(20 + m_nAutoThresholdingVal, 30);
		dc.LineTo(20 + m_nAutoThresholdingVal, 200);
		m_nOldPos = m_nAutoThresholdingVal;
	}
	//<<

// 		//�׷��̽����� ���� ���
// 		for (int i = 0; i < 256; i++) {
// 			dc.SetDCPenColor(RGB(i, i, i));
// 			dc.MoveTo(20 + i, 130);
// 			dc.LineTo(20 + i, 145);
// 			//dc.Rectangle(20 + (i * 2), 230, 22 + (i * 2), 245);
// 		}

	dc.SelectObject(pOldPen);
}


void CThresholdDlg::OnBnClickedCheck16bit()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_ctrlCheck16Bit.GetCheck()==false) {
	}

	else {
	}

	UpdateData(false);
}
