
// MyIPTool2Doc.cpp : CMyIPTool2Doc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MyIPTool2.h"
#endif

#include "MyIPTool2Doc.h"
#include "MyIPImage.h"
#include "MyIPConvert.h"
#include "MyIPEnhance.h"
#include "MyIPAutoThreshold.h"		//190328 YWCho Auto Threshold Header 추가 
#include "MyIPSpetialFilter.h"		//190408 YWCho Spetial Filter Header 추가
#include "MyIPEdge.h"				//190418 YWCho Edge Detectir Header 추가 
#include "MyIPMorphology.h"			//190425 YWCho Morphology Header 추가 
#include "MyIPFreqFilter.h"			//190509 YWCho Frequency Filter Header 추가
#include "MyIPObjectDetection.h"	//190513 YWCho ObjectDetection Header 추가 
#include "MyIPGeometry.h"			//220310 YWCho Geometric transformation Header 추가 

#include "BrightnessContrastDlg.h"
#include "GammaDlg.h"
#include "HistogramDlg.h"
#include "ColorCombineDlg.h"		//190313 YWCho 색상 합치기 다이얼로그 추가
#include "ArithmeticLogicalDlg.h"	//190319 YWCho 산술 및 논리연산 다이얼로그 추가
#include "ThresholdDlg.h"			//190325 YWCho 이진화 다이얼로그 추가 
#include "CVThresholdDlg.h"			//190402 YWCho CV Thresholding Dlg 추가
#include "SpetialFilterDlg.h"		//190408 YWCho 공간필트 Dlg추가 
#include "NoiseDlg.h"				//190412 YWCho 노이즈 Dlg 추가
#include "ShapeningFilterDlg.h"		//190416 YWCho 샤프닝 Dlg 추가 
#include "LineProfileDlg.h"			//190416 YWCho Line Profile Dlg 추가(테스트)
#include "EdgeDetectorDlg.h"		//190418 YWCho 엣지검출 Dlg 추가 
#include "MorphologyDlg.h"			//190426 YWCho 모폴로지 Dlg 추가
#include "LoopDlg.h"				//190501 YWCho 모폴로지 테스트 Dlg 추가
#include "FrequencyFilterDlg.h"		//190509 YWCho 주파수 필터 Dlg 추가 
#include "PatternMatchingDlg.h"		//190513 YWCho 패턴매칭 Dlg 추가 
#include "HoughLineDlg.h"			//190515 YWCho 허프변환 Dlg 추가
#include "ContourThresoldDlg.h"		//220307 YWCho 컨투어 유클라디안거리 방식 Dlg 추가
#include "TranslationDlg.h"			//220310 YWCho 영상 평행이동 Dlg 추가

#include <propkey.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMyIPTool2Doc

IMPLEMENT_DYNCREATE(CMyIPTool2Doc, CDocument)

BEGIN_MESSAGE_MAP(CMyIPTool2Doc, CDocument)
	ON_COMMAND(ID_INVERSE, &CMyIPTool2Doc::OnInverse)
	ON_COMMAND(ID_BRIGHTNESS_CONTRAST, &CMyIPTool2Doc::OnBrightnessContrast)
	ON_COMMAND(ID_GAMMA, &CMyIPTool2Doc::OnGamma)
	ON_COMMAND(ID_VIEW_HISTOGRAM, &CMyIPTool2Doc::OnViewHistogram)
	ON_COMMAND(ID_HISTO_STRETCHING, &CMyIPTool2Doc::OnHistoStretching)
	ON_COMMAND(ID_HISTO_EQUALIZATION, &CMyIPTool2Doc::OnHistoEqualization)
	ON_COMMAND(ID_RGBTOGRAY, &CMyIPTool2Doc::OnRgbtogray)
	ON_COMMAND(ID_COLOR_SPLIT_RGB, &CMyIPTool2Doc::OnColorSplitRGB)
	ON_COMMAND(ID_COLOR_SPLIT_HSI, &CMyIPTool2Doc::OnColorSplitHSI)
	ON_COMMAND(ID_COLOR_SPLIT_YUV, &CMyIPTool2Doc::OnColorSplitYuv)
	ON_COMMAND(ID_OPENCV_RGBTOGRAY, &CMyIPTool2Doc::OnOpencvRgbtogray)
	ON_COMMAND(ID_OPENCV_COLOR_SPLIT_RGB, &CMyIPTool2Doc::OnOpencvColorSplitRgb)
	ON_COMMAND(ID_OPENCV_COLOR_SPLIT_HSI, &CMyIPTool2Doc::OnOpencvColorSplitHsi)
	ON_COMMAND(ID_OPENCV_COLOR_SPLIT_YUV, &CMyIPTool2Doc::OnOpencvColorSplitYuv)
	ON_COMMAND(ID_OPENCV_COLOR_SPLIT_HSV, &CMyIPTool2Doc::OnOpencvColorSplitHsv)
	ON_COMMAND(ID_OPENCV_COLOR_SPLIT_YCRCB, &CMyIPTool2Doc::OnOpencvColorSplitYcrcb)
	ON_COMMAND(ID_OPENCV_COLOR_SPLIT_LAB, &CMyIPTool2Doc::OnOpencvColorSplitLab)
	ON_COMMAND(ID_OPENCV_COLOR_SPLIT_CMY, &CMyIPTool2Doc::OnOpencvColorSplitCmy)
	ON_COMMAND(ID_OPENCV_COLOR_SPLIT_CMYK, &CMyIPTool2Doc::OnOpencvColorSplitCmyk)
	ON_COMMAND(ID_COLOR_COMBINE_RGB, &CMyIPTool2Doc::OnColorCombineRgb)
	ON_COMMAND(ID_COLOR_COMBINE_HSI, &CMyIPTool2Doc::OnColorCombineHsi)
	ON_COMMAND(ID_COLOR_COMBINE_YUV, &CMyIPTool2Doc::OnColorCombineYuv)
	ON_COMMAND(ID_OPENCV_HISTOGRAM, &CMyIPTool2Doc::OnOpencvHistogram)
	ON_COMMAND(ID_ARITHMETICLOGICAL, &CMyIPTool2Doc::OnArithmeticlogical)
	ON_COMMAND(ID_CV_ARITHMETICLOGICAL, &CMyIPTool2Doc::OnCvArithmeticlogical)
	ON_COMMAND(ID_THRESHOLD, &CMyIPTool2Doc::OnThreshold)
	ON_COMMAND(ID_BITPLANE, &CMyIPTool2Doc::OnBitplane)
	ON_COMMAND(ID_AUTOMATICTHRESHOLD_ITERATIVE, &CMyIPTool2Doc::OnAutomaticthresholdIterative)
	ON_COMMAND(ID_AUTOMATICTHRESHOLD_OTSU, &CMyIPTool2Doc::OnAutomaticthresholdOtsu)
	ON_COMMAND(ID_CV_AUTOMATICTHRESHOLD, &CMyIPTool2Doc::OnCvAutomaticthreshold)
	ON_COMMAND(ID_AUTOMATICTHRESHOLD_ENTROPY, &CMyIPTool2Doc::OnAutomaticthresholdEntropy)
	ON_COMMAND(ID_CV_HISTO_EQUALIZATION, &CMyIPTool2Doc::OnCvHistoEqualization)
	ON_COMMAND(ID_CONVERT8BITTO16BIT, &CMyIPTool2Doc::OnConvert8bitto16bit)
	ON_COMMAND(ID_SPATIALFILTER_AVERAGE, &CMyIPTool2Doc::OnSpatialfilterAverage)
	ON_COMMAND(ID_CV_SPATIALFILTER_AVERAGE, &CMyIPTool2Doc::OnCvSpatialfilterAverage)
	ON_COMMAND(ID_SPATIALFILTER_GAUSSIAN, &CMyIPTool2Doc::OnSpatialfilterGaussian)
// 	ON_COMMAND(ID_ADDNOISE_GAUSSIAN, &CMyIPTool2Doc::OnAddnoiseGaussian)		//>>Addnoise로 통합
// 	ON_COMMAND(ID_ADDNOISE_SALTPEPPER, &CMyIPTool2Doc::OnAddnoiseSaltpepper)
	ON_COMMAND(ID_SPATIALFILTER_MAXIMUM, &CMyIPTool2Doc::OnSpatialfilterMaximum)
	ON_COMMAND(ID_SPATIALFILTER_MINUMUM, &CMyIPTool2Doc::OnSpatialfilterMinumum)
	ON_COMMAND(ID_MYIPIMAGE_ADDNOISE, &CMyIPTool2Doc::OnMyipimageAddnoise)
	ON_COMMAND(ID_CV_SPATIALFILTER_GAUSSIAN, &CMyIPTool2Doc::OnCvSpatialfilterGaussian)
	ON_COMMAND(ID_CV_SPATIALFILTER_MAXIMUM, &CMyIPTool2Doc::OnCvSpatialfilterMaximum)
	ON_COMMAND(ID_CV_SPATIALFILTER_MINUMUM, &CMyIPTool2Doc::OnCvSpatialfilterMinumum)
	ON_COMMAND(ID_CV_SPATIALFILTER_MEDIAN, &CMyIPTool2Doc::OnCvSpatialfilterMedian)
	ON_COMMAND(ID_SPATIALFILTER_ADAPTIVE_MEDIAN, &CMyIPTool2Doc::OnSpatialfilterAdaptiveMedian)
	ON_COMMAND(ID_SPATIALFILTER_MEDIAN, &CMyIPTool2Doc::OnSpatialfilterMedian)
	ON_COMMAND(ID_SHARPENING_HIGHBOOSTFILTER, &CMyIPTool2Doc::OnSharpeningHighboostfilter)
	ON_COMMAND(ID_CV_SPATIALFILTER_ADAPTIVE_MEDIAN, &CMyIPTool2Doc::OnCvSpatialfilterAdaptiveMedian)
	//ON_COMMAND(ID_LINE_PROFILE, &CMyIPTool2Doc::OnLineProfile) -> view
	ON_COMMAND(ID_SPATIALFILTER_EDGEDETECTOR, &CMyIPTool2Doc::OnSpatialfilterEdgedetector)
	ON_COMMAND(ID_SHARPENING_LAPLACIANSHARPENING, &CMyIPTool2Doc::OnSharpeningLaplaciansharpening)
	ON_COMMAND(ID_MORPHOLOGY, &CMyIPTool2Doc::OnMorphology)
	ON_COMMAND(ID_GRAYSCALE_MORPHOLOGY, &CMyIPTool2Doc::OnGrayscaleMorphology)
	ON_COMMAND(ID_CV_MORPHOLOGY, &CMyIPTool2Doc::OnCvMorphology)
	ON_COMMAND(ID_MORPHOLOGY_TEST, &CMyIPTool2Doc::OnMorphologyTest)
	ON_COMMAND(ID_WEBCAM_LIVECAM, &CMyIPTool2Doc::OnWebcamLivecam)
	ON_COMMAND(ID_FOURIER_TRANSFORM, &CMyIPTool2Doc::OnFourierTransform)
	ON_COMMAND(ID_FREQ_FILTER, &CMyIPTool2Doc::OnFreqFilter)
	ON_COMMAND(ID_DFTFILTER2D_AVERAGE, &CMyIPTool2Doc::OnDftfilter2dAverage)
	ON_COMMAND(ID_DFTFILTER2D_GAUSSIAN, &CMyIPTool2Doc::OnDftfilter2dGaussian)
	ON_COMMAND(ID_OBJECTDETECTION_PATTERNMATCHING, &CMyIPTool2Doc::OnObjectdetectionPatternmatching)
	ON_COMMAND(ID_HOUGH_TRANSFORM, &CMyIPTool2Doc::OnHoughTransform)
	ON_UPDATE_COMMAND_UI(ID_OPENCV_RGBTOGRAY, &CMyIPTool2Doc::OnUpdateOpencvRgbtogray)
	ON_UPDATE_COMMAND_UI(ID_RGBTOGRAY, &CMyIPTool2Doc::OnUpdateRgbtogray)
	ON_COMMAND(ID_CONTOUR_TRACING, &CMyIPTool2Doc::OnContourTracing)
	ON_COMMAND(ID_CONTOURTRACING_EUCLIDEAN, &CMyIPTool2Doc::OnContourtracingEuclidean)
	ON_COMMAND(ID_CONTOURTRACING_SINGLEOBJECT, &CMyIPTool2Doc::OnContourtracingSingleobject)
	ON_COMMAND(ID_GEOMETRICTRANSFORMATIONS_TRANSLATION, &CMyIPTool2Doc::OnGeometrictransformationsTranslation)
	ON_COMMAND(ID_USERFEATURE_PERIODICTRANSLATION, &CMyIPTool2Doc::OnUserfeaturePeriodictranslation)
END_MESSAGE_MAP()


// CMyIPTool2Doc 생성/소멸

CMyIPTool2Doc::CMyIPTool2Doc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	m_Title = _T("");
	srand((unsigned)(time(NULL)));		//srand 인수를 시간으로 준다.
}

