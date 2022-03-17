// MorphAlgorithmDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "MorphAlgorithmDlg.h"
#include "afxdialogex.h"


// CMorphAlgorithmDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMorphAlgorithmDlg, CDialogEx)

CMorphAlgorithmDlg::CMorphAlgorithmDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MORPH_ALGORITHM, pParent)
{

}

CMorphAlgorithmDlg::~CMorphAlgorithmDlg()
{
}

void CMorphAlgorithmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboBox);
}


BEGIN_MESSAGE_MAP(CMorphAlgorithmDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMorphAlgorithmDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMorphAlgorithmDlg 메시지 처리기입니다.


BOOL CMorphAlgorithmDlg::OnInitDialog()
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
		m_comboBox.InsertString(nIndex, strTitle);
		m_comboBox.SetItemDataPtr(nIndex, (void*)pDoc);


		nIndex++;
	}
	m_comboBox.SetCurSel(0);	//콤보 박스에 들어가있는 도큐먼트 리스트 중에서 가장 첫번째 또는 두번째 


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



void CMorphAlgorithmDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pDoc = (CMyIPTool2Doc*)m_comboBox.GetItemDataPtr(m_comboBox.GetCurSel());
	CDialogEx::OnOK();
}
