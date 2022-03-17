
// MyIPTool2.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "MyIPTool2.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "MyIPTool2Doc.h"
#include "MyIPTool2View.h"
#include "MyIPConvert.h"		//190426 YWCho Webcam 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyIPTool2App

BEGIN_MESSAGE_MAP(CMyIPTool2App, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CMyIPTool2App::OnAppAbout)
	// ǥ�� ������ ���ʷ� �ϴ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// ǥ�� �μ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_WINDOW_CLOSEALL, &CMyIPTool2App::OnWindowCloseall)
	ON_COMMAND(ID_WEBCAM_LIVECAM, &CMyIPTool2App::OnWebcamLivecam)
END_MESSAGE_MAP()


// CMyIPTool2App ����

CMyIPTool2App::CMyIPTool2App()
{
	m_bHiColorIcons = TRUE;
	m_pMat = (Mat*)NULL;			//190227 ���� �� â���� ����, ������� �ʱ�ȭ 
	m_pImage = (MyIPImage*)NULL;	//190227 ���� �� â���� ����, ������� �ʱ�ȭ 
	m_sTitle = _T("");

	// �ٽ� ���� ������ ����
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���� ���α׷��� ���� ��� ��Ÿ�� ������ ����Ͽ� ������ ���(/clr):
	//     1) �� �߰� ������ �ٽ� ���� ������ ������ ����� �۵��ϴ� �� �ʿ��մϴ�.
	//     2) ������Ʈ���� �����Ϸ��� System.Windows.Forms�� ���� ������ �߰��ؾ� �մϴ�.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: �Ʒ� ���� ���α׷� ID ���ڿ��� ���� ID ���ڿ��� �ٲٽʽÿ�(����).
	// ���ڿ��� ���� ����: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("MyIPTool2.AppID.NoVersion"));

	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}

// ������ CMyIPTool2App ��ü�Դϴ�.

CMyIPTool2App theApp;


// CMyIPTool2App �ʱ�ȭ

BOOL CMyIPTool2App::InitInstance()
{
	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControlsEx()�� �ʿ��մϴ�. 
	// InitCommonControlsEx()�� ������� ������ â�� ���� �� �����ϴ�.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ���� ���α׷����� ����� ��� ���� ��Ʈ�� Ŭ������ �����ϵ���
	// �� �׸��� �����Ͻʽÿ�.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// OLE ���̺귯���� �ʱ�ȭ�մϴ�.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit ��Ʈ���� ����Ϸ���  AfxInitRichEdit2()�� �־�� �մϴ�.	
	// AfxInitRichEdit2();

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));
	LoadStdProfileSettings(4);  // MRU�� �����Ͽ� ǥ�� INI ���� �ɼ��� �ε��մϴ�.


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ���� ���α׷��� ���� ���ø��� ����մϴ�.  ���� ���ø���
	//  ����, ������ â �� �� ������ ���� ������ �մϴ�.
	//CMultiDocTemplate* pDocTemplate;
	m_pImageDocTemplate = new CMultiDocTemplate(IDR_MyIPTool2TYPE,	//190313 ��ť��Ʈ ���ø� ���������� ��� 
		RUNTIME_CLASS(CMyIPTool2Doc),
		RUNTIME_CLASS(CChildFrame), // ����� ���� MDI �ڽ� �������Դϴ�.
		RUNTIME_CLASS(CMyIPTool2View));
	if (!m_pImageDocTemplate)
		return FALSE;
	AddDocTemplate(m_pImageDocTemplate);

	// �� MDI ������ â�� ����ϴ�.
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// ���̻簡 ���� ��쿡�� DragAcceptFiles�� ȣ���մϴ�.
	//  MDI ���� ���α׷������� m_pMainWnd�� ������ �� �ٷ� �̷��� ȣ���� �߻��ؾ� �մϴ�.
	// ��� ���⿡ ���� ���⸦ Ȱ��ȭ�մϴ�.
	m_pMainWnd->DragAcceptFiles();

	// ǥ�� �� ���, DDE, ���� ���⿡ ���� ������� ���� �м��մϴ�.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	//���α׷� ���� �� �� â�� ����� �ʽ��ϴ�.
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;