CMyIPTool2Doc::~CMyIPTool2Doc()
{
}

BOOL CMyIPTool2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	//AfxPrintInfo(_T("CMyIPTool2Doc::OnNewDocument()"));  190313 YWCho 너무 많이 출력된당~ 주석~

	if (theApp.m_sTitle.GetLength() > 0) {
		//m_Title = theApp.m_sTitle;
		SetTitle(theApp.m_sTitle);
		theApp.m_sTitle = _T("");
	}

	if (theApp.m_pMat) {
		if (!IpMatToImage(*(theApp.m_pMat), m_MyIPImage)) {
			theApp.m_pMat = (Mat*)NULL;
			return false;
		}
		else
			theApp.m_pMat = (Mat*)NULL;
	}

	else if (theApp.m_pImage) {
		m_MyIPImage = *(theApp.m_pImage);
		theApp.m_pImage = (MyIPImage *)NULL;
	}

	return true;
}




// CMyIPTool2Doc serialization

void CMyIPTool2Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CMyIPTool2Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CMyIPTool2Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CMyIPTool2Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMyIPTool2Doc 진단

#ifdef _DEBUG
void CMyIPTool2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMyIPTool2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMyIPTool2Doc 명령


BOOL CMyIPTool2Doc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	AfxPrintInfo(_T("CMyIPTool2Doc::OnOpenDocument"));
	AfxPrintInfo(lpszPathName);
	
	//CImage::Load로 읽어오기
// 	if (!m_MyIPImage.Load(lpszPathName))	//load는 8비트1ch, 8비트3ch영상은 잘 읽는데 그 외를 잘 못읽음..
// 		return false;						//16비트 흑백 이미지를 컬러로 읽곤함 

	//Mat로 읽어오기
	string filename = CT2A(lpszPathName);
	m_Mat = imread(filename, IMREAD_UNCHANGED);
	if (!IpMatToImage(m_Mat, m_MyIPImage))
		return false;
	/*
	IMREAD_UNCHANGED
	IMREAD_GRAYSCALE
	IMREAD_COLOR
	IMREAD_ANYDEPTH
	IMREAD_ANYCOLOR
	IMREAD_LOAD_GDAL
	IMREAD_REDUCED_GRAYSCALE_2
	IMREAD_REDUCED_COLOR_2
	IMREAD_REDUCED_GRAYSCALE_4
	IMREAD_REDUCED_COLOR_4
	IMREAD_REDUCED_GRAYSCALE_8
	IMREAD_REDUCED_COLOR_8
	IMREAD_IGNORE_ORIENTATION
		*/



	//CImage로 읽어오기
// 	m_MyIPImage.Destroy();
// 	m_MyIPImage.Load(lpszPathName);
// 
// 	m_MyIPImage.SetDepth(m_Mat.depth());
// 	m_MyIPImage.SetChannels(m_Mat.channels());
	

	return TRUE;

}


BOOL CMyIPTool2Doc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (!IpImageToMat(m_MyIPImage, m_Mat)) return false;
	string filename = CT2A(lpszPathName);
	return imwrite(filename, m_Mat);

	//return CDocument::OnSaveDocument(lpszPathName);
}



void CMyIPTool2Doc::OnInverse()
{
	AfxPrintInfo(_T("CMyIPTool2Doc::OnInverse"));
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	/*-------------IP_Inverse(img)-----------------*/
#ifdef USE_IPIMAGE
	MyIPImage imgcopy(m_MyIPImage);
//	imgcopy = m_MyIPImage;
//	imgcopy.Copy(m_MyIPImage);
	if (IP_Inverse(imgcopy)) {
		AfxNewImage(_T("Inverse"), imgcopy);
		AfxPrintInfo(_T("[COMPLETE] Inverse"));
	}
	else
		AfxPrintInfo(_T("[Fail] Inverse"));

#else
	MyIPImage img;
	if (IpMatToImage(m_Mat, img)) {
		if (IP_Inverse(img)) {
			AfxNewImage(_T("Inverse"), img);
			AfxPrintInfo(_T("[COMPLETE] Inverse"));
		}
		else
			AfxPrintInfo(_T("[Fail] Inverse"));
	}
	else
		AfxPrintInfo(_T("[Fail] IpMatToImage"));
#endif 

	/*-------------IP_Inverse(mat)-----------------*/
// 	Mat mat_copy;
// 	m_Mat.copyTo(mat_copy);
// 	if (IP_Inverse(mat_copy)) {
// 		AfxNewImage(mat_copy);
// 	}

	/*--------------과제 1------------------*/
	/*IP_Inverse(Mat) 프로그래밍 하고 mat로 읽어 변환하고 imshow로 출력하기*/
// 	if (IP_Inverse(m_Mat)) {
// 		imshow("IP_Inverse(Mat)", m_Mat);
// 	}

	/*--------------과제 2------------------*/  //안됨 
	/*IP_Inverse(MyIPImage)사용해서 img.draw로 출력하기*/

// 	MyIPImage img;
// 	img.CreateImage(m_MyIPImage.GetHeight(), m_MyIPImage.GetWidth(), m_MyIPImage.GetDepth(), m_MyIPImage.GetChannel());
// 	if (IP_Inverse(img)) {
// 		AfxNewImage(img);
// 	}
}

void CMyIPTool2Doc::OnBrightnessContrast()
{
	AfxPrintInfo(_T("CMyIPTool2Doc::OnBrightnessContrast()"));
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CBrightnessContrastDlg dlg;
	dlg.m_pDoc = this;
	POSITION pos = GetFirstViewPosition();
	dlg.m_pView = (CMyIPTool2View*)GetNextView(pos);


	/*-----------IP_BrightnessContrast(MyIPImage)-------------------------*/
#ifdef USE_IPIMAGE
	MyIPImage imgcopy;
	imgcopy = m_MyIPImage;
	if (imgcopy.GetDepth() == CV_16U || imgcopy.GetDepth() == CV_16S)	//16비트 적용
		dlg.bIs16Bit = true;
	if (dlg.DoModal() == IDOK) {
		if (IP_BrightnessContrast(imgcopy, dlg.m_nOffset, dlg.m_fGain)) {
			AfxNewImage(_T("Brightness & Contrast"), imgcopy);
		}
	}
	dlg.m_pView->Invalidate(false);

#else
	MyIPImage img;
	if (IpMatToImage(m_Mat, img)) {
		if (dlg.DoModal() == IDOK) {
			if (IP_BrightnessContrast(img, dlg.m_nOffset, dlg.m_fGain)) {
				AfxNewImage(_T("Brightness & Contrast"), img);
			}
		}
		dlg.m_pView->Invalidate(false);
	}
#endif
	/*-----------IP_BrightnessContrast(Mat)-------------------------*/
// 	Mat mat_copy;
// 	m_Mat.copyTo(mat_copy);
// 	if (dlg.DoModal() == IDOK) {
// 		if (IP_BrightnessContrast(mat_copy,dlg.m_nOffset,dlg.m_fGain)) {
// 			AfxNewImage(mat_copy);
// 			AfxPrintInfo(_T("[COMPLETE] BrightnessContrast"));
// 		}
// 		else
// 			AfxPrintInfo(_T("[FAIL] BrightnessContrast"));
// 	}
// 
// 	dlg.m_pView->Invalidate(false);
}

void CMyIPTool2Doc::OnGamma()
{
	AfxPrintInfo(_T("CMyIPTool2Doc::OnGamma()"));
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	/*----------------다이얼로그 클래스 변수선언 및 맴버변수 정의------------------*/
	CGammaDlg dlg;											//Gamma 다이얼로그 사용을 위한 헤더파일 추가 必

	dlg.m_pDoc = this;										//다이얼로그의 m_pDoc포인터 변수에 Doc.cpp의 주소를 전달
	POSITION pos = GetFirstViewPosition();					//도큐먼트 클래스에서 뷰 클래스의 인스턴스 포인터를 얻으려면 
	dlg.m_pView = (CMyIPTool2View*)GetNextView(pos);		//GetFirstViewPosition() 함수와 GetNextView() 함수를 조합하여 사용해야 한다. 
															//이렇게 복잡해지는 이유는 도큐먼트 하나에 여러개의 뷰가 연결될 수 있기 때문이다.


	/*-----------IP_GammaCorrection(MyIPImage)------------------------------------*/
#ifdef USE_IPIMAGE
	MyIPImage imgcopy;
	imgcopy = m_MyIPImage;
	if (imgcopy.GetDepth() == CV_16U || imgcopy.GetDepth() == CV_16S)	//16비트 적용
		dlg.bIs16Bit = true;
	if (dlg.DoModal() == IDOK) {						//만약 다이얼로그에서 확인을 누르면
		if (IP_GammaCorrection(imgcopy, dlg.m_fGamma)) {	//만약 감마보정이 다 됬으면 
			AfxNewImage(_T("Gamma Correction"), imgcopy);	//AfxNewImage함수에 img를 전달한다.
			AfxPrintInfo(_T("[COMPLETE] Gamma Control"));
		}
		else
			AfxPrintInfo(_T("[FAIL] Gamma Control"));
	}
	dlg.m_pView->Invalidate(false);	//이걸 넣어주면 취소 해도 그대로 유지 
#else
	MyIPImage img;
	if (IpMatToImage(m_Mat, img)) {							//Mat을 MyIPImage로 변환에 성공하면
		if (dlg.DoModal() == IDOK) {						//만약 다이얼로그에서 확인을 누르면
			if (IP_GammaCorrection(img, dlg.m_fGamma)) {	//만약 감마보정이 다 됬으면 
				AfxNewImage(_T("Gamma Correction"), img);	//AfxNewImage함수에 img를 전달한다.
				AfxPrintInfo(_T("[COMPLETE] Gamma Control"));
			}
			else
				AfxPrintInfo(_T("[FAIL] Gamma Control"));
		}
		dlg.m_pView->Invalidate(false);	//이걸 넣어주면 취소 해도 그대로 유지 
	}
	else
		AfxPrintInfo(_T("[FAIL] IpMatToImage"));
#endif

	/*-----------IP_GammaCorrection(Mat)------------------------------------*/
// 	Mat mat_copy;
// 	m_Mat.copyTo(mat_copy);
// 	if (dlg.DoModal() == IDOK) {
// 		if (IP_GammaCorrection(mat_copy, dlg.m_fGamma)) {
// 			AfxNewImage(mat_copy);
// 		}
// 	}
// 	dlg.m_pView->Invalidate(false);	//이걸 넣어주면 취소 해도 그대로 유지 

} 

void CMyIPTool2Doc::OnViewHistogram()
{
	AfxPrintInfo(_T("CMyIPTool2Doc::OnViewHistogram()"));
	// TODO: 여기에 명령 처리기 코드를 추가합니다.



	/*-----------SetImage(MyIPImage) / IP_Histogram(MyIPImage) -------------------*/
#ifdef USE_IPIMAGE
	MyIPImage img;
	img = m_MyIPImage;
	//CHistogramDlg dlg;					//domodal
	//dlg.SetImage(img); 
	//dlg.DoModal();
	m_pHistogramDlg = new CHistogramDlg();	//showwindow 
	m_pHistogramDlg->SetImage(img);
	m_pHistogramDlg->Create(IDD_HISTOGRAM, NULL);
	m_pHistogramDlg->ShowWindow(SW_SHOW);
	//m_pHistogramDlg->MoveWindow(CRect(200, 200, 400, 400));
	AfxPrintInfo(_T("[COMPLETE] Show Histogram"));
#else
	MyIPImage img;
	if (IpMatToImage(m_Mat, img)) {
		dlg.SetImage(img);
		dlg.DoModal();
		AfxPrintInfo(_T("[COMPLETE] Show Histogram"));
	}
	else
		AfxPrintInfo(_T("[FAIL] IpMatToImage"));
#endif
/*-----------SetImage(Mat) / IP_Histogram(Mat) -------------------*/
// 	dlg.SetImage(m_Mat);
// 	dlg.DoModal();
}

void CMyIPTool2Doc::OnHistoStretching()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnHistoStretching()"));

	/*----------- OnHistoStretching(MyIPImage) -------------------*/
#ifdef USE_IPIMAGE
	MyIPImage imgcopy;
	imgcopy = m_MyIPImage;
	if (IP_HistogramStretching(imgcopy)) {
		AfxNewImage(_T("히스토그램 스트레칭"), imgcopy);
		AfxPrintInfo(_T("[COMPLETE] Histogram Stretching"));
	}
	else
		AfxPrintInfo(_T("[FAIL] Histogram Stretching"));
#else
	MyIPImage img;
	if (IpMatToImage(m_Mat, img)) {
		if (IP_HistogramStretching(img)) {
			AfxNewImage(img);
			AfxPrintInfo(_T("[COMPLETE] Histogram Stretching"));
		}
		else
			AfxPrintInfo(_T("[FAIL] Histogram Stretching"));
	}
	else
		AfxPrintInfo(_T("[FAIL] IpMatToImage"));
#endif
}

