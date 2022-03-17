// ArithmeticLogicalDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "ArithmeticLogicalDlg.h"
#include "afxdialogex.h"
#include "MyIPTool2.h"
#include "MyIPTool2Doc.h"
#include "MyIPConvert.h"	//190412 YWCho image resize(picture control)
#include "MyIPEnhance.h"	//190414 YWCho result preview

// CArithmeticLogicalDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CArithmeticLogicalDlg, CDialogEx)

CArithmeticLogicalDlg::CArithmeticLogicalDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ARITHMETIC_LOGICAL, pParent)
	, m_nFunction(0), m_pDoc1(NULL), m_pDoc2(NULL)
	, m_editConstant(0), m_bUseConstant(false), bIs16Bit(false)
	, bResultFLAG(false)
{

}

CArithmeticLogicalDlg::~CArithmeticLogicalDlg()
{
}

void CArithmeticLogicalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_IMAGE1, m_comboImage1);
	DDX_Control(pDX, IDC_COMBO_IMAGE2, m_comboImage2);
	DDX_Radio(pDX, IDC_RADIO_FUNCTION1, m_nFunction);
	DDX_Control(pDX, IDC_CHECK, m_UseConstant);
	DDX_Control(pDX, IDC_EDIT_CONSTANT, m_ctrlConstant);
	DDX_Text(pDX, IDC_EDIT_CONSTANT, m_editConstant);
	//DDV_MinMaxInt(pDX, m_editConstant, 0, 255); -> event 함수에서 조절 
	DDX_Control(pDX, IDC_CHECK_16BIT, m_ctrlCheck16Bit);
}


BEGIN_MESSAGE_MAP(CArithmeticLogicalDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CArithmeticLogicalDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK, &CArithmeticLogicalDlg::OnBnClickedCheck)
	ON_EN_CHANGE(IDC_EDIT_CONSTANT, &CArithmeticLogicalDlg::OnEnChangeEditConstant)
	ON_CBN_SELCHANGE(IDC_COMBO_IMAGE1, &CArithmeticLogicalDlg::OnCbnSelchangeComboImage1)
	ON_CBN_SELCHANGE(IDC_COMBO_IMAGE2, &CArithmeticLogicalDlg::OnCbnSelchangeComboImage2)
	ON_BN_CLICKED(IDC_BUTTON_RESULT, &CArithmeticLogicalDlg::OnBnClickedButtonResult)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CArithmeticLogicalDlg 메시지 처리기입니다.


BOOL CArithmeticLogicalDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	int nIndex = 0;
	CString strTitle;
	CMyIPTool2Doc* pDoc;
	CMyIPTool2App* pApp = (CMyIPTool2App*)AfxGetApp();
	POSITION pos = pApp->GetFirstDocTemplatePosition();
	CDocTemplate* tDoc = pApp->GetNextDocTemplate(pos);
	pos = tDoc->GetFirstDocPosition();
// 	CMyIPTool2App* pApp = (CMyIPTool2App*)AfxGetApp();
// 	POSITION pos = pApp->m_pImageDocTemplate->GetFirstDocPosition();

	while (pos != NULL)
	{
		pDoc = (CMyIPTool2Doc*)tDoc->GetNextDoc(pos);
		//CMyIPTool2Doc* pDoc = (CMyIPTool2Doc*)pApp->m_pImageDocTemplate->GetNextDoc(pos);

		strTitle = pDoc->GetTitle();	//불러온 영상의 이름을 반환해줌.
		m_comboImage1.InsertString(nIndex, strTitle);
		m_comboImage1.SetItemDataPtr(nIndex, (void*)pDoc);


		nIndex++;
	}
	m_comboImage1.SetCurSel(0);	//콤보 박스에 들어가있는 도큐먼트 리스트 중에서 가장 첫번째 또는 두번째 

	OnCbnSelchangeComboImage1();	//첫번째 박스를 기준으로 두번째 박스의 내용 Set

	//>>콤보박스 1선택된 이미지 표시

