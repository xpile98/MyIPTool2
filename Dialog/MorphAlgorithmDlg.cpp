// MorphAlgorithmDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyIPTool2.h"
#include "MorphAlgorithmDlg.h"
#include "afxdialogex.h"


// CMorphAlgorithmDlg ��ȭ �����Դϴ�.

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


// CMorphAlgorithmDlg �޽��� ó�����Դϴ�.


BOOL CMorphAlgorithmDlg::OnInitDialog()
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
		m_comboBox.InsertString(nIndex, strTitle);
		m_comboBox.SetItemDataPtr(nIndex, (void*)pDoc);


		nIndex++;
	}
	m_comboBox.SetCurSel(0);	//�޺� �ڽ��� ���ִ� ��ť��Ʈ ����Ʈ �߿��� ���� ù��° �Ǵ� �ι�° 


	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}



void CMorphAlgorithmDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_pDoc = (CMyIPTool2Doc*)m_comboBox.GetItemDataPtr(m_comboBox.GetCurSel());
	CDialogEx::OnOK();
}