void CMyIPTool2Doc::OnHistoEqualization()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnHistoEqualization()"));

	/*----------- OnHistoStretching(MyIPImage) -------------------*/
#ifdef USE_IPIMAGE
	MyIPImage imgcopy;
	imgcopy = m_MyIPImage;
	if (IP_HistogramEqualization(imgcopy)) {
		AfxNewImage(_T("히스토그램 균등화"),imgcopy);
		AfxPrintInfo(_T("[COMPLETE] Histogram Equalization"));
	}
	else
		AfxPrintInfo(_T("[FAIL] Histogram Equalization"));
#else
	MyIPImage img;
	if (IpMatToImage(m_Mat, img)) {
		if (IP_HistogramEqualization(img)) {
			AfxNewImage(img);
			AfxPrintInfo(_T("[COMPLETE] Histogram Equalization"));
		}
		else
			AfxPrintInfo(_T("[FAIL] Histogram Equalization"));
	}
	else
		AfxPrintInfo(_T("[FAIL] IpMatToImage"));
#endif
}

/*--------------------MyIPImage Color Conversion--------------------*/
void CMyIPTool2Doc::OnRgbtogray()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnRgbtogray()"));

	MyIPImage imggray;
	
#ifdef USE_IPIMAGE
	MyIPImage imgcopy;
	imgcopy= m_MyIPImage;
	if (IpRGBToGray(imgcopy, imggray)) {
		AfxNewImage(_T("RGB To Gray"), imggray);
		AfxPrintInfo(_T("[COMPLETE] RGB To GRAY"));
	}
	else
		AfxPrintInfo(_T("[FAIL] RGB To GRAY"));
#else
	MyIPImage img;
	if (IpMatToImage(m_Mat, img)) {
		if (IpRGBToGray(img, imggray)) {
			AfxNewImage(_T("RGB To Gray"), imggray);
			AfxPrintInfo(_T("[COMPLETE] RGB To GRAY"));
		}
		else
			AfxPrintInfo(_T("[FAIL] RGB To GRAY"));
	}
	else
		AfxPrintInfo(_T("[FAIL] IpMatToImage"));
#endif
}

void CMyIPTool2Doc::OnUpdateRgbtogray(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_MyIPImage.GetChannels() == 3);
}

void CMyIPTool2Doc::OnColorSplitRGB()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnColorSplitRGB()"));
#ifdef USE_IPIMAGE
	CString title;
	MyIPImage imgcopy, imgR, imgG, imgB;
	imgcopy = m_MyIPImage;
	if (IpColorSplitRGB(imgcopy, imgR, imgG, imgB)) {
		title = _T("R ") + GetTitle();
		AfxNewImage(title, imgR);
		title = _T("G ") + GetTitle();
		AfxNewImage(title, imgG);
		title = _T("B ") + GetTitle();
		AfxNewImage(title, imgB);
		AfxPrintInfo(_T("[COMPLETE] Color Split RGB"));
	}
	else
		AfxPrintInfo(_T("[FAIL] Color Split RGB"));
#else
	MyIPImage img, imgR, imgG, imgB;
	CString title;

	if (IpMatToImage(m_Mat, img)) {
		if (IpColorSplitRGB(img, imgR, imgG, imgB)) {
			title = _T("R ") + GetTitle();
			AfxNewImage(title, imgR);
			title = _T("G ") + GetTitle();
			AfxNewImage(title, imgG);
			title = _T("B ") + GetTitle();
			AfxNewImage(title, imgB);
			AfxPrintInfo(_T("[COMPLETE] Color Split RGB"));
		}
		else
			AfxPrintInfo(_T("[FAIL] Color Split RGB"));
	}
	else
		AfxPrintInfo(_T("[FAIL] IpMatToImage"));
#endif
}

void CMyIPTool2Doc::OnColorSplitHSI()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnColorSplitHSI()"));

#ifdef USE_IPIMAGE
	CString title;
	MyIPImage imgcopy, imgH, imgS, imgI;
	imgcopy = m_MyIPImage;

	if (IpColorSplitHSI(imgcopy, imgH, imgS, imgI)) {
		title = _T("H ") + GetTitle();
		AfxNewImage(title, imgH);
		title = _T("S ") + GetTitle();
		AfxNewImage(title, imgS);
		title = _T("I ") + GetTitle();
		AfxNewImage(title, imgI);
		AfxPrintInfo(_T("[COMPLETE] Color Split HSI"));
	}
	else
		AfxPrintInfo(_T("[FAIL] Color Split HSI"));
#else
	MyIPImage img, imgH, imgS, imgI;
	CString title;

	if (IpMatToImage(m_Mat, img)) {
		if (IpColorSplitHSI(img, imgH, imgS, imgI)) {
			title = _T("H ") + GetTitle();
			AfxNewImage(title, imgH);
			title = _T("S ") + GetTitle();
			AfxNewImage(title, imgS);
			title = _T("I ") + GetTitle();
			AfxNewImage(title, imgI);
			AfxPrintInfo(_T("[COMPLETE] Color Split HSI"));
		}
		else
			AfxPrintInfo(_T("[FAIL] Color Split HSI"));
	}
	else
		AfxPrintInfo(_T("[FAIL] IpMatToImage"));
#endif
}

void CMyIPTool2Doc::OnColorSplitYuv()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnColorSplitYuv()"));
#ifdef USE_IPIMAGE
	CString title;
	MyIPImage imgcopy, imgY, imgU, imgV;
	imgcopy = m_MyIPImage;

	if (IpColorSplitYUV(imgcopy, imgY, imgU, imgV)) {
		title = _T("Y ") + GetTitle();
		AfxNewImage(title, imgY);
		title = _T("U ") + GetTitle();
		AfxNewImage(title, imgU);
		title = _T("V ") + GetTitle();
		AfxNewImage(title, imgV);
		AfxPrintInfo(_T("[COMPLETE] Color Split YUV"));
	}
	else
		AfxPrintInfo(_T("[FAIL] Color Split YUV"));
#else
	CString title;
	MyIPImage img, imgY, imgU, imgV;

	if (IpMatToImage(m_Mat, img)) {
		if (IpColorSplitYUV(img, imgY, imgU, imgV)) {
			title = _T("Y ") + GetTitle();
			AfxNewImage(title, imgY);
			title = _T("U ") + GetTitle();
			AfxNewImage(title, imgU);
			title = _T("V ") + GetTitle();
			AfxNewImage(title, imgV);
			AfxPrintInfo(_T("[COMPLETE] Color Split YUV"));
		}
		else
			AfxPrintInfo(_T("[FAIL] Color Split YUV"));
	}
	else
		AfxPrintInfo(_T("[FAIL] IpMatToImage"));
#endif
}

void CMyIPTool2Doc::OnColorCombineRgb()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnColorCombineRgb()"));
	CColorCombineDlg dlg;
	dlg.m_strColorSpace = _T("RGB 색상 평면 합치기");
	if (dlg.DoModal() == IDOK)
	{
		CMyIPTool2Doc* pDoc1 = (CMyIPTool2Doc*)dlg.m_pDoc1;
		CMyIPTool2Doc* pDoc2 = (CMyIPTool2Doc*)dlg.m_pDoc2;
		CMyIPTool2Doc* pDoc3 = (CMyIPTool2Doc*)dlg.m_pDoc3;

#ifdef USE_IPIMAGE
		MyIPImage imgR, imgG, imgB, imgColor;
		imgR = pDoc1->m_MyIPImage;
		imgG = pDoc2->m_MyIPImage;
		imgB = pDoc3->m_MyIPImage;

		if (!IpColorConbineRGB(imgR, imgG, imgB, imgColor)) {
			AfxMessageBox(_T("영상의 Size, Channel, Depth 오류"));
			return;
		}

		AfxPrintInfo(_T("[색상 평면 합치기/RGB] 입력영상: R: %s, G: %s, B: %s"),
			pDoc1->GetTitle(), pDoc2->GetTitle(), pDoc3->GetTitle());
		AfxNewImage(_T("RGB Image"), imgColor);
#else
		MyIPImage imgR, imgG, imgB, imgColor;
		if (!IpMatToImage(pDoc1->m_Mat, imgR))
			return;
		if (!IpMatToImage(pDoc2->m_Mat, imgG))
			return;
		if (!IpMatToImage(pDoc3->m_Mat, imgB))
			return;

		if (!IpColorConbineRGB(imgR, imgG, imgB, imgColor)) {
			AfxMessageBox(_T("영상의 Size, Channel, Depth 오류"));
			return;
		}

		AfxPrintInfo(_T("[색상 평면 합치기/RGB] 입력영상: R: %s, G: %s, B: %s"),
			pDoc1->GetTitle(), pDoc2->GetTitle(), pDoc3->GetTitle());
		AfxNewImage(_T("RGB Image"), imgColor);
#endif
	}
}

void CMyIPTool2Doc::OnColorCombineHsi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnColorCombineHsi()"));
	CColorCombineDlg dlg;
	dlg.m_strColorSpace = _T("HSI 색상 평면 합치기");
	if (dlg.DoModal() == IDOK)
	{
		CMyIPTool2Doc* pDoc1 = (CMyIPTool2Doc*)dlg.m_pDoc1;
		CMyIPTool2Doc* pDoc2 = (CMyIPTool2Doc*)dlg.m_pDoc2;
		CMyIPTool2Doc* pDoc3 = (CMyIPTool2Doc*)dlg.m_pDoc3;

#ifdef USE_IPIMAGE
		MyIPImage imgH, imgS, imgI, imgColor;
		imgH = pDoc1->m_MyIPImage;
		imgS = pDoc2->m_MyIPImage;
		imgI = pDoc3->m_MyIPImage;

		if (!IpColorConbineHSI(imgH, imgS, imgI, imgColor)) {
			AfxMessageBox(_T("영상의 Size, Channel, Depth 오류"));
			return;
		}

		AfxPrintInfo(_T("[색상 평면 합치기/HSI] 입력영상: H: %s, S: %s, I: %s"),
			pDoc1->GetTitle(), pDoc2->GetTitle(), pDoc3->GetTitle());
		AfxNewImage(_T("HSI Image"), imgColor);
#else
		MyIPImage imgH, imgS, imgI, imgColor;
		if (!IpMatToImage(pDoc1->m_Mat, imgH))
			return;
		if (!IpMatToImage(pDoc2->m_Mat, imgS))
			return;
		if (!IpMatToImage(pDoc3->m_Mat, imgI))
			return;

		if (!IpColorConbineHSI(imgH, imgS, imgI, imgColor)) {
			AfxMessageBox(_T("영상의 Size, Channel, Depth 오류"));
			return;
		}

		AfxPrintInfo(_T("[색상 평면 합치기/HSI] 입력영상: H: %s, S: %s, I: %s"),
			pDoc1->GetTitle(), pDoc2->GetTitle(), pDoc3->GetTitle());
		AfxNewImage(_T("HSI Image"),imgColor);
#endif
	}
}

void CMyIPTool2Doc::OnColorCombineYuv()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnColorCombineYuv()"));
	CColorCombineDlg dlg;
	dlg.m_strColorSpace = _T("YUV 색상 평면 합치기");
	if (dlg.DoModal() == IDOK)
	{
		CMyIPTool2Doc* pDoc1 = (CMyIPTool2Doc*)dlg.m_pDoc1;
		CMyIPTool2Doc* pDoc2 = (CMyIPTool2Doc*)dlg.m_pDoc2;
		CMyIPTool2Doc* pDoc3 = (CMyIPTool2Doc*)dlg.m_pDoc3;

#ifdef USE_IPIMAGE
		MyIPImage imgY, imgU, imgV, imgColor;
		imgY = pDoc1->m_MyIPImage;
		imgU = pDoc2->m_MyIPImage;
		imgV = pDoc3->m_MyIPImage;

		if (!IpColorConbineYUV(imgY, imgU, imgV, imgColor)) {
			AfxMessageBox(_T("영상의 Size, Channel, Depth 오류"));
			return;
		}

		AfxPrintInfo(_T("[색상 평면 합치기/YUV] 입력영상: Y: %s, U: %s, V: %s"),
			pDoc1->GetTitle(), pDoc2->GetTitle(), pDoc3->GetTitle());
		AfxNewImage(_T("YUV Image"), imgColor);
#else
		MyIPImage imgY, imgU, imgV, imgColor;
		if (!IpMatToImage(pDoc1->m_Mat, imgY))
			return;
		if (!IpMatToImage(pDoc2->m_Mat, imgU))
			return;
		if (!IpMatToImage(pDoc3->m_Mat, imgV))
			return;

		if (!IpColorConbineYUV(imgY, imgU, imgV, imgColor)) {
			AfxMessageBox(_T("영상의 Size, Channel, Depth 오류"));
			return;
		}

		AfxPrintInfo(_T("[색상 평면 합치기/YUV] 입력영상: Y: %s, U: %s, V: %s"),
			pDoc1->GetTitle(), pDoc2->GetTitle(), pDoc3->GetTitle());
		AfxNewImage(_T("YUV Image"), imgColor);
#endif
	}
}