// 	CMyIPTool2Doc* pDocTemp = (CMyIPTool2Doc*)m_comboImage1.GetItemDataPtr(m_comboImage1.GetCurSel());
// 	int depth = pDocTemp->m_MyIPImage.GetDepth();
// 	if (depth == CV_16U || depth == CV_16S) {
// 		m_ctrlCheck16Bit.SetCheck(true);
// 		bIs16Bit = true;
// 	}
// 	else {
// 		m_ctrlCheck16Bit.SetCheck(false);
// 		bIs16Bit = false;
// 	}
	m_ctrlCheck16Bit.EnableWindow(FALSE);
	m_ctrlConstant.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CArithmeticLogicalDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pDoc1 = (CMyIPTool2Doc*)m_comboImage1.GetItemDataPtr(m_comboImage1.GetCurSel());
	m_pDoc2 = (CMyIPTool2Doc*)m_comboImage2.GetItemDataPtr(m_comboImage2.GetCurSel());

	CDialogEx::OnOK();
}


void CArithmeticLogicalDlg::OnBnClickedCheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_UseConstant.GetCheck()) {					//선택 안되면 
		m_ctrlConstant.EnableWindow(FALSE);
		m_comboImage2.EnableWindow(TRUE);
		m_bUseConstant = false;
	}
	else {												//선택 되면 
		m_ctrlConstant.EnableWindow(TRUE);
		m_comboImage2.EnableWindow(FALSE);
		m_bUseConstant = true;
		Invalidate(false);
	}
}



void CArithmeticLogicalDlg::OnEnChangeEditConstant()
{
	UpdateData(true);
	if (bIs16Bit == false) {
		if (m_editConstant < 0 || m_editConstant > 255) {
			if (m_editConstant < 0)			m_editConstant = 0;
			else if (m_editConstant > 255)	m_editConstant = 255;
			AfxMessageBox(_T("0에서 255 사이의 정수를 입력하십시오."));
		}
	}

	else {
		if (m_editConstant < 0 || m_editConstant > 65535) {
			if (m_editConstant < 0)			m_editConstant = 0;
			else if (m_editConstant > 65535)	m_editConstant = 65535;
			AfxMessageBox(_T("0에서 65535 사이의 정수를 입력하십시오."));
		}
	}
	Invalidate(false);
	UpdateData(false);
}

void CArithmeticLogicalDlg::OnCbnSelchangeComboImage1()
{
	int cursel = m_comboImage2.GetCurSel();	//콤보박스 2번의 현재 선택이미지 저장...없으면 -1

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMyIPTool2Doc* pDoc = (CMyIPTool2Doc*)m_comboImage1.GetItemDataPtr(m_comboImage1.GetCurSel());

	//>>콤보박스 1의 이미지 비트에 맞춰 콤보박스 2리스트 정렬
	int depth = pDoc->m_MyIPImage.GetDepth();
	switch (depth)
	{
	case CV_8U:
	case CV_8S:
		m_ctrlCheck16Bit.SetCheck(false);
		bIs16Bit = false;
		break;

	case CV_16U:
	case CV_16S:
		m_ctrlCheck16Bit.SetCheck(true);
		bIs16Bit = true;
		break;

	default:
		break;
	}

	int nIndex = 0;
	CString strTitle;
	CMyIPTool2App* pApp = (CMyIPTool2App*)AfxGetApp();
	POSITION pos = pApp->GetFirstDocTemplatePosition();
	CDocTemplate* tDoc = pApp->GetNextDocTemplate(pos);
	pos = tDoc->GetFirstDocPosition();


	// 	for (int i = 0; i < m_comboImage2.GetCount(); i++)	//두번째 콤보박스의 내용을 지우고 첫번째 이미지의 종류에 따라 Set해주기 
	//m_comboImage2.DeleteString(nIndex);				//위해서 내용을 다 지우기 위해 GetCount함수 썼는데 안되서 
	m_comboImage2.ResetContent();						//ResetContent함수 사용

	while (pos != NULL)
	{
		pDoc = (CMyIPTool2Doc*)tDoc->GetNextDoc(pos);

		strTitle = pDoc->GetTitle();	//불러온 영상의 이름을 반환해줌.
		int depth = pDoc->m_MyIPImage.GetDepth();
		if ((bIs16Bit == false && (depth == CV_8U || depth == CV_8S)) ||
			(bIs16Bit == true && (depth == CV_16U || depth == CV_16S))) {
			m_comboImage2.InsertString(nIndex, strTitle);
			m_comboImage2.SetItemDataPtr(nIndex, (void*)pDoc);
			nIndex++;
		}
	}


	m_comboImage2.SetCurSel(0);	//도큐먼트를 기본으로 선택하도록 SetCurSel함수를 사용하였다.
	if (nIndex > 1) {
		if (cursel > 0)
			m_comboImage2.SetCurSel(cursel);
		else
			m_comboImage2.SetCurSel(1);
	}

	//DisplayPreviewImage();	//이거 하고 invalidate함수 하면 displayprevie...()에서 afxnewimage하고, 
	//invalidate함수가 wm_paint호출->displayprevi....()함수 재호출하여 함수 두번 실행.
	Invalidate(false);	
	/*Invalidate()  을 실행시키면,
	강제로 WM_PAINT메시지를 발생, WM_PAINT메시지는 OnDraw()함수를 호출하여
	화면을 다시 그리는 역활을 한다.*/
}