// 	// DDE Execute ���⸦ Ȱ��ȭ�մϴ�.
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// ����ٿ� ������ ����� ����ġ�մϴ�.
	// ���� ���α׷��� /RegServer, /Register, /Unregserver �Ǵ� /Unregister�� ���۵� ��� FALSE�� ��ȯ�մϴ�.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	
	//�巹�� �� ������� ���� ���� ���� //190304
	//pMainFrame->DragAcceptFiles(true);


	// �� â�� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

// 	FILE *fp = fopen("test.csv", "a");	//190501 YWCho create csv file
// 	fprintf(fp, "Size_A, Size_B,");
// 	fprintf(fp, "Dilation1_Border,Dilation1_BorderX,Dilation2_Border,Dilation2_BorderX,");
// 	fprintf(fp, "Erosion1_Border,Erosion1_BorderX,Erosion2_Border,Erosion2_BorderX,\n");
// 	fclose(fp);							//190501 YWCho create csv file
	return TRUE;
}

int CMyIPTool2App::ExitInstance()
{
	//TODO: �߰��� �߰� ���ҽ��� ó���մϴ�.
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CMyIPTool2App �޽��� ó����


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
//	afx_msg void OnClose();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_ACTIVATE()
ON_WM_CLOSE()
END_MESSAGE_MAP()

// ��ȭ ���ڸ� �����ϱ� ���� ���� ���α׷� ����Դϴ�.
void CMyIPTool2App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMyIPTool2App ����� ���� �ε�/���� �޼���

void CMyIPTool2App::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CMyIPTool2App::LoadCustomState()
{
}

void CMyIPTool2App::SaveCustomState()
{
}

// CMyIPTool2App �޽��� ó����
void AfxNewImage(MyIPImage & image)
{
	theApp.m_pImage = &image;	//ġȯ�����ڸ� ������� pg142
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}

void AfxNewImage(Mat & mat)
{
	theApp.m_pMat = &mat;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}

void AfxNewImage(CString title, MyIPImage & image)
{
	theApp.m_pImage = &image;	//ġȯ�����ڸ� ������� pg142
	theApp.m_sTitle = title;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}

void AfxNewImage(CString title, Mat & mat)
{
	theApp.m_pMat = &mat;
	theApp.m_sTitle = title;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}

void AfxPrintInfo(CString message)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_wndOutput.AddString(message);
}

void AfxPrintInfo(LPCTSTR IpszFormat, ...)
{
	CString message;

	va_list argList;
	va_start(argList, IpszFormat);
	message.FormatV(IpszFormat, argList);
	va_end(argList);

	AfxPrintInfo(message);
}



void CMyIPTool2App::OnWindowCloseall()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CloseAllDocuments(TRUE);
}


void CMyIPTool2App::OnWebcamLivecam()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	VideoCapture capture(0);	//default camera open
	Mat video;
	namedWindow("camera",WINDOW_AUTOSIZE);			//create window for camera 
	MyIPImage img;
// 	Size size(640, 360);
// 	capture.set(CAP_PROP_FRAME_WIDTH, size.width);
// 	capture.set(CAP_PROP_FRAME_HEIGHT, size.height);
// 	int w = capture.get(CAP_PROP_FRAME_WIDTH);		//can get camera info (640)
// 	int h = capture.get(CAP_PROP_FRAME_HEIGHT);		//					  (480)x (360)o
	while (true) {
		capture >> video;
		imshow("camera", video);	//show camera on window 
		//AfxNewImage(_T("webcam"), video);

		if (waitKeyEx(1) > 0) {
			IpMatToImage(video, img);
			break;
		}
	}
	destroyAllWindows();
	AfxNewImage(_T("webcam capture"), img);
}