/*--------------------Open CV Color Conversion-----------------------*/
void CMyIPTool2Doc::OnOpencvRgbtogray()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnOpencvRgbtogray()"));
	Mat out, matimg;
#ifdef USE_IPIMAGE
	if (!IpImageToMat(m_MyIPImage, matimg)) {
		AfxPrintInfo(_T("[FAIL] IpImageToMat"));
	}
	if (IpRGBToGray(matimg,out)) {
#else
	if (IpRGBToGray(m_Mat, out)) {
#endif
		AfxNewImage(_T("OnOpencvRgbtogray"), out);
		AfxPrintInfo(_T("[COMPLETE] (CV) RGB to GRAY Convert"));
	}
	else
		AfxPrintInfo(_T("[FAIL] (CV) RGB to GRAY Convert"));
}

void CMyIPTool2Doc::OnUpdateOpencvRgbtogray(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
}


void CMyIPTool2Doc::OnOpencvColorSplitRgb()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnOpencvColorSplitRgb()"));
	Mat out, rgb[3], matimg;
#ifdef USE_IPIMAGE
	if (!IpImageToMat(m_MyIPImage, matimg)) {
		AfxPrintInfo(_T("[FAIL] IpImageToMat"));
	}
	if (IpColorSplitRGB(matimg, out)) {
#else
	if (IpColorSplitRGB(m_Mat, out)) {
#endif
		split(out, rgb);
		AfxNewImage(_T("Rmat"), rgb[0]);
		AfxNewImage(_T("Gmat"), rgb[1]);
		AfxNewImage(_T("Bmat"), rgb[2]);
		AfxNewImage(_T("RGB mat"), out);
		AfxPrintInfo(_T("[COMPLETE] (CV) RGB TO COLOR SPLIT"));
	}
	else
		AfxPrintInfo(_T("[FAIL] (CV) RGB TO COLOR SPLIT"));
}

void CMyIPTool2Doc::OnOpencvColorSplitHsi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnOpencvColorSplitHsi()"));
	Mat out,hsi[3], matimg;
#ifdef USE_IPIMAGE
	if (!IpImageToMat(m_MyIPImage, matimg)) {
		AfxPrintInfo(_T("[FAIL] IpImageToMat"));
	}
	if (IpColorSplitHSI(matimg, out)) {
#else
	if (IpColorSplitHSI(m_Mat, out)) {
#endif
		split(out, hsi);
		AfxNewImage(_T("Hmat"), hsi[0]);
		AfxNewImage(_T("Smat"), hsi[1]);
		AfxNewImage(_T("Imat"), hsi[2]);
		AfxNewImage(_T("HSI mat"), out);
		AfxPrintInfo(_T("[COMPLETE] (CV) RGB TO HSI SPLIT"));
	}
	else
		AfxPrintInfo(_T("[FAIL] (CV) RGB TO HSI SPLIT"));
}

void CMyIPTool2Doc::OnOpencvColorSplitHsv()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnOpencvColorSplitHsv()"));
	Mat out, hsv[3], matimg;
#ifdef USE_IPIMAGE
	if (!IpImageToMat(m_MyIPImage, matimg)) {
		AfxPrintInfo(_T("[FAIL] IpImageToMat"));
	}
	if (IpColorSplitHSV(matimg, out)) {
#else
	if (IpColorSplitHSV(m_Mat, out)) {
#endif
		split(out, hsv);
		AfxNewImage(_T("Hmat"), hsv[0]);
		AfxNewImage(_T("Smat"), hsv[1]);
		AfxNewImage(_T("Vmat"), hsv[2]);
		AfxNewImage(_T("HSV mat"), out);
		AfxPrintInfo(_T("[COMPLETE] (CV) RGB TO HSV SPLIT"));
	}
	else
		AfxPrintInfo(_T("[FAIL] (CV) RGB TO HSV SPLIT"));
}

void CMyIPTool2Doc::OnOpencvColorSplitYuv()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnOpencvColorSplitYuv()"));
	Mat out, yuv[3], matimg;
#ifdef USE_IPIMAGE
	if (!IpImageToMat(m_MyIPImage, matimg)) {
		AfxPrintInfo(_T("[FAIL] IpImageToMat"));
	}
	if (IpColorSplitYUV(matimg, out)) {
#else
	if (IpColorSplitYUV(m_Mat, out)) {
#endif
		split(out, yuv);
		AfxNewImage(_T("Ymat"), yuv[0]);
		AfxNewImage(_T("Umat"), yuv[1]);
		AfxNewImage(_T("Vmat"), yuv[2]);
		AfxNewImage(_T("YUV mat"), out);
		AfxPrintInfo(_T("[COMPLETE] (CV) RGB TO YUV SPLIT"));
	}
	else
		AfxPrintInfo(_T("[FAIL] (CV) RGB TO YUV SPLIT"));
}

void CMyIPTool2Doc::OnOpencvColorSplitYcrcb()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnOpencvColorSplitYcrcb()"));
	Mat out, ycrcb[3], matimg;
#ifdef USE_IPIMAGE
	if (!IpImageToMat(m_MyIPImage, matimg)) {
		AfxPrintInfo(_T("[FAIL] IpImageToMat"));
	}
	if (IpColorSplitYCrCB(matimg, out)) {
#else
	if (IpColorSplitYCrCB(m_Mat, out)) {
#endif
		split(out, ycrcb);
		AfxNewImage(_T(" Ymat"), ycrcb[0]);
		AfxNewImage(_T("CRmat"), ycrcb[1]);
		AfxNewImage(_T("CBmat"), ycrcb[2]);
		AfxNewImage(_T("YCrCb mat"), out);
		AfxPrintInfo(_T("[COMPLETE] (CV) RGB TO YCRCB SPLIT"));
	}
	else
		AfxPrintInfo(_T("[FAIL] (CV) RGB TO YCRCB SPLIT"));
}

void CMyIPTool2Doc::OnOpencvColorSplitLab()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnOpencvColorSplitLab()"));
	Mat out, lab[3], matimg;
#ifdef USE_IPIMAGE
	if (!IpImageToMat(m_MyIPImage, matimg)) {
		AfxPrintInfo(_T("[FAIL] IpImageToMat"));
	}
	if (IpColorSplitLAB(matimg, out)) {
#else
	if (IpColorSplitLAB(m_Mat, out)) {
#endif
		split(out, lab);
		AfxNewImage(_T("Lmat"), lab[0]);
		AfxNewImage(_T("Amat"), lab[1]);
		AfxNewImage(_T("Bmat"), lab[2]);
		AfxNewImage(_T("LAB mat"), out);
		AfxPrintInfo(_T("[COMPLETE] (CV) RGB TO LAB SPLIT"));
	}
	else
		AfxPrintInfo(_T("[FAIL] (CV) RGB TO LAB SPLIT"));
}

void CMyIPTool2Doc::OnOpencvColorSplitCmy()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnOpencvColorSplitCmy()"));
	Mat out, cmy[3], matimg;
#ifdef USE_IPIMAGE
	if (!IpImageToMat(m_MyIPImage, matimg)) {
		AfxPrintInfo(_T("[FAIL] IpImageToMat"));
	}
	if (IpColorSplitCMY(matimg, out)) {
#else
	if (IpColorSplitCMY(m_Mat, out)) {
#endif
		split(out, cmy);
		AfxNewImage(_T("Cmat"), cmy[0]);
		AfxNewImage(_T("Mmat"), cmy[1]);
		AfxNewImage(_T("Ymat"), cmy[2]);
		AfxNewImage(_T("CMY mat"), out);
		AfxPrintInfo(_T("[COMPLETE] (CV) RGB TO CMY SPLIT"));
	}
	else
		AfxPrintInfo(_T("[FAIL] (CV) RGB TO CMY SPLIT"));
}

void CMyIPTool2Doc::OnOpencvColorSplitCmyk()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnOpencvColorSplitCmyk()"));
	Mat out, cmyk[4], matimg;
#ifdef USE_IPIMAGE
	if (!IpImageToMat(m_MyIPImage, matimg)) {
		AfxPrintInfo(_T("[FAIL] IpImageToMat"));
	}
	if (IpColorSplitCMYK(matimg, out)) 
	{
#else
	if (IpColorSplitCMYK(m_Mat, out)) {
#endif
		split(out, cmyk);
		AfxNewImage(_T("Cmat"), cmyk[0]);
		AfxNewImage(_T("Mmat"), cmyk[1]);
		AfxNewImage(_T("Ymat"), cmyk[2]);
		AfxNewImage(_T("Kmat"), cmyk[3]);
		AfxNewImage(_T("CMYK mat"), out);
		AfxPrintInfo(_T("[COMPLETE] (CV) RGB TO CMYK SPLIT"));
	}
	else
		AfxPrintInfo(_T("[FAIL] (CV) RGB TO CMYK SPLIT"));
}

void CMyIPTool2Doc::OnOpencvHistogram()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnOpencvHistogram()"));

	Mat img, hist;
	if (IpImageToMat(m_MyIPImage, img)) {
		TIMECHECK_START
		if (IP_Histogram(img, hist)) {
			TIMECHECK_END
			AfxNewImage(hist);
		}
	}
}




/*-------------------- Arithmetic -----------------------*/
void CMyIPTool2Doc::OnArithmeticlogical()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CArithmeticLogicalDlg dlg;
	if (dlg.DoModal() == IDOK) {

		//>> 윈도우 이름 세팅
		TCHAR* op[] = { _T("덧셈"),_T("뺄셈"),_T("평균"),_T("차이"),_T("논리합"),_T("논리곱"),_T("최소값"),_T("최대값"), _T("베타적 논리합") };
		AfxPrintInfo(_T("산술 및 논리 연산: %s      입력영상#1: %s      입력영상#2: %s"),
			op[dlg.m_nFunction], dlg.m_pDoc1->GetTitle(), dlg.m_pDoc2->GetTitle());
		//<<

		MyIPImage oimg;
		if (dlg.m_bUseConstant == false) {
			if (!IP_ARLogic(dlg.m_pDoc1->m_MyIPImage, dlg.m_pDoc2->m_MyIPImage, oimg, dlg.m_nFunction))	return;
		}
		else if(dlg.m_bUseConstant == true){
			if (!IP_ARLogic(dlg.m_pDoc1->m_MyIPImage, dlg.m_pDoc2->m_MyIPImage, oimg, dlg.m_nFunction, TRUE, dlg.m_editConstant))	return;
		}
		CString strTmp;
		strTmp.Format(_T("%s(1)%s_(2)%s)"), op[dlg.m_nFunction], dlg.m_pDoc1->GetTitle(), dlg.m_pDoc2->GetTitle());
		AfxNewImage(strTmp, oimg);
	}
}

void CMyIPTool2Doc::OnCvArithmeticlogical()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CArithmeticLogicalDlg dlg;
	if (dlg.DoModal() == IDOK) {

		//>> 윈도우 이름 세팅
		TCHAR* op[] = { _T("덧셈"),_T("뺄셈"),_T("평균"),_T("차이"),_T("논리합"),_T("논리곱"),_T("최소값"),_T("최대값"), _T("베타적 논리합") };
		AfxPrintInfo(_T("산술 및 논리 연산: %s      입력영상#1: %s      입력영상#2: %s"),
			op[dlg.m_nFunction], dlg.m_pDoc1->GetTitle(), dlg.m_pDoc2->GetTitle());
		//<<

		Mat img1, img2, oimg; 

		if(IpImageToMat(dlg.m_pDoc1->m_MyIPImage,img1) && IpImageToMat(dlg.m_pDoc2->m_MyIPImage,img2)){

			if (dlg.m_bUseConstant == false) {
				if (!IP_ARLogic(img1, img2, oimg, dlg.m_nFunction)) return;
			}
			else {
				if (!IP_ARLogic(img1, img2, oimg, dlg.m_nFunction, TRUE, dlg.m_editConstant)) return;
			}
		}
		CString title = _T("CV-") + (CString)(_T("%s"), op[dlg.m_nFunction]);
		AfxNewImage(title, oimg);
	}
}


/*-------------------- Thresholding -----------------------*/
void CMyIPTool2Doc::OnThreshold()
{
	AfxPrintInfo(_T("CMyIPTool2Doc::OnThreshold()"));
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	/*----------------다이얼로그 클래스 변수선언 및 맴버변수 정의------------------*/
	CThresholdDlg dlg;											//Gamma 다이얼로그 사용을 위한 헤더파일 추가 必

	dlg.m_pDoc = this;										//다이얼로그의 m_pDoc포인터 변수에 Doc.cpp의 주소를 전달
	POSITION pos = GetFirstViewPosition();					//도큐먼트 클래스에서 뷰 클래스의 인스턴스 포인터를 얻으려면 
	dlg.m_pView = (CMyIPTool2View*)GetNextView(pos);		//GetFirstViewPosition() 함수와 GetNextView() 함수를 조합하여 사용해야 한다. 
															//이렇게 복잡해지는 이유는 도큐먼트 하나에 여러개의 뷰가 연결될 수 있기 때문이다.

	MyIPImage imgcopy;
	imgcopy = m_MyIPImage;
	dlg.SetImage(imgcopy);
	if (dlg.DoModal() == IDOK) {						//만약 다이얼로그에서 확인을 누르면
		if (IP_Threshold(imgcopy, dlg.m_nThreshold)) {	//만약 감마보정이 다 됬으면 
			AfxNewImage(_T("Threshold"), imgcopy);	//AfxNewImage함수에 img를 전달한다.
			AfxPrintInfo(_T("[COMPLETE] Threshold"));
		}
		else
			AfxPrintInfo(_T("[FAIL] Threshold"));
	}
	dlg.m_pView->Invalidate(false);	//이걸 넣어주면 취소 해도 그대로 유지 
}

