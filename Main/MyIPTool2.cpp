
// MyIPTool2.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
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
	// 표준 파일을 기초로 하는 문서 명령입니다.
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 표준 인쇄 설정 명령입니다.
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_WINDOW_CLOSEALL, &CMyIPTool2App::OnWindowCloseall)
	ON_COMMAND(ID_WEBCAM_LIVECAM, &CMyIPTool2App::OnWebcamLivecam)
END_MESSAGE_MAP()


// CMyIPTool2App 생성

CMyIPTool2App::CMyIPTool2App()
{
	m_bHiColorIcons = TRUE;
	m_pMat = (Mat*)NULL;			//190227 영상 새 창으로 띄우기, 멤버변수 초기화 
	m_pImage = (MyIPImage*)NULL;	//190227 영상 새 창으로 띄우기, 멤버변수 초기화 
	m_sTitle = _T("");

	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 응용 프로그램을 공용 언어 런타임 지원을 사용하여 빌드한 경우(/clr):
	//     1) 이 추가 설정은 다시 시작 관리자 지원이 제대로 작동하는 데 필요합니다.
	//     2) 프로젝트에서 빌드하려면 System.Windows.Forms에 대한 참조를 추가해야 합니다.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 아래 응용 프로그램 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("MyIPTool2.AppID.NoVersion"));

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CMyIPTool2App 개체입니다.

CMyIPTool2App theApp;


// CMyIPTool2App 초기화

BOOL CMyIPTool2App::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit 컨트롤을 사용하려면  AfxInitRichEdit2()가 있어야 합니다.	
	// AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 응용 프로그램의 문서 템플릿을 등록합니다.  문서 템플릿은
	//  문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
	//CMultiDocTemplate* pDocTemplate;
	m_pImageDocTemplate = new CMultiDocTemplate(IDR_MyIPTool2TYPE,	//190313 도큐먼트 템플릿 전역변수로 사용 
		RUNTIME_CLASS(CMyIPTool2Doc),
		RUNTIME_CLASS(CChildFrame), // 사용자 지정 MDI 자식 프레임입니다.
		RUNTIME_CLASS(CMyIPTool2View));
	if (!m_pImageDocTemplate)
		return FALSE;
	AddDocTemplate(m_pImageDocTemplate);

	// 주 MDI 프레임 창을 만듭니다.
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// 접미사가 있을 경우에만 DragAcceptFiles를 호출합니다.
	//  MDI 응용 프로그램에서는 m_pMainWnd를 설정한 후 바로 이러한 호출이 발생해야 합니다.
	// 끌어서 놓기에 대한 열기를 활성화합니다.
	m_pMainWnd->DragAcceptFiles();

	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	//프로그램 시작 시 빈 창을 띄우지 않습니다.
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;


// 	// DDE Execute 열기를 활성화합니다.
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	
	//드레그 앤 드롭으로 영상 파일 열기 //190304
	//pMainFrame->DragAcceptFiles(true);


	// 주 창이 초기화되었으므로 이를 표시하고 업데이트합니다.
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
	//TODO: 추가한 추가 리소스를 처리합니다.
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CMyIPTool2App 메시지 처리기


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CMyIPTool2App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMyIPTool2App 사용자 지정 로드/저장 메서드

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

// CMyIPTool2App 메시지 처리기
void AfxNewImage(MyIPImage & image)
{
	theApp.m_pImage = &image;	//치환연산자를 해줘야함 pg142
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}

void AfxNewImage(Mat & mat)
{
	theApp.m_pMat = &mat;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}

void AfxNewImage(CString title, MyIPImage & image)
{
	theApp.m_pImage = &image;	//치환연산자를 해줘야함 pg142
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
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CloseAllDocuments(TRUE);
}


void CMyIPTool2App::OnWebcamLivecam()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
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
