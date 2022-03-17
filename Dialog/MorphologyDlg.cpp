// MorphologyDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "MorphologyDlg.h"
#include "afxdialogex.h"


// CMorphologyDlg 대화 상자입니다.

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


// CMorphologyDlg 메시지 처리기입니다.


void CMorphologyDlg::OnBnClickedRadioMorphType()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
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

		strTitle = pDoc->GetTitle();	//불러온 영상의 이름을 반환해줌.
		m_ctrlComboBox.InsertString(nIndex, strTitle);
		m_ctrlComboBox.SetItemDataPtr(nIndex, (void*)pDoc);


		nIndex++;
	}
	m_ctrlComboBox.SetCurSel(0);	//콤보 박스에 들어가있는 도큐먼트 리스트 중에서 가장 첫번째 또는 두번째 



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CMorphologyDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pDoc1 = (CMyIPTool2Doc*)m_ctrlComboBox.GetItemDataPtr(m_ctrlComboBox.GetCurSel());
	CDialogEx::OnOK();
}