void CMyIPTool2Doc::OnBitplane()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnBitplane()"));

	CString title;
	MyIPImage imgcopy;
	imgcopy = m_MyIPImage;
	for (int bit = 0; bit < 8; bit++) {
		MyIPImage out;
		if (IP_Bitplane(imgcopy, out, bit)) {
			title.Format(L"BIT: %d", bit);
			AfxNewImage(title, out);	//AfxNewImage함수에 img를 전달한다.
		}
	}

}

void CMyIPTool2Doc::OnAutomaticthresholdIterative()
{
	AfxPrintInfo(_T("CMyIPTool2Doc::OnAutomaticthresholdIterative()"));
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MyIPImage imgcopy;
	imgcopy = m_MyIPImage;
	int threshold;

	//>> Doc, View 주소 전달해주기 (없으면 getpixel()에서 에러)
	CThresholdDlg dlg;
	dlg.m_pDoc = this;										//다이얼로그의 m_pDoc포인터 변수에 Doc.cpp의 주소를 전달
	POSITION pos = GetFirstViewPosition();					//도큐먼트 클래스에서 뷰 클래스의 인스턴스 포인터를 얻으려면 
	dlg.m_pView = (CMyIPTool2View*)GetNextView(pos);		//GetFirstViewPosition() 함수와 GetNextView() 함수를 조합하여 사용해야 한다. 
															//이렇게 복잡해지는 이유는 도큐먼트 하나에 여러개의 뷰가 연결될 수 있기 때문이다.
	//<< 

	if (IP_AutoThreshold_Iterative(imgcopy, &threshold)) {
		dlg.SetImage(imgcopy);
		dlg.m_nAutoThresholdingVal = threshold;
		if (dlg.DoModal() == IDOK) {
			if (IP_Threshold(imgcopy, dlg.m_nThreshold)) {
				AfxNewImage(_T("AutomaticthresholdIterative"), imgcopy);
				AfxPrintInfo(_T("[COMPLETE] Auto Threshold Iterative: %d"), threshold);
			}
		}
		dlg.m_pView->Invalidate(false);	//이걸 넣어주면 취소 해도 그대로 유지 
	}
}

void CMyIPTool2Doc::OnAutomaticthresholdOtsu()
{
	AfxPrintInfo(_T("CMyIPTool2Doc::OnAutomaticthresholdOtsu()"));
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MyIPImage imgcopy;
	imgcopy = m_MyIPImage;
	int threshold;

	//>> Doc, View 주소 전달해주기 (없으면 getpixel()에서 에러)
	CThresholdDlg dlg;
	dlg.m_pDoc = this;										//다이얼로그의 m_pDoc포인터 변수에 Doc.cpp의 주소를 전달
	POSITION pos = GetFirstViewPosition();					//도큐먼트 클래스에서 뷰 클래스의 인스턴스 포인터를 얻으려면 
	dlg.m_pView = (CMyIPTool2View*)GetNextView(pos);		//GetFirstViewPosition() 함수와 GetNextView() 함수를 조합하여 사용해야 한다. 
															//이렇게 복잡해지는 이유는 도큐먼트 하나에 여러개의 뷰가 연결될 수 있기 때문이다.
															//<< 

	if (IP_AutoThreshold_Otsu(imgcopy, &threshold)) {
		dlg.SetImage(imgcopy);
		dlg.m_nAutoThresholdingVal = threshold;
		if (dlg.DoModal() == IDOK) {
			if (IP_Threshold(imgcopy, threshold)) {
				AfxNewImage(_T("OTSU Thresholding"), imgcopy);
				AfxPrintInfo(_T("[COMPLETE] Auto Threshold Otsu: %d"), threshold);
			}
		}
		dlg.m_pView->Invalidate(false);	//이걸 넣어주면 취소 해도 그대로 유지 
	}
}

void CMyIPTool2Doc::OnAutomaticthresholdEntropy()
{
	AfxPrintInfo(_T("CMyIPTool2Doc::OnAutomaticthresholdEntropy()"));
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MyIPImage imgcopy;
	imgcopy = m_MyIPImage;
	int threshold;

	//>> Doc, View 주소 전달해주기 (없으면 getpixel()에서 에러)
	CThresholdDlg dlg;
	dlg.m_pDoc = this;										//다이얼로그의 m_pDoc포인터 변수에 Doc.cpp의 주소를 전달
	POSITION pos = GetFirstViewPosition();					//도큐먼트 클래스에서 뷰 클래스의 인스턴스 포인터를 얻으려면 
	dlg.m_pView = (CMyIPTool2View*)GetNextView(pos);		//GetFirstViewPosition() 함수와 GetNextView() 함수를 조합하여 사용해야 한다. 
															//이렇게 복잡해지는 이유는 도큐먼트 하나에 여러개의 뷰가 연결될 수 있기 때문이다.
															//<< 

	if (IP_AutoThreshold_Entropy(imgcopy, &threshold)) {
		dlg.SetImage(imgcopy);
		dlg.m_nAutoThresholdingVal = threshold;
		if (dlg.DoModal() == IDOK) {
			if (IP_Threshold(imgcopy, threshold)) {
				AfxNewImage(_T("ENTROPY Thresholding"), imgcopy);
				AfxPrintInfo(_T("[COMPLETE] Auto Threshold Entropy: %d"), threshold);
			}
		}
		dlg.m_pView->Invalidate(false);	//이걸 넣어주면 취소 해도 그대로 유지 
	}
}

void CMyIPTool2Doc::OnCvAutomaticthreshold()
{	
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnCvAutomaticthreshold()"));

	int depth = m_MyIPImage.GetDepth();
	int channel = m_MyIPImage.GetChannels();
	if ((depth != CV_8U && depth != CV_8S) || channel != 1) {	//이미지가 8비트가 아니라면 opencv thresholding 안된다.
		AfxMessageBox(_T("cv::threshold() only support 8bit grayscale image"));
		return;
	}
	Mat img;
	if (IpImageToMat(m_MyIPImage, img)) {
		CCVThresholdDlg dlg;
		if (dlg.DoModal() == IDOK)
		{
			CString str = L"";
			//m_nType
			switch (dlg.m_nType) {
			case 0: str += L"Binary ";			break;
			case 1: str += L"Binary Inverse ";	break;
			case 2: str += L"Trunc ";			break;
			case 3: str += L"Tozero ";			break;
			case 4: str += L"Tozero Inverse ";	break;
			}
			//m_nMethod
			switch (dlg.m_nMethod) {
			case 0: str += L"Mask Treshold";		dlg.m_nMethod = 7; 	break;
			case 1: str += L"Otsu Treshold";		dlg.m_nMethod = 8;	break;
			case 2: str += L"Triangle Treshold";	dlg.m_nMethod = 16;	break;
			}

			if (IP_AutoThreshold_OpenCV(img,dlg.m_nType,dlg.m_nMethod)) {
				AfxNewImage(str, img);
			}
		}
	}
}

void CMyIPTool2Doc::OnCvHistoEqualization()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnCvHistoEqualization()"));

	int depth = m_MyIPImage.GetDepth();
	int channel = m_MyIPImage.GetChannels();
	if ((depth != CV_8U && depth != CV_8S) || channel != 1) {	//이미지가 8비트가 아니라면 opencv thresholding 안된다.
		AfxMessageBox(_T("cv::equalizeHist() only support 8bit grayscale image"));
		return;
	}

	Mat img,out;
	if (IpImageToMat(m_MyIPImage, img)) {
		equalizeHist(img, out);
		AfxNewImage(_T("CV Histo Equalization"),out);
	}
}


void CMyIPTool2Doc::OnConvert8bitto16bit()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnConvert8bitto16bit()"));
	MyIPImage img;
	if (IpConvert8to16(m_MyIPImage, img)) {
		AfxNewImage(img);
	}
}


void CMyIPTool2Doc::OnSpatialfilterAverage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnSpatialfilterAverage()"));
	CSpetialFilterDlg dlg;
	dlg.m_nFilterType = FILTER_AVR;
	if (dlg.DoModal() == IDOK) {
		MyIPImage img;
		img = m_MyIPImage;
		int w = dlg.m_nWidth;
		int h = dlg.m_nHeight;
		int borderType = dlg.m_nBorderType;
		int value = dlg.m_nBorderValue;

		double *buffer = new double[h*w];
		double **kernel = new double*[h];
		for (int y = 0; y < h; y++) {
			kernel[y] = buffer + y*w;		//double이기에 8*y*w임
		}
		double v = 1. / (double)(h*w);
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				kernel[y][x] = v;
			}
		}
		TIMECHECK_START
			if (IpFilter2D(img, h, w, kernel, borderType, value)) {
				AfxNewImage(_T("MyIPImage Average"), img);
			}
		TIMECHECK_END
			delete[] kernel;
		delete[] buffer;
	}
}


void CMyIPTool2Doc::OnSpatialfilterGaussian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnSpatialfilterGaussian()"));
	CSpetialFilterDlg dlg;
	dlg.m_nFilterType = FILTER_GAUSSIAN;
	if (dlg.DoModal() == IDOK) {
		MyIPImage img;
		img = m_MyIPImage;

		int borderType = dlg.m_nBorderType;
		int value = dlg.m_nBorderValue;

		int h, w;
		double n, sigma;
		
		if(dlg.m_nGaussianOption==0){			//kernel
			h = dlg.m_nHeight;
			w = dlg.m_nWidth;
			n = dlg.m_dNsigma;
			//sigma = 0.3 * ((h - 1) * 0.5 - 1) + 0.8;
			sigma = (h - 1) / (2 * n);
		}
		else if (dlg.m_nGaussianOption == 1) {	//sigma
			n = dlg.m_dNsigma;
			sigma = dlg.m_dSigmaVal;					//나중에 dlg에서 받아오게끔
			h = (int)(n*sigma + 0.5) * 2 + 1;	//3 Sigma
			w = h;
		}

		double *buffer = new double[h*w];
		double **kernel = new double*[h];
		for (int y = 0; y < h; y++) {
			kernel[y] = buffer + y*w;		//double이기에 8*y*w임
		}

		TIMECHECK_START
			if (IpGaussianKernel(kernel, h, w, sigma)) {
				if (IpFilter2D(img, h, w, kernel, borderType, value)) {
					AfxNewImage(_T("MyIPImage Gaussian"), img);
				}
			}
		TIMECHECK_END
		delete[] kernel;
		delete[] buffer;
	}
}


void CMyIPTool2Doc::OnSpatialfilterMaximum()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnSpatialfilterMaximum()"));
	CSpetialFilterDlg dlg;
	dlg.m_nFilterType = FILTER_MAX;
	if (dlg.DoModal() == IDOK) {
		MyIPImage img;
		img = m_MyIPImage;

		int w = dlg.m_nWidth;
		int h = dlg.m_nHeight;
		int borderType = dlg.m_nBorderType;
		int value = dlg.m_nBorderValue;

		TIMECHECK_START
			if (IpFilterMax(img, h, w, borderType, value)) {//곱하는게 아니니까 커널이 필요없지!! 
				AfxNewImage(_T("MyIPImage Maximum"), img);
			}
		TIMECHECK_END
	}
}

void CMyIPTool2Doc::OnSpatialfilterMinumum()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnSpatialfilterMinumum()"));
	CSpetialFilterDlg dlg;
	dlg.m_nFilterType = FILTER_MIN;
	if (dlg.DoModal() == IDOK) {
		MyIPImage img;
		img = m_MyIPImage;

		int w = dlg.m_nWidth;
		int h = dlg.m_nHeight;
		int borderType = dlg.m_nBorderType;
		int value = dlg.m_nBorderValue;

		TIMECHECK_START
			if (IpFilterMin(img, h, w, borderType, value)) {//곱하는게 아니니까 커널이 필요없지!! 
				AfxNewImage(_T("MyIPImage Minimum"), img);
			}
		TIMECHECK_END
	}
}


void CMyIPTool2Doc::OnSpatialfilterMedian()		
{
	AfxPrintInfo(_T("CMyIPTool2Doc::OnSpatialfilterMedian()"));
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSpetialFilterDlg dlg;
	dlg.m_nFilterType = FILTER_MED;
	if (dlg.DoModal() == IDOK) {
		MyIPImage img;
		img = m_MyIPImage;

		int h = dlg.m_nHeight;
		int w = dlg.m_nWidth;

		TIMECHECK_START
			if (IpFilterMed(img, h, w)) {
				AfxNewImage(_T("MyIPImage Median"), img);
			}
		TIMECHECK_END
	}
}