void CArithmeticLogicalDlg::OnCbnSelchangeComboImage2()
{
	//DisplayPreviewImage();
	Invalidate(false);
}


void CArithmeticLogicalDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	DisplayPreviewImage();
	if (bResultFLAG == true) {
		DisplayResultImage();
		bResultFLAG = false;
	}
}

bool CArithmeticLogicalDlg::DisplayPreviewImage()
{

	m_pDoc1 = (CMyIPTool2Doc*)m_comboImage1.GetItemDataPtr(m_comboImage1.GetCurSel());
	m_pDoc2 = (CMyIPTool2Doc*)m_comboImage2.GetItemDataPtr(m_comboImage2.GetCurSel());

 	CRect rect,rect2;
 	CWnd *pImageWnd, *pImageWnd2;
 	MyIPImage img, img2, img_resize, img2_resize;
 
	pImageWnd = GetDlgItem(IDC_IMAGE_PREVIEW1);
	pImageWnd2 = GetDlgItem(IDC_IMAGE_PREVIEW2);
	pImageWnd->GetClientRect(rect);
	pImageWnd2->GetClientRect(rect2);

	//img_resize 만들기
 	img = m_pDoc1->m_MyIPImage;
	if (!IpResizeNearest(img, img_resize, rect.Height(), rect.Width())) 
		return false;
	
	//img2_resize만들기 
	if (m_bUseConstant == false) {		//constant -> notuse 
		img2 = m_pDoc2->m_MyIPImage;
		if (!IpResizeNearest(img2, img2_resize, rect.Height(), rect.Width()))	
			return false;
	}
	else if (m_bUseConstant == true) {	//constant -> use 
		Mat preview(img_resize.GetHeight(), img_resize.GetWidth(), CV_8UC1);
		preview.setTo(m_editConstant);
		if (!IpMatToImage(preview, img2_resize)) 
			return false;
	}


	CPaintDC dcPreview1(GetDlgItem(IDC_IMAGE_PREVIEW1));
	CPaintDC dcPreview2(GetDlgItem(IDC_IMAGE_PREVIEW2));
	img_resize.Draw(dcPreview1.m_hDC, 0, 0);
	img2_resize.Draw(dcPreview2.m_hDC, 0, 0);

	return true;
}

bool CArithmeticLogicalDlg::DisplayResultImage()
{
	UpdateData(TRUE);
	m_pDoc1 = (CMyIPTool2Doc*)m_comboImage1.GetItemDataPtr(m_comboImage1.GetCurSel());
	m_pDoc2 = (CMyIPTool2Doc*)m_comboImage2.GetItemDataPtr(m_comboImage2.GetCurSel());

	CRect rect3;
	CWnd *pImageWnd3;

	pImageWnd3 = GetDlgItem(IDC_IMAGE_PREVIEW3);
	pImageWnd3->GetClientRect(rect3);

	MyIPImage result, result_resize;


	if (m_bUseConstant == false) {
		if (!IP_ARLogic(m_pDoc1->m_MyIPImage, m_pDoc2->m_MyIPImage, result, m_nFunction))
			return false;
	}
	else if (m_bUseConstant == true) {
		if (!IP_ARLogic(m_pDoc1->m_MyIPImage, m_pDoc2->m_MyIPImage, result, m_nFunction, TRUE, m_editConstant))
			return false;
	}

	if (!IpResizeNearest(result, result_resize, rect3.Height(), rect3.Width()))
		return false;

	CPaintDC dcPreview3(GetDlgItem(IDC_IMAGE_PREVIEW3));
	result_resize.Draw(dcPreview3.m_hDC, 0, 0);

	bResultFLAG = true;
	return true;
}


void CArithmeticLogicalDlg::OnBnClickedButtonResult()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	DisplayResultImage();
	Invalidate(false);
}
