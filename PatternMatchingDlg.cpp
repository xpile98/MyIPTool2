// PatternMatchingDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "PatternMatchingDlg.h"
#include "afxdialogex.h"


// CPatternMatchingDlg ��ȭ �����Դϴ�.

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


// CPatternMatchingDlg �޽��� ó�����Դϴ�.



BOOL CPatternMatchingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
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

		strTitle = pDoc->GetTitle();	//�ҷ��� ������ �̸��� ��ȯ����.
		m_PatternImage.InsertString(nIndex, strTitle);
		m_PatternImage.SetItemDataPtr(nIndex, (void*)pDoc);


		nIndex++;
	}
	m_PatternImage.SetCurSel(1);	//�޺� �ڽ��� ���ִ� ��ť��Ʈ ����Ʈ �߿��� ���� ù��° �Ǵ� �ι�° 

	GetDlgItem(IDC_EDIT_NCC_TH)->EnableWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CPatternMatchingDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_pDoc = (CMyIPTool2Doc*)m_PatternImage.GetItemDataPtr(m_PatternImage.GetCurSel());
	CDialogEx::OnOK();
}

void CPatternMatchingDlg::OnBnClickedRadioPmMode(UINT msg)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	GetDlgItem(IDC_EDIT_OBJECT)->EnableWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_NCC_TH)->EnableWindow(SW_HIDE);
	if (m_ctrlRadioPmMode == 0)
		GetDlgItem(IDC_EDIT_OBJECT)->EnableWindow(SW_SHOW);
	else if (m_ctrlRadioPmMode == 1)
		GetDlgItem(IDC_EDIT_NCC_TH)->EnableWindow(SW_SHOW);
}