void CMyIPTool2Doc::OnSpatialfilterAdaptiveMedian()
{
	AfxPrintInfo(_T("CMyIPTool2Doc::OnSpatialfilterAdaptiveMedian()"));
	CSpetialFilterDlg dlg;
	dlg.m_nFilterType = FILTER_MED_ADP;
	if (dlg.DoModal() == IDOK) {
		MyIPImage img;
		img = m_MyIPImage;

		int maxsize = dlg.m_nFilterMaxSize;

		TIMECHECK_START
			if (IpFilterAdpMed(img, maxsize)) {//곱하는게 아니니까 커널이 필요없지!! 
				AfxNewImage(_T("MyIPImage Adp Med"), img);
			}
		TIMECHECK_END
	}
}


void CMyIPTool2Doc::OnCvSpatialfilterAverage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnCvSpatialfilterAverage()"));
	Mat img, out;
	if (IpImageToMat(m_MyIPImage, img)) {
		CSpetialFilterDlg dlg;
		dlg.m_nFilterType = FILTER_AVR;
		dlg.bIsOpencv = true;
		if (dlg.DoModal() == IDOK) {
			double avr = 0.0;
			for (int loop = 0; loop < 10; loop++) {
				int w = dlg.m_nWidth;
				int h = dlg.m_nHeight;
				int borderType = dlg.m_nBorderType;
				int value = dlg.m_nBorderValue;

				int kw2 = w / 2;
				int kh2 = h / 2;


				Mat kernel(h, w, CV_32F);
				double v = 1. / (double)(h*w);
				kernel.setTo(v);

				int depth = img.depth();

				TIMECHECK_START

					if (borderType == IOP_BORDER_WRAP || borderType == IOP_BORDER_CONSTANT) {
						Mat border;
						Rect crop(kw2, kh2, img.rows, img.cols);
						if (borderType == IOP_BORDER_WRAP)
							copyMakeBorder(img, border, kh2, kh2, kw2, kw2, BORDER_WRAP);
						else if (borderType == IOP_BORDER_CONSTANT)
							copyMakeBorder(img, border, kh2, kh2, kw2, kw2, BORDER_CONSTANT, Scalar(value, value, value));
						blur(border, border, kernel.size(), Point(-1, -1), BORDER_DEFAULT);
						out = border(crop);
					}
					else
						filter2D(img, out, -1, kernel, Point(-1, -1), 0.0, borderType);	//BORDER_WRAP 지원X

				TIMECHECK_END
					avr += (double)(endtime - s);
					AfxNewImage(_T("CV Average"), out);
			}
			AfxPrintInfo(L"average : %f", avr);
		}
	}
}


void CMyIPTool2Doc::OnCvSpatialfilterGaussian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnCvSpatialfilterGaussian()"));
	Mat img, out;
	if (IpImageToMat(m_MyIPImage, img)) {
		CSpetialFilterDlg dlg;
		dlg.m_nFilterType = FILTER_GAUSSIAN;
		dlg.bIsOpencv = true;
		if (dlg.DoModal() == IDOK) {
			int borderType = dlg.m_nBorderType;
			int value = dlg.m_nBorderValue;

			int h, w;
			double n, sigma;

			if (dlg.m_nGaussianOption == 0) {			//kernel
				h = dlg.m_nHeight;
				w = dlg.m_nWidth;
				n = dlg.m_dNsigma;
				sigma = (h - 1) / (2 * n);
			}
			else if (dlg.m_nGaussianOption == 1) {	//sigma
				n = dlg.m_dNsigma;
				sigma = dlg.m_dSigmaVal;					//나중에 dlg에서 받아오게끔
				h = (int)(n*sigma + 0.5) * 2 + 1;	//3 Sigma
				w = h;
			}

			TIMECHECK_START
// 				if (dlg.m_nGaussianOption == 0)			//calculate sigma from kernel size
// 					GaussianBlur(img, out, Size(w, h), 0, 0, borderType);
// 				else
					GaussianBlur(img, out, Size(w, h), sigma, sigma, borderType);
			TIMECHECK_END
				AfxNewImage(_T("CV Gaussian"), out);
		}
	}
}


void CMyIPTool2Doc::OnCvSpatialfilterMaximum()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnCvSpatialfilterMaximum()"));
	Mat img, out;
	if (IpImageToMat(m_MyIPImage, img)) {
		CSpetialFilterDlg dlg;
		dlg.m_nFilterType = FILTER_MAX;
		dlg.bIsOpencv = true;
		if (dlg.DoModal() == IDOK) {

			int w = dlg.m_nWidth;
			int h = dlg.m_nHeight;
			int borderType = dlg.m_nBorderType;
			int value = dlg.m_nBorderValue;

			Mat kernel(h, w,img.depth());
			TIMECHECK_START
				dilate(img, out, kernel, Point(-1, -1), 1, borderType, value);
			TIMECHECK_END
				AfxNewImage(_T("CV Maximum"), out);
		}
	}
}


void CMyIPTool2Doc::OnCvSpatialfilterMinumum()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnCvSpatialfilterMinumum()"));
	Mat img, out;
	if (IpImageToMat(m_MyIPImage, img)) {
		CSpetialFilterDlg dlg;
		dlg.m_nFilterType = FILTER_MIN;
		dlg.bIsOpencv = true;
		if (dlg.DoModal() == IDOK) {

			int w = dlg.m_nWidth;
			int h = dlg.m_nHeight;
			int borderType = dlg.m_nBorderType;
			int value = dlg.m_nBorderValue;

			Mat kernel(h, w, img.depth());
			TIMECHECK_START
				erode(img, out, kernel, Point(-1, -1), 1, borderType, value);
			TIMECHECK_END
				AfxNewImage(_T("CV Minimum"), out);
		}
	}
}


void CMyIPTool2Doc::OnCvSpatialfilterMedian()
{
	AfxPrintInfo(_T("CMyIPTool2Doc::OnCvSpatialfilterMedian()"));
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Mat img, out;
	if (IpImageToMat(m_MyIPImage, img)) {
		CSpetialFilterDlg dlg;
		dlg.m_nFilterType = FILTER_MED;
		dlg.bIsOpencv = true;
		if (dlg.DoModal() == IDOK) {

			int w = dlg.m_nWidth;
			int h = dlg.m_nHeight;
			int borderType = dlg.m_nBorderType;
			int value = dlg.m_nBorderValue;

			TIMECHECK_START
				//IpFilterMed(img, h, w);
				medianBlur(img, out, h);
			TIMECHECK_END
				//AfxNewImage(_T("CV Median(My)"), img);
				AfxNewImage(_T("CV Median"), out);
		}
	}
}


void CMyIPTool2Doc::OnCvSpatialfilterAdaptiveMedian()
{
	AfxPrintInfo(_T("CMyIPTool2Doc::OnCvSpatialfilterAdaptiveMedian()"));
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Mat img;
	if (IpImageToMat(m_MyIPImage, img)) {
		CSpetialFilterDlg dlg;
		dlg.m_nFilterType = FILTER_MED_ADP;
		dlg.bIsOpencv = true;
		if (dlg.DoModal() == IDOK) {

			int maxsize = dlg.m_nFilterMaxSize;

			TIMECHECK_START;
			IpFilterAdpMed(img, maxsize);
			TIMECHECK_END
				AfxNewImage(_T("CV Adaptive Median"), img);
		}
	}
}


// void CMyIPTool2Doc::OnAddnoiseGaussian()
// {
// 	// TODO: 여기에 명령 처리기 코드를 추가합니다.
// 	AfxPrintInfo(_T("CMyIPTool2Doc::OnAddnoiseGaussian()"));
// 	CNoiseDlg dlg;
// 
// 	if (dlg.DoModal() == IDOK) {
// 		MyIPImage img;
// 		img = m_MyIPImage;
// 
// 		if (IpAddGaussianNoise(img, dlg.m_dSigma)) {
// 			AfxNewImage(_T("Gaussian Noise"), img);
// 		}
// 	}
// 
// 
// 	//GaussianRand()함수 이용한 노이즈 범위 살펴보기 
// 	// 	for (int noise = 0; noise < 80; noise++) {
// 	// 		double v = GaussianRand(0., 3);
// 	// 		AfxPrintInfo(_T("%d: %f"), noise, v);
// 	// 	}
// }


//void CMyIPTool2Doc::OnAddnoiseSaltpepper()
//{
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//	AfxPrintInfo(_T("CMyIPTool2Doc::OnAddnoiseSaltpepper()"));
//	CNoiseDlg dlg;
//	if (dlg.DoModal() == IDOK) {
//
//		MyIPImage img;
//		img = m_MyIPImage;
//
//		if (IpAddSaltPepperNoise(img, dlg.m_dSalt, dlg.m_dPepper)) {
//			AfxNewImage(_T("Gaussian Noise"), img);
//		}
//	}
//}





void CMyIPTool2Doc::OnMyipimageAddnoise()
{
	AfxPrintInfo(_T("CMyIPTool2Doc::OnMyipimageAddnoise()"));
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CNoiseDlg dlg;

	if (dlg.DoModal() == IDOK) {
		MyIPImage img;
		img = m_MyIPImage;

		switch (dlg.m_nRadioNoise)
		{
		case 0:	//Gaussian
			if (IpAddGaussianNoise(img, dlg.m_dSigma))
				AfxNewImage(_T("Gaussian Noise"), img);
			break;

		case 1:	//Salt Pepper
			if (IpAddSaltPepperNoise(img, dlg.m_dSalt, dlg.m_dPepper, dlg.m_bCheckRGB)) 
				AfxNewImage(_T("Salt Pepper Noise"), img);
			break;
		}
	}
}



void CMyIPTool2Doc::OnSharpeningHighboostfilter()
{
	AfxPrintInfo(_T("CMyIPTool2Doc::OnSharpeningHighboostfilter()"));
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CShapeningFilterDlg dlg;
	dlg.m_nFilterType = SHP_HIGHBOOST;

	if (dlg.DoModal() == IDOK) {
		//for highboosting 
		double sigma = dlg.m_dSigma;	//gaussian sigma 
		double k = dlg.m_dWeight;		//highboost weight 

		//goussian kernel calculating 
		int h = (int)(3 * sigma + 0.5) * 2 + 1;	//3 Sigma
		int w = h;

		double **kernel = new double *[h];
		for (int i = 0; i < h; i++)
			kernel[i] = new double[w];

		MyIPImage img, GAUimg, PosSUBimg, NegSUBimg, out1, out2;
		img = m_MyIPImage;
		GAUimg = m_MyIPImage;


		TIMECHECK_START;

		//get gaussian image 
		if (!IpGaussianKernel(kernel, h, w, sigma)) 			//가우시안 커널 생성
			return;
		if (!IpFilter2D(GAUimg, h, w, kernel, BORDER_REFLECT)) 	//가우시안 필터 적용
			return;
		//AfxNewImage(_T("gau"), GAUimg);

		//get subtracted image 
		if (!IP_ARLogic(img, GAUimg, PosSUBimg, IOP_SUB))	//positive
			return;
		if (!IP_ARLogic(GAUimg, img, NegSUBimg, IOP_SUB))	//negative
			return;
		//AfxNewImage(_T("img-gau"), PosSUBimg);
		//AfxNewImage(_T("gau-img"), NegSUBimg);

		//calc K * SUBimg
		if (k != 1) {				//highboost filter 
			MyIPImage kPosSUBimg, kNegSUBimg;
			if (!IP_ARLogic(PosSUBimg, PosSUBimg, kPosSUBimg, IOP_MUL, true, 0, k))	//positive
				return;
			if (!IP_ARLogic(NegSUBimg, NegSUBimg, kNegSUBimg, IOP_MUL, true, 0, k))	//negative 
				return;
			if (!IP_ARLogic(img, kPosSUBimg, out1, IOP_ADD))
				return;
			if (!IP_ARLogic(out1, kNegSUBimg, out2, IOP_SUB))
				return;
		}
		else {						//unsharp masking filter 
			//calc img + K SUBimg
			if (!IP_ARLogic(img, PosSUBimg, out1, IOP_ADD))
				return;
			if (!IP_ARLogic(out1, NegSUBimg, out2, IOP_SUB))
				return;
		}

		if (k != 1)
			AfxNewImage(_T("High Boost Filter"), out2);
		else
			AfxNewImage(_T("Unsharp Masking Filter"), out2);

		TIMECHECK_END;

		for (int i = 0; i < h; i++)
			delete[] kernel[i];
		delete[] kernel;
	}
}


