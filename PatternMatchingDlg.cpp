// PatternMatchingDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "PatternMatchingDlg.h"
#include "afxdialogex.h"


// CPatternMatchingDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPatternMatchingDlg, CDialogEx)

CPatternMatchingDlg::CPatternMatchingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PATTERNMATCHING_DLG, pParent)
	, m_nObjects(1)
	, m_fNCCThreshold(0.8f)
	, m_ctrlRadioPmMode(0)
{

}

CPatternMatchingDlg::~CPatternMatchingDlg()
{
}

void CPatternMatchingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PIMAGE, m_PatternImage);
	DDX_Text(pDX, IDC_EDIT_OBJECT, m_nObjects);
	DDX_Text(pDX, IDC_EDIT_NCC_TH, m_fNCCThreshold);
	DDX_Radio(pDX, IDC_RADIO_PM_MODE1, m_ctrlRadioPmMode);
}


BEGIN_MESSAGE_MAP(CPatternMatchingDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPatternMatchingDlg::OnBnClickedOk)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_PM_MODE1, IDC_RADIO_PM_MODE2, &CPatternMatchingDlg::OnBnClickedRadioPmMode)
END_MESSAGE_MAP()


// CPatternMatchingDlg 메시지 처리기입니다.



BOOL CPatternMatchingDlg::OnInitDialog()
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
		m_PatternImage.InsertString(nIndex, strTitle);
		m_PatternImage.SetItemDataPtr(nIndex, (void*)pDoc);


		nIndex++;
	}
	m_PatternImage.SetCurSel(1);	//콤보 박스에 들어가있는 도큐먼트 리스트 중에서 가장 첫번째 또는 두번째 

	GetDlgItem(IDC_EDIT_NCC_TH)->EnableWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CPatternMatchingDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pDoc = (CMyIPTool2Doc*)m_PatternImage.GetItemDataPtr(m_PatternImage.GetCurSel());
	CDialogEx::OnOK();
}

void CPatternMatchingDlg::OnBnClickedRadioPmMode(UINT msg)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	GetDlgItem(IDC_EDIT_OBJECT)->EnableWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_NCC_TH)->EnableWindow(SW_HIDE);
	if (m_ctrlRadioPmMode == 0)
		GetDlgItem(IDC_EDIT_OBJECT)->EnableWindow(SW_SHOW);
	else if (m_ctrlRadioPmMode == 1)
		GetDlgItem(IDC_EDIT_NCC_TH)->EnableWindow(SW_SHOW);
}
