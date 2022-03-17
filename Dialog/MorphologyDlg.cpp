// MorphologyDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "MorphologyDlg.h"
#include "afxdialogex.h"


// CMorphologyDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMorphologyDlg, CDialogEx)

CMorphologyDlg::CMorphologyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MORPHOLOGY, pParent)
	, m_nMorphologyType(0), m_nSEType(0)
	, m_nRadius(3)
	, m_nRectHeight(3), m_nRectWidth(3)
	, bGrayMorphFlag(false)
{

}

CMorphologyDlg::~CMorphologyDlg()
{
}

void CMorphologyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_MORPH_TYPE1, m_nMorphologyType);
	DDX_Radio(pDX, IDC_RADIO_SE_TYPE1, m_nSEType);
	DDX_Text(pDX, IDC_EDIT_DISK_RADIUS, m_nRadius);
	DDX_Text(pDX, IDC_EDIT_RECT_H, m_nRectHeight);
	DDX_Text(pDX, IDC_EDIT_RECT_W, m_nRectWidth);
	DDX_Control(pDX, IDC_COMBO1, m_ctrlComboBox);
}


BEGIN_MESSAGE_MAP(CMorphologyDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_MORPH_TYPE2, &CMorphologyDlg::OnBnClickedRadioMorphType)
	ON_BN_CLICKED(IDC_RADIO_MORPH_TYPE1, &CMorphologyDlg::OnBnClickedRadioMorphType)
	ON_BN_CLICKED(IDC_RADIO_MORPH_TYPE4, &CMorphologyDlg::OnBnClickedRadioMorphType)
	ON_BN_CLICKED(IDC_RADIO_MORPH_TYPE3, &CMorphologyDlg::OnBnClickedRadioMorphType)
	ON_BN_CLICKED(IDC_RADIO_MORPH_TYPE5, &CMorphologyDlg::OnBnClickedRadioMorphType)
	ON_BN_CLICKED(IDC_RADIO_MORPH_TYPE6, &CMorphologyDlg::OnBnClickedRadioMorphType)
	ON_BN_CLICKED(IDC_RADIO_MORPH_TYPE7, &CMorphologyDlg::OnBnClickedRadioMorphType)
	ON_BN_CLICKED(IDC_RADIO_SE_TYPE1, &CMorphologyDlg::OnBnClickedRadioSeType)
	ON_BN_CLICKED(IDC_RADIO_SE_TYPE2, &CMorphologyDlg::OnBnClickedRadioSeType)
	ON_BN_CLICKED(IDC_RADIO_SE_TYPE3, &CMorphologyDlg::OnBnClickedRadioSeType)
	ON_BN_CLICKED(IDC_RADIO_SE_TYPE4, &CMorphologyDlg::OnBnClickedRadioSeType)
	ON_BN_CLICKED(IDC_RADIO_SE_TYPE5, &CMorphologyDlg::OnBnClickedRadioSeType)
	ON_BN_CLICKED(IDC_RADIO_SE_TYPE6, &CMorphologyDlg::OnBnClickedRadioSeType)
	ON_BN_CLICKED(IDOK, &CMorphologyDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMorphologyDlg �޽��� ó�����Դϴ�.


void CMorphologyDlg::OnBnClickedRadioMorphType()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_DISK_RADIUS)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_RECT_H)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_RECT_W)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_SE_TYPE1)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_SE_TYPE2)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_SE_TYPE3)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_SE_TYPE4)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_SE_TYPE5)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_SE_TYPE6)->EnableWindow(TRUE);
	if (m_nMorphologyType == 4) {		//conditional Dialation
		GetDlgItem(IDC_COMBO1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DISK_RADIUS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RECT_H)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RECT_W)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SE_TYPE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SE_TYPE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SE_TYPE3)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SE_TYPE4)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SE_TYPE5)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SE_TYPE6)->EnableWindow(FALSE);
	}
}



void CMorphologyDlg::OnBnClickedRadioSeType()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_DISK_RADIUS)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_RECT_H)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_RECT_W)->EnableWindow(FALSE);

	if (m_nSEType == SE_DISK) {		
		GetDlgItem(IDC_EDIT_DISK_RADIUS)->EnableWindow(TRUE);
	}
	else if (m_nSEType == SE_RING) {
		GetDlgItem(IDC_EDIT_DISK_RADIUS)->EnableWindow(TRUE);
	}
	else if (m_nSEType == SE_RECTANGLE) {
		GetDlgItem(IDC_EDIT_RECT_H)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RECT_W)->EnableWindow(TRUE);
	}
	else if (m_nSEType == SE_FRAME) {
		GetDlgItem(IDC_EDIT_RECT_H)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RECT_W)->EnableWindow(TRUE);
	}
	else if (m_nSEType == SE_CROSS) {
		GetDlgItem(IDC_EDIT_RECT_H)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RECT_W)->EnableWindow(TRUE);
	}
	else if (m_nSEType == SE_DIAMOND) {
		GetDlgItem(IDC_EDIT_RECT_H)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RECT_W)->EnableWindow(TRUE);
	}
}



BOOL CMorphologyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	OnBnClickedRadioMorphType();
	OnBnClickedRadioSeType();
	if (bGrayMorphFlag) {
		GetDlgItem(IDC_RADIO_MORPH_TYPE6)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_MORPH_TYPE7)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_RADIO_MORPH_TYPE6)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_MORPH_TYPE7)->EnableWindow(FALSE);
	}


	//ComboBox Programming
	int nIndex = 0;
	CString strTitle;
	CMyIPTool2Doc* pDoc;
	CMyIPTool2App* pApp = (CMyIPTool2App*)AfxGetApp();
	POSITION pos = pApp->GetFirstDocTemplatePosition();
	CDocTemplate* tDoc = pApp->GetNextDocTemplate(pos);
	pos = tDoc->GetFirstDocPosition();

	while (pos != NULL)
	{
		pDoc = (CMyIPTool2Doc*)tDoc->GetNextDoc(pos);
		//CMyIPTool2Doc* pDoc = (CMyIPTool2Doc*)pApp->m_pImageDocTemplate->GetNextDoc(pos);

		strTitle = pDoc->GetTitle();	//�ҷ��� ������ �̸��� ��ȯ����.
		m_ctrlComboBox.InsertString(nIndex, strTitle);
		m_ctrlComboBox.SetItemDataPtr(nIndex, (void*)pDoc);


		nIndex++;
	}
	m_ctrlComboBox.SetCurSel(0);	//�޺� �ڽ��� ���ִ� ��ť��Ʈ ����Ʈ �߿��� ���� ù��° �Ǵ� �ι�° 



	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CMorphologyDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_pDoc1 = (CMyIPTool2Doc*)m_ctrlComboBox.GetItemDataPtr(m_ctrlComboBox.GetCurSel());
	CDialogEx::OnOK();
}