void CMyIPTool2Doc::OnSharpeningLaplaciansharpening()
{
	AfxPrintInfo(_T("CMyIPTool2Doc::OnSharpeningLaplaciansharpening()"));
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CShapeningFilterDlg dlg;
	dlg.m_nFilterType = SHP_LAPLACIAN;

	if (dlg.DoModal() == IDOK) {
		MyIPImage img, Limg, out_neg, out_pos;
		img = m_MyIPImage;
		Limg = img;
		//1. get laplacian image

// 		if (!IpLaplacian(Limg, EDGE_LAPLACIAN4 + dlg.m_nLaplacianType, FALSE))		//-> 0~255 limitted 
// 			return;
// 		AfxNewImage(_T("Laplacian"), Limg);


		double maxican4[3][3] = {
			 0,-1, 0,
			-1, 5,-1,
			 0,-1, 0
		};
		double maxican8[3][3] = {
			-1,-1,-1,
			-1, 9,-1,
			-1,-1,-1
		};
		double **kernel = new double*[3];
		for (int y = 0; y < 3; y++)
			kernel[y] = new double[3];		//double이기에 8*y*w임

		for (int i = 0; i < 9; i++) {
			if(dlg.m_nLaplacianType == 0)				//laplacian4
				kernel[i / 3][i % 3] = maxican4[i / 3][i % 3];
			else if (dlg.m_nLaplacianType == 1)			//laplacian8
				kernel[i / 3][i % 3] = maxican8[i / 3][i % 3];
		}

		
		TIMECHECK_START
			if (IpFilter2D(img, 3, 3, kernel, BORDER_REFLECT))
				AfxNewImage(_T("Laplacian Sharpening"), img);
		TIMECHECK_END

		//2. get laplacian sharpen image (img - Limg)
// 		if (!IP_ARLogic(img, Limg, out_neg, IOP_SUB))
// 			return;

	} 
}


void CMyIPTool2Doc::OnSpatialfilterEdgedetector()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CEdgeDetectorDlg dlg;
	CString str;

	dlg.m_pDoc = this;									//for DisplayImage Function
	POSITION pos = GetFirstViewPosition();				//for DisplayImage Function
	dlg.m_pView = (CMyIPTool2View*)GetNextView(pos);	//for DisplayImage Function

	if (dlg.DoModal() == IDOK) {
		MyIPImage img, angle;
		img = m_MyIPImage;

		switch (dlg.m_nEdgeType)
		{
		case EDGE_ROBERT:
		case EDGE_PREWITT:
		case EDGE_SOBEL:
			if (IpEdgeDetector(img, angle, dlg.m_nEdgeType, dlg.m_bNormalize)) {
				if (dlg.m_nEdgeType == EDGE_ROBERT)				str = _T("Roberto Edge ");
				else if (dlg.m_nEdgeType == EDGE_PREWITT)		str = _T("Prewitt Edge ");
				else if (dlg.m_nEdgeType == EDGE_SOBEL)			str = _T("Sobel Edge ");
				if (dlg.m_bNormalize)							str += _T("Normalized");
			}
			break;
		case EDGE_LAPLACIAN4:
		case EDGE_LAPLACIAN8:
			if (IpLaplacian(img, dlg.m_nEdgeType, dlg.m_bNormalize)) {
				if (dlg.m_nEdgeType == EDGE_LAPLACIAN4)			str = _T("Laplacian 4 Edge ");
				else if (dlg.m_nEdgeType == EDGE_LAPLACIAN8)	str = _T("Laplacian 8 Edge ");
				if (dlg.m_bNormalize)							str += _T("Normalized");
			}
			break;
		case EDGE_CANNY:
			if (IpCanny(img, dlg.m_nLowTh, dlg.m_nHighTh))		str = _T("Canny Edge ");
			break;
		case EDGE_LOG:
			if (IpLoG(img, dlg.m_dLogSigma))					str = _T("LoG Edge ");
			break;
		case EDGE_DOG:
			if (IpDoG(img, dlg.m_dDogSigma1, dlg.m_dDogSigma2)) str = _T("DoG Edge "); 
			break;
		}

		AfxNewImage(str, img);
	}
	dlg.m_pView->Invalidate(false);
}


void CMyIPTool2Doc::OnMorphology()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CString str;
	MyIPImage img, ref;
	img.Copy(m_MyIPImage);
	CShapeElement se;
	CMorphologyDlg dlg;

	if (dlg.DoModal() == IDOK) {
		switch (dlg.m_nSEType)
		{
		case SE_DISK:	
			if (!se.disk(dlg.m_nRadius))	return;
			str.Format(_T("_disk(%d)"),dlg.m_nRadius);
			break; 
		case SE_RING:
				if (!se.ring(dlg.m_nRadius))	return;
				str.Format(_T("_ring(%d)"), dlg.m_nRadius);
				break;
		case SE_RECTANGLE: 
			if (!se.box(dlg.m_nRectHeight, dlg.m_nRectWidth))	return;
			str.Format(_T("_box(%dx%d)"), dlg.m_nRectHeight, dlg.m_nRectWidth);
			break;
		case SE_FRAME:
			if (!se.frame(dlg.m_nRectHeight, dlg.m_nRectWidth))	return;
			str.Format(_T("_frame(%dx%d)"), dlg.m_nRectHeight, dlg.m_nRectWidth);
			break;
		case SE_CROSS:
			if (!se.cross(dlg.m_nRectHeight, dlg.m_nRectWidth))	return;
			str.Format(_T("_cross(%dx%d)"), dlg.m_nRectHeight, dlg.m_nRectWidth);
			break;
		case SE_DIAMOND:
			break;
		}

		switch (dlg.m_nMorphologyType)
		{
		case MORPH_ERODE:
			if (!IpErotion(img, se))	return;
			str = _T("Morph Erosion") + str;
			break;
		case MORPH_DILATE:
			if (!IpDilation(img, se))	return;
			str = _T("Morph Dilation") + str;
			break;
		case MORPH_OPEN:
			if (!IpErotion(img, se))	return;
			if (!IpDilation(img, se))	return;
			str = _T("Morph Opening") + str;
			break;
		case MORPH_CLOSE:
			if (!IpDilation(img, se))	return;
			if (!IpErotion(img, se))	return;
			str = _T("Morph Closing") + str;
			break;
		case 4:	//Conditional Dialation
			ref = dlg.m_pDoc1->m_MyIPImage;
			if (!IpConditional(img, ref))	return;
			str = _T("Conditional Dialation");
			break;
		}

		AfxNewImage(str, img);
	}
}


void CMyIPTool2Doc::OnGrayscaleMorphology()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CString str;
	MyIPImage img, ref,residue;
	img.Copy(m_MyIPImage);
	CShapeElement se;
	CMorphologyDlg dlg;

	dlg.bGrayMorphFlag = true;
	if (dlg.DoModal() == IDOK) {
		switch (dlg.m_nSEType)
		{
		case SE_DISK:
			if (!se.disk(dlg.m_nRadius))	return;
			str.Format(_T("_disk(%d)"), dlg.m_nRadius);
			break;
		case SE_RING:
			if (!se.ring(dlg.m_nRadius))	return;
			str.Format(_T("_ring(%d)"), dlg.m_nRadius);
			break;
		case SE_RECTANGLE:
			if (!se.box(dlg.m_nRectHeight, dlg.m_nRectWidth))	return;
			str.Format(_T("_box(%dx%d)"), dlg.m_nRectHeight, dlg.m_nRectWidth);
			break;
		case SE_FRAME:
			if (!se.frame(dlg.m_nRectHeight, dlg.m_nRectWidth))	return;
			str.Format(_T("_frame(%dx%d)"), dlg.m_nRectHeight, dlg.m_nRectWidth);
			break;
		case SE_CROSS:
			if (!se.cross(dlg.m_nRectHeight, dlg.m_nRectWidth))	return;
			str.Format(_T("_cross(%dx%d)"), dlg.m_nRectHeight, dlg.m_nRectWidth);
			break;
		case SE_DIAMOND:
			break;
		}

		switch (dlg.m_nMorphologyType)
		{
		case MORPH_ERODE:
			if (!IpGrayErotion(img, se))	return;
			str = _T("Morph Erosion") + str;
			break;
		case MORPH_DILATE:
			if (!IpGrayDilation(img, se))	return;
			str = _T("Morph Dilation") + str;
			break;
		case MORPH_OPEN:
			if (!IpGrayErotion(img, se))	return;
			if (!IpGrayDilation(img, se))	return;
 			str = _T("Morph Opening") + str;
			break;
		case MORPH_CLOSE:
			if (!IpGrayDilation(img, se))	return;
			if (!IpGrayErotion(img, se))	return;
 			str = _T("Morph Closing") + str;
			break;
		case 4:	//Conditional Dialation
			ref = dlg.m_pDoc1->m_MyIPImage;
			if (!IpConditional(img, ref))	return;
			str = _T("Conditional Dialation");
			break;
		case 5:	//Opening Residue 
			ref = m_MyIPImage;
			residue = m_MyIPImage;
			if (!IpGrayErotion(residue, se))	return;
			if (!IpGrayDilation(residue, se))	return;
			if (!IP_ARLogic(ref, residue, img, IOP_SUB))	return;
			str = _T("Opening Residue") + str;
			break;
		case 6:	//Closing Residue 
			ref = m_MyIPImage;
			residue = m_MyIPImage;
			if (!IpGrayDilation(residue, se))	return;
			if (!IpGrayErotion(residue, se))	return;
			if (!IP_ARLogic(residue, ref, img, IOP_SUB))	return;
			str = _T("Closing Residue") + str;
			break;
		}

		AfxNewImage(str, img);
	}
}


void CMyIPTool2Doc::OnCvMorphology()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CString str;
	Mat img, out;
	if (!IpImageToMat(m_MyIPImage, img))	return;

	CShapeElement se;
	CMorphologyDlg dlg;

	if (dlg.DoModal() == IDOK) {
		switch (dlg.m_nSEType)
		{
		case SE_DISK:
			if (!se.disk(dlg.m_nRadius))	return;
			str.Format(_T("_disk(%d)"), dlg.m_nRadius);
			break;
		case SE_RING:
			if (!se.ring(dlg.m_nRadius))	return;
			str.Format(_T("_ring(%d)"), dlg.m_nRadius);
			break;
		case SE_RECTANGLE:
			if (!se.box(dlg.m_nRectHeight, dlg.m_nRectWidth))	return;
			str.Format(_T("_box(%dx%d)"), dlg.m_nRectHeight, dlg.m_nRectWidth);
			break;
		case SE_FRAME:
			if (!se.frame(dlg.m_nRectHeight, dlg.m_nRectWidth))	return;
			str.Format(_T("_frame(%dx%d)"), dlg.m_nRectHeight, dlg.m_nRectWidth);
			break;
		case SE_CROSS:
			if (!se.cross(dlg.m_nRectHeight, dlg.m_nRectWidth))	return;
			str.Format(_T("_cross(%dx%d)"), dlg.m_nRectHeight, dlg.m_nRectWidth);
			break;
		case SE_DIAMOND:
			break;
		}

		//make mask from structuring element
		CPoint* pt = se.GetPoint();									//array for CPoint Class Variable
		CPoint center = se.GetCenter();
		CPoint point;
		int nse = se.GetSize();										//element size
		int kh = 0, kw = 0;											//height, width of structure element
		for (int k = 0; k < nse; k++) {								//
			if (abs(pt[k].x) > kw)	kw = (abs(pt[k].x));			//get largest x
			if (abs(pt[k].y) > kh)	kh = (abs(pt[k].y));			// "	"	  y
		}
		kh = 2 * kh + 1;											//element height
		kw = 2 * kw + 1;											//element width

		Mat mask(kh, kw, CV_8U, Scalar(0));							//making mask
		for (int k = 0; k < nse; k++) {								
			point = pt[k] + center;
			mask.at<byte>(point.y, point.x) = (byte)255;
		}
		imshow("mask", mask);

		switch (dlg.m_nMorphologyType)
		{
		case MORPH_ERODE:
			morphologyEx(img, out, MORPH_ERODE, mask);
			str = _T("CV Morph Erosion") + str;
			break;
		case MORPH_DILATE:
			morphologyEx(img, out, MORPH_DILATE, mask);
			str = _T("CV Morph Dilation") + str;
			break;
		case MORPH_OPEN:
			morphologyEx(img, out, MORPH_OPEN, mask);
			str = _T("CV Morph Opening") + str;
			break;
		case MORPH_CLOSE:
			morphologyEx(img, out, MORPH_CLOSE, mask);
			str = _T("CV Morph Closing") + str;
			break;
		}

		AfxNewImage(str, out);
	}
}


void CMyIPTool2Doc::OnMorphologyTest()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CString str;
	MyIPImage img, ref;
	img.Copy(m_MyIPImage);
	CShapeElement se;
	CMorphologyDlg dlg;
	CLoopDlg loopdlg;
	//set loop 
	MORPHTEST *arr = new MORPHTEST[20];
	int loopcnt = 0;
	int arrcnt = 0;
	while(1){
		if (dlg.DoModal() == IDOK) {
			if (loopdlg.DoModal() == IDOK) {
				arr[arrcnt].nMorphType = dlg.m_nMorphologyType;
				arr[arrcnt].nSEType = dlg.m_nSEType;
				arr[arrcnt].nRadius = dlg.m_nRadius;
				arr[arrcnt].nHeight = dlg.m_nRectHeight;
				arr[arrcnt].nWidth = dlg.m_nRectWidth;
				arr[arrcnt].nLoopCnt = loopdlg.m_nLoopCnt;
				if (loopdlg.bAddFlag == false)
					break;
				else
					arrcnt++;
			}
		}
		else
			return;
	}
	//loop operation
	for (int a = 0; a <= arrcnt; a++) {
		for (int b = 0; b < arr[a].nLoopCnt; b++) {
			img.Copy(m_MyIPImage);
			switch (arr[a].nSEType)
			{
			case SE_DISK:
				if (!se.disk(arr[a].nRadius))	return;
				str.Format(_T("_disk(%d)"), arr[a].nRadius);
				break; 
			case SE_RING:
				if (!se.ring(arr[a].nRadius))	return;
				str.Format(_T("_ring(%d)"), arr[a].nRadius);
				break;
			case SE_RECTANGLE:
				if (!se.box(arr[a].nHeight, arr[a].nWidth))	return;
				str.Format(_T("_box(%dx%d)"), arr[a].nHeight, arr[a].nWidth);
				break;
			case SE_FRAME:
				if (!se.frame(arr[a].nHeight, arr[a].nWidth))	return;
				str.Format(_T("_frame(%dx%d)"), arr[a].nHeight, arr[a].nWidth);
				break;
			case SE_CROSS:
				if (!se.cross(arr[a].nHeight, arr[a].nWidth))	return;
				str.Format(_T("_cross(%dx%d)"), arr[a].nHeight, arr[a].nWidth);
				break;
			case SE_DIAMOND:
				break;
			}

			switch (arr[a].nMorphType)
			{
			case MORPH_ERODE:
				if (!IpErotion(img, se))	return;
				str = _T("Morph Erosion") + str;
				break;
			case MORPH_DILATE:
				if (!IpDilation(img, se))	return;
				str = _T("Morph Dilation") + str;
				break;
			case MORPH_OPEN:
				// 		if (!IpErotion(img, se))	return;
				// 		if (!IpDilation(img, se))	return;
				// 		str = _T("Morph Opening") + str;
				break;
			case MORPH_CLOSE:
				// 		if (!IpDilation(img, se))	return;
				// 		if (!IpErotion(img, se))	return;
				// 		str = _T("Morph Closing") + str;
				break;
			case 4:	//Conditional Dialation
		// 		ref = dlg.m_pDoc1->m_MyIPImage;
		// 		if (!IpConditional(img, ref))	return;
		// 		str = _T("Conditional Dialation");
				break;
			}

			//AfxNewImage(str, img);
		}
	}
}

void CMyIPTool2Doc::OnWebcamLivecam()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	POSITION pos = GetFirstViewPosition();					//도큐먼트 클래스에서 뷰 클래스의 인스턴스 포인터를 얻으려면 
	CMyIPTool2View *m_pView = (CMyIPTool2View*)GetNextView(pos);		//GetFirstViewPosition() 함수와 GetNextView() 함수를 조합하여 사용해야 한다. 
															//이렇게 복잡해지는 이유는 도큐먼트 하나에 여러개의 뷰가 연결될 수 있기 때문이다.
	CClientDC dc(m_pView);


	VideoCapture capture(0);	//default camera open
	Mat video;
	MyIPImage img, grayimg, edgeimg, angle;
	img = m_MyIPImage;
	while (true) {
		capture >> video;

		IpMatToImage(video, img);
		if (IpRGBToGray(img, grayimg)) {
			if (IpEdgeDetector(grayimg, angle, EDGE_SOBEL, true)) {
				grayimg.Draw(dc.m_hDC, 0, 0);
			}
			//grayimg.Draw(dc.m_hDC, 0, 0);
		} 
		if (waitKeyEx(1) > 0) {
			IpMatToImage(video, img);
			break;
		}
	}
	//destroyAllWindows();
	AfxNewImage(_T("end while"), img);




}


void CMyIPTool2Doc::OnFourierTransform()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CString title;

	//1. Convert MyIPImage -> Mat
	Mat img;											
	if (!IpImageToMat(m_MyIPImage, img))	return;		//Convert Input Image to Mat

	//2. Zero Padding & cv::dft()
	Mat fft_img;
	if (!IpFFT2D(img, fft_img))	return;				//Make Pad Image

	//3. FFT / Fourier Spectrum
	Mat fft_spectrum;
	if (!IpFFTSpectrum(fft_img, fft_spectrum))	return;

	title = GetTitle() + _T("_FFT");
	AfxNewImage(title, fft_spectrum);
	AfxPrintInfo(_T("FFT Image : %s"), title);
	AfxPrintInfo(_T("H: %d     W:%d"), fft_img.rows,fft_img.cols);

	//4. IFFT
	Mat ifft_img(img.size(), img.type());
	if (!IpIFFT2D(fft_img, ifft_img))	return;				//Make Pad Image

	title = GetTitle() + _T("_IFFT");
	AfxNewImage(title, ifft_img);
}


void CMyIPTool2Doc::OnFreqFilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CString title;
	CFrequencyFilterDlg dlg;


	//1. Convert MyIPImage -> Mat
	Mat img;
	if (!IpImageToMat(m_MyIPImage, img))	return;	

	//2. show dialog 
	if (dlg.DoModal() != IDOK)	return;

	//3. make filter
	Mat filter;
	filter = IpMakeFilter(img, dlg.m_nFilterType, dlg.m_nFilterShape, dlg.m_nCOFval, dlg.m_nBPFval, dlg.m_nBUTval);

	//4. do filter 
	if (!IpFreqFilter(img, filter, dlg.m_bDoScale, dlg.m_bAddAvr))	return;

	//5. Show Image
	AfxNewImage(title, img);
}


void CMyIPTool2Doc::OnDftfilter2dAverage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnDftfilter2dAverage()"));
	CSpetialFilterDlg dlg;
	dlg.m_nFilterType = FILTER_AVR;
	if (dlg.DoModal() == IDOK) {
		Mat img, dst;
		if (!IpImageToMat(m_MyIPImage, img))	return;
		dst = Mat(img.size(), img.type());

		int w = dlg.m_nWidth;
		int h = dlg.m_nHeight;

		double *buffer = new double[h*w];
		double **kernel = new double*[h];
		for (int y = 0; y < h; y++) {
			kernel[y] = buffer + y*w;		//double이기에 8*y*w임
		}
		double v = 1. / (double)(h*w);
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				kernel[y][x] = v;
				//kernel[y][x] = 1;
			}
		}
		TIMECHECK_START
			if (IpFilter2D_DFT(img, dst, h, w, kernel)) {
				AfxNewImage(_T("DFT Filter2D Average"), dst);
			}
		TIMECHECK_END
			delete[] kernel;
		delete[] buffer;
	}
}


void CMyIPTool2Doc::OnDftfilter2dGaussian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnDftfilter2dGaussian()"));
	CSpetialFilterDlg dlg;
	dlg.m_nFilterType = FILTER_GAUSSIAN;
	if (dlg.DoModal() == IDOK) {
		Mat img, dst;
		if (!IpImageToMat(m_MyIPImage, img))	return;
		dst = Mat(img.size(), img.type());

		int h, w;
		double n, sigma;

		if (dlg.m_nGaussianOption == 0) {			//kernel
			h = dlg.m_nHeight;
			w = dlg.m_nWidth;
			n = dlg.m_dNsigma;
			//sigma = 0.3 * ((h - 1) * 0.5 - 1) + 0.8;
			sigma = (h - 1) / (2 * n);
		}
		else if (dlg.m_nGaussianOption == 1) {	//sigma
			n = dlg.m_dNsigma;
			sigma = dlg.m_dSigmaVal;					//나중에 dlg에서 받아오게끔
			h = (int)(n*sigma + 0.5) * 2 + 1;	//3 Sigma
			w = h;
		}

		double *buffer = new double[h*w];
		double **kernel = new double*[h];
		for (int y = 0; y < h; y++) {
			kernel[y] = buffer + y*w;		//double이기에 8*y*w임
		}

		TIMECHECK_START
			if (IpGaussianKernel(kernel, h, w, sigma)) {
				if (IpFilter2D_DFT(img, dst, h, w, kernel)) {
					AfxNewImage(_T("DFT Filter2D Gaussian"), dst);
				}
			}
		TIMECHECK_END
			delete[] kernel;
		delete[] buffer;
	}
}


void CMyIPTool2Doc::OnObjectdetectionPatternmatching()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPatternMatchingDlg dlg;
	if (dlg.DoModal()==IDOK) {
		Mat img, pattern, out;
		int mode = dlg.m_ctrlRadioPmMode;
		if (IpImageToMat(m_MyIPImage, img) && IpImageToMat(dlg.m_pDoc->m_MyIPImage, pattern)) {
			TIMECHECK_START
			if (mode == 0)			{
				if (IpPatternMatching(img, pattern, out, dlg.m_nObjects, dlg.m_fNCCThreshold)) {	//out: Map of comparison results
					AfxNewImage(_T("Pattern Matching Image"), img);
					AfxNewImage(_T("Pattern Matching Result"), out);
				}
			}
			else if (mode == 1) {
				if (IpPatternMatching_Thresold(img, pattern, out, dlg.m_fNCCThreshold)) {	//out: Map of comparison results
					AfxNewImage(_T("Pattern Matching Image"), img);
					AfxNewImage(_T("Pattern Matching Result"), out);
				}
			}
			TIMECHECK_END
		}
	}
}


void CMyIPTool2Doc::OnHoughTransform()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CHoughLineDlg dlg; 
	dlg.m_pDoc = this;
	POSITION pos = GetFirstViewPosition();
	dlg.m_pView = (CMyIPTool2View*)GetNextView(pos);

	if (dlg.DoModal() == IDOK) {
		MyIPImage img;
		Mat canny_img, out_img;
		img = m_MyIPImage;

		//1. Canny Edge
		if (IpCanny(img, dlg.m_nLowTh, dlg.m_nHighTh)) {
			if (!IpImageToMat(img, canny_img))	return;
		}

		//2. Hough
		if (IpHoughLine(canny_img, out_img, dlg.m_dRho, dlg.m_dTheta, dlg.m_nHoughTh, dlg.m_nMaskHeight, dlg.m_nMaskWidth))
			AfxNewImage(_T("Hough Line"), out_img);
	}
	dlg.m_pView->Invalidate(false);
}



void CMyIPTool2Doc::OnContourTracing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CMyIPTool2Doc::OnContourtracingSingleobject()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Mat img, out;
	if (IpImageToMat(m_MyIPImage, img)) {
		TIMECHECK_START
			if (IpContourTracing(img, out)) {
				AfxNewImage(_T("Contour Tracing"), out);
			}
		TIMECHECK_END
	}
}


void CMyIPTool2Doc::OnContourtracingEuclidean()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	ContourThresoldDlg dlg;
	Mat img, out;
	if (dlg.DoModal() == IDOK)
	{
		if (IpImageToMat(m_MyIPImage, img)) {
			TIMECHECK_START
				if (IpContourTracingEuclideanFast(img, out, dlg.m_nContourThreshold)) {
					AfxNewImage(_T("Contour Tracing"), out);
				}
			TIMECHECK_END
		}
	}
}


void CMyIPTool2Doc::OnGeometrictransformationsTranslation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	TranslationDlg dlg;
	MyIPImage img, dst;
	img = m_MyIPImage;
	if (dlg.DoModal() == IDOK)
	{
		TIMECHECK_START
		if (dlg.m_bRoiMode == false && IpTranslate(img, dst, dlg.m_nShiftX, dlg.m_nShiftY)) {
			CString strTitle;
			strTitle.Format(_T("Translation(%d,%d)"), dlg.m_nShiftX, dlg.m_nShiftY);
			AfxNewImage(strTitle, dst);
		}
		else if (dlg.m_bRoiMode == true && IpTranslateROI(img, dst, dlg.m_nShiftX, dlg.m_nShiftY, dlg.m_nRoiX, dlg.m_nRoiY, dlg.m_nRoiW, dlg.m_nRoiH)) {
			CString strTitle;
			strTitle.Format(_T("TranslationROI(%d,%d,%d,%d,%d,%d)"), dlg.m_nShiftX, dlg.m_nShiftY, dlg.m_nRoiX, dlg.m_nRoiY, dlg.m_nRoiW, dlg.m_nRoiH);
			AfxNewImage(strTitle, dst);
		}
		TIMECHECK_END
	}
}


void CMyIPTool2Doc::OnUserfeaturePeriodictranslation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	ContourThresoldDlg dlg;
	Mat img, out;
	if (dlg.DoModal() == IDOK)
	{
		if (IpImageToMat(m_MyIPImage, img)) {
			TIMECHECK_START
				if (IpContourTracingEuclideanFast(img, out, dlg.m_nContourThreshold)) {
					AfxNewImage(_T("Contour Tracing"), out);
				}
			TIMECHECK_END
		}
	}
}
