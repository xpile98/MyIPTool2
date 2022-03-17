
// MyIPTool2Doc.cpp : CMyIPTool2Doc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "MyIPTool2.h"
#endif

#include "MyIPTool2Doc.h"
#include "MyIPImage.h"
#include "MyIPConvert.h"
#include "MyIPEnhance.h"
#include "MyIPAutoThreshold.h"		//190328 YWCho Auto Threshold Header �߰� 
#include "MyIPSpetialFilter.h"		//190408 YWCho Spetial Filter Header �߰�
#include "MyIPEdge.h"				//190418 YWCho Edge Detectir Header �߰� 
#include "MyIPMorphology.h"			//190425 YWCho Morphology Header �߰� 
#include "MyIPFreqFilter.h"			//190509 YWCho Frequency Filter Header �߰�
#include "MyIPObjectDetection.h"	//190513 YWCho ObjectDetection Header �߰� 
#include "MyIPGeometry.h"			//220310 YWCho Geometric transformation Header �߰� 

#include "BrightnessContrastDlg.h"
#include "GammaDlg.h"
#include "HistogramDlg.h"
#include "ColorCombineDlg.h"		//190313 YWCho ���� ��ġ�� ���̾�α� �߰�
#include "ArithmeticLogicalDlg.h"	//190319 YWCho ��� �� ������ ���̾�α� �߰�
#include "ThresholdDlg.h"			//190325 YWCho ����ȭ ���̾�α� �߰� 
#include "CVThresholdDlg.h"			//190402 YWCho CV Thresholding Dlg �߰�
#include "SpetialFilterDlg.h"		//190408 YWCho ������Ʈ Dlg�߰� 
#include "NoiseDlg.h"				//190412 YWCho ������ Dlg �߰�
#include "ShapeningFilterDlg.h"		//190416 YWCho ������ Dlg �߰� 
#include "LineProfileDlg.h"			//190416 YWCho Line Profile Dlg �߰�(�׽�Ʈ)
#include "EdgeDetectorDlg.h"		//190418 YWCho �������� Dlg �߰� 
#include "MorphologyDlg.h"			//190426 YWCho �������� Dlg �߰�
#include "LoopDlg.h"				//190501 YWCho �������� �׽�Ʈ Dlg �߰�
#include "FrequencyFilterDlg.h"		//190509 YWCho ���ļ� ���� Dlg �߰� 
#include "PatternMatchingDlg.h"		//190513 YWCho ���ϸ�Ī Dlg �߰� 
#include "HoughLineDlg.h"			//190515 YWCho ������ȯ Dlg �߰�
#include "ContourThresoldDlg.h"		//220307 YWCho ������ ��Ŭ���ȰŸ� ��� Dlg �߰�
#include "TranslationDlg.h"			//220310 YWCho ���� �����̵� Dlg �߰�

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
// 	ON_COMMAND(ID_ADDNOISE_GAUSSIAN, &CMyIPTool2Doc::OnAddnoiseGaussian)		//>>Addnoise�� ����
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


// CMyIPTool2Doc ����/�Ҹ�

CMyIPTool2Doc::CMyIPTool2Doc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.
	m_Title = _T("");
	srand((unsigned)(time(NULL)));		//srand �μ��� �ð����� �ش�.
}

CMyIPTool2Doc::~CMyIPTool2Doc()
{
}

BOOL CMyIPTool2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	//AfxPrintInfo(_T("CMyIPTool2Doc::OnNewDocument()"));  190313 YWCho �ʹ� ���� ��µȴ�~ �ּ�~

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
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}

#ifdef SHARED_HANDLERS

// ����� �׸��� �����մϴ�.
void CMyIPTool2Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// ������ �����͸� �׸����� �� �ڵ带 �����Ͻʽÿ�.
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

// �˻� ó���⸦ �����մϴ�.
void CMyIPTool2Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
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

// CMyIPTool2Doc ����

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


// CMyIPTool2Doc ���


BOOL CMyIPTool2Doc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

	AfxPrintInfo(_T("CMyIPTool2Doc::OnOpenDocument"));
	AfxPrintInfo(lpszPathName);
	
	//CImage::Load�� �о����
// 	if (!m_MyIPImage.Load(lpszPathName))	//load�� 8��Ʈ1ch, 8��Ʈ3ch������ �� �дµ� �� �ܸ� �� ������..
// 		return false;						//16��Ʈ ��� �̹����� �÷��� �а��� 

	//Mat�� �о����
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



	//CImage�� �о����
// 	m_MyIPImage.Destroy();
// 	m_MyIPImage.Load(lpszPathName);
// 
// 	m_MyIPImage.SetDepth(m_Mat.depth());
// 	m_MyIPImage.SetChannels(m_Mat.channels());
	

	return TRUE;

}


BOOL CMyIPTool2Doc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (!IpImageToMat(m_MyIPImage, m_Mat)) return false;
	string filename = CT2A(lpszPathName);
	return imwrite(filename, m_Mat);

	//return CDocument::OnSaveDocument(lpszPathName);
}



void CMyIPTool2Doc::OnInverse()
{
	AfxPrintInfo(_T("CMyIPTool2Doc::OnInverse"));
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

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

	/*--------------���� 1------------------*/
	/*IP_Inverse(Mat) ���α׷��� �ϰ� mat�� �о� ��ȯ�ϰ� imshow�� ����ϱ�*/
// 	if (IP_Inverse(m_Mat)) {
// 		imshow("IP_Inverse(Mat)", m_Mat);
// 	}

	/*--------------���� 2------------------*/  //�ȵ� 
	/*IP_Inverse(MyIPImage)����ؼ� img.draw�� ����ϱ�*/

// 	MyIPImage img;
// 	img.CreateImage(m_MyIPImage.GetHeight(), m_MyIPImage.GetWidth(), m_MyIPImage.GetDepth(), m_MyIPImage.GetChannel());
// 	if (IP_Inverse(img)) {
// 		AfxNewImage(img);
// 	}
}

void CMyIPTool2Doc::OnBrightnessContrast()
{
	AfxPrintInfo(_T("CMyIPTool2Doc::OnBrightnessContrast()"));
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CBrightnessContrastDlg dlg;
	dlg.m_pDoc = this;
	POSITION pos = GetFirstViewPosition();
	dlg.m_pView = (CMyIPTool2View*)GetNextView(pos);


	/*-----------IP_BrightnessContrast(MyIPImage)-------------------------*/
#ifdef USE_IPIMAGE
	MyIPImage imgcopy;
	imgcopy = m_MyIPImage;
	if (imgcopy.GetDepth() == CV_16U || imgcopy.GetDepth() == CV_16S)	//16��Ʈ ����
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	/*----------------���̾�α� Ŭ���� �������� �� �ɹ����� ����------------------*/
	CGammaDlg dlg;											//Gamma ���̾�α� ����� ���� ������� �߰� ��

	dlg.m_pDoc = this;										//���̾�α��� m_pDoc������ ������ Doc.cpp�� �ּҸ� ����
	POSITION pos = GetFirstViewPosition();					//��ť��Ʈ Ŭ�������� �� Ŭ������ �ν��Ͻ� �����͸� �������� 
	dlg.m_pView = (CMyIPTool2View*)GetNextView(pos);		//GetFirstViewPosition() �Լ��� GetNextView() �Լ��� �����Ͽ� ����ؾ� �Ѵ�. 
															//�̷��� ���������� ������ ��ť��Ʈ �ϳ��� �������� �䰡 ����� �� �ֱ� �����̴�.


	/*-----------IP_GammaCorrection(MyIPImage)------------------------------------*/
#ifdef USE_IPIMAGE
	MyIPImage imgcopy;
	imgcopy = m_MyIPImage;
	if (imgcopy.GetDepth() == CV_16U || imgcopy.GetDepth() == CV_16S)	//16��Ʈ ����
		dlg.bIs16Bit = true;
	if (dlg.DoModal() == IDOK) {						//���� ���̾�α׿��� Ȯ���� ������
		if (IP_GammaCorrection(imgcopy, dlg.m_fGamma)) {	//���� ���������� �� ������ 
			AfxNewImage(_T("Gamma Correction"), imgcopy);	//AfxNewImage�Լ��� img�� �����Ѵ�.
			AfxPrintInfo(_T("[COMPLETE] Gamma Control"));
		}
		else
			AfxPrintInfo(_T("[FAIL] Gamma Control"));
	}
	dlg.m_pView->Invalidate(false);	//�̰� �־��ָ� ��� �ص� �״�� ���� 
#else
	MyIPImage img;
	if (IpMatToImage(m_Mat, img)) {							//Mat�� MyIPImage�� ��ȯ�� �����ϸ�
		if (dlg.DoModal() == IDOK) {						//���� ���̾�α׿��� Ȯ���� ������
			if (IP_GammaCorrection(img, dlg.m_fGamma)) {	//���� ���������� �� ������ 
				AfxNewImage(_T("Gamma Correction"), img);	//AfxNewImage�Լ��� img�� �����Ѵ�.
				AfxPrintInfo(_T("[COMPLETE] Gamma Control"));
			}
			else
				AfxPrintInfo(_T("[FAIL] Gamma Control"));
		}
		dlg.m_pView->Invalidate(false);	//�̰� �־��ָ� ��� �ص� �״�� ���� 
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
// 	dlg.m_pView->Invalidate(false);	//�̰� �־��ָ� ��� �ص� �״�� ���� 

} 

void CMyIPTool2Doc::OnViewHistogram()
{
	AfxPrintInfo(_T("CMyIPTool2Doc::OnViewHistogram()"));
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.



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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnHistoStretching()"));

	/*----------- OnHistoStretching(MyIPImage) -------------------*/
#ifdef USE_IPIMAGE
	MyIPImage imgcopy;
	imgcopy = m_MyIPImage;
	if (IP_HistogramStretching(imgcopy)) {
		AfxNewImage(_T("������׷� ��Ʈ��Ī"), imgcopy);
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnHistoEqualization()"));

	/*----------- OnHistoStretching(MyIPImage) -------------------*/
#ifdef USE_IPIMAGE
	MyIPImage imgcopy;
	imgcopy = m_MyIPImage;
	if (IP_HistogramEqualization(imgcopy)) {
		AfxNewImage(_T("������׷� �յ�ȭ"),imgcopy);
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	pCmdUI->Enable(m_MyIPImage.GetChannels() == 3);
}

void CMyIPTool2Doc::OnColorSplitRGB()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnColorCombineRgb()"));
	CColorCombineDlg dlg;
	dlg.m_strColorSpace = _T("RGB ���� ��� ��ġ��");
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
			AfxMessageBox(_T("������ Size, Channel, Depth ����"));
			return;
		}

		AfxPrintInfo(_T("[���� ��� ��ġ��/RGB] �Է¿���: R: %s, G: %s, B: %s"),
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
			AfxMessageBox(_T("������ Size, Channel, Depth ����"));
			return;
		}

		AfxPrintInfo(_T("[���� ��� ��ġ��/RGB] �Է¿���: R: %s, G: %s, B: %s"),
			pDoc1->GetTitle(), pDoc2->GetTitle(), pDoc3->GetTitle());
		AfxNewImage(_T("RGB Image"), imgColor);
#endif
	}
}

void CMyIPTool2Doc::OnColorCombineHsi()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnColorCombineHsi()"));
	CColorCombineDlg dlg;
	dlg.m_strColorSpace = _T("HSI ���� ��� ��ġ��");
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
			AfxMessageBox(_T("������ Size, Channel, Depth ����"));
			return;
		}

		AfxPrintInfo(_T("[���� ��� ��ġ��/HSI] �Է¿���: H: %s, S: %s, I: %s"),
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
			AfxMessageBox(_T("������ Size, Channel, Depth ����"));
			return;
		}

		AfxPrintInfo(_T("[���� ��� ��ġ��/HSI] �Է¿���: H: %s, S: %s, I: %s"),
			pDoc1->GetTitle(), pDoc2->GetTitle(), pDoc3->GetTitle());
		AfxNewImage(_T("HSI Image"),imgColor);
#endif
	}
}

void CMyIPTool2Doc::OnColorCombineYuv()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnColorCombineYuv()"));
	CColorCombineDlg dlg;
	dlg.m_strColorSpace = _T("YUV ���� ��� ��ġ��");
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
			AfxMessageBox(_T("������ Size, Channel, Depth ����"));
			return;
		}

		AfxPrintInfo(_T("[���� ��� ��ġ��/YUV] �Է¿���: Y: %s, U: %s, V: %s"),
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
			AfxMessageBox(_T("������ Size, Channel, Depth ����"));
			return;
		}

		AfxPrintInfo(_T("[���� ��� ��ġ��/YUV] �Է¿���: Y: %s, U: %s, V: %s"),
			pDoc1->GetTitle(), pDoc2->GetTitle(), pDoc3->GetTitle());
		AfxNewImage(_T("YUV Image"), imgColor);
#endif
	}
}



/*--------------------Open CV Color Conversion-----------------------*/
void CMyIPTool2Doc::OnOpencvRgbtogray()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
}


void CMyIPTool2Doc::OnOpencvColorSplitRgb()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CArithmeticLogicalDlg dlg;
	if (dlg.DoModal() == IDOK) {

		//>> ������ �̸� ����
		TCHAR* op[] = { _T("����"),_T("����"),_T("���"),_T("����"),_T("����"),_T("����"),_T("�ּҰ�"),_T("�ִ밪"), _T("��Ÿ�� ����") };
		AfxPrintInfo(_T("��� �� �� ����: %s      �Է¿���#1: %s      �Է¿���#2: %s"),
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CArithmeticLogicalDlg dlg;
	if (dlg.DoModal() == IDOK) {

		//>> ������ �̸� ����
		TCHAR* op[] = { _T("����"),_T("����"),_T("���"),_T("����"),_T("����"),_T("����"),_T("�ּҰ�"),_T("�ִ밪"), _T("��Ÿ�� ����") };
		AfxPrintInfo(_T("��� �� �� ����: %s      �Է¿���#1: %s      �Է¿���#2: %s"),
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	/*----------------���̾�α� Ŭ���� �������� �� �ɹ����� ����------------------*/
	CThresholdDlg dlg;											//Gamma ���̾�α� ����� ���� ������� �߰� ��

	dlg.m_pDoc = this;										//���̾�α��� m_pDoc������ ������ Doc.cpp�� �ּҸ� ����
	POSITION pos = GetFirstViewPosition();					//��ť��Ʈ Ŭ�������� �� Ŭ������ �ν��Ͻ� �����͸� �������� 
	dlg.m_pView = (CMyIPTool2View*)GetNextView(pos);		//GetFirstViewPosition() �Լ��� GetNextView() �Լ��� �����Ͽ� ����ؾ� �Ѵ�. 
															//�̷��� ���������� ������ ��ť��Ʈ �ϳ��� �������� �䰡 ����� �� �ֱ� �����̴�.

	MyIPImage imgcopy;
	imgcopy = m_MyIPImage;
	dlg.SetImage(imgcopy);
	if (dlg.DoModal() == IDOK) {						//���� ���̾�α׿��� Ȯ���� ������
		if (IP_Threshold(imgcopy, dlg.m_nThreshold)) {	//���� ���������� �� ������ 
			AfxNewImage(_T("Threshold"), imgcopy);	//AfxNewImage�Լ��� img�� �����Ѵ�.
			AfxPrintInfo(_T("[COMPLETE] Threshold"));
		}
		else
			AfxPrintInfo(_T("[FAIL] Threshold"));
	}
	dlg.m_pView->Invalidate(false);	//�̰� �־��ָ� ��� �ص� �״�� ���� 
}

void CMyIPTool2Doc::OnBitplane()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnBitplane()"));

	CString title;
	MyIPImage imgcopy;
	imgcopy = m_MyIPImage;
	for (int bit = 0; bit < 8; bit++) {
		MyIPImage out;
		if (IP_Bitplane(imgcopy, out, bit)) {
			title.Format(L"BIT: %d", bit);
			AfxNewImage(title, out);	//AfxNewImage�Լ��� img�� �����Ѵ�.
		}
	}

}

void CMyIPTool2Doc::OnAutomaticthresholdIterative()
{
	AfxPrintInfo(_T("CMyIPTool2Doc::OnAutomaticthresholdIterative()"));
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	MyIPImage imgcopy;
	imgcopy = m_MyIPImage;
	int threshold;

	//>> Doc, View �ּ� �������ֱ� (������ getpixel()���� ����)
	CThresholdDlg dlg;
	dlg.m_pDoc = this;										//���̾�α��� m_pDoc������ ������ Doc.cpp�� �ּҸ� ����
	POSITION pos = GetFirstViewPosition();					//��ť��Ʈ Ŭ�������� �� Ŭ������ �ν��Ͻ� �����͸� �������� 
	dlg.m_pView = (CMyIPTool2View*)GetNextView(pos);		//GetFirstViewPosition() �Լ��� GetNextView() �Լ��� �����Ͽ� ����ؾ� �Ѵ�. 
															//�̷��� ���������� ������ ��ť��Ʈ �ϳ��� �������� �䰡 ����� �� �ֱ� �����̴�.
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
		dlg.m_pView->Invalidate(false);	//�̰� �־��ָ� ��� �ص� �״�� ���� 
	}
}

void CMyIPTool2Doc::OnAutomaticthresholdOtsu()
{
	AfxPrintInfo(_T("CMyIPTool2Doc::OnAutomaticthresholdOtsu()"));
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	MyIPImage imgcopy;
	imgcopy = m_MyIPImage;
	int threshold;

	//>> Doc, View �ּ� �������ֱ� (������ getpixel()���� ����)
	CThresholdDlg dlg;
	dlg.m_pDoc = this;										//���̾�α��� m_pDoc������ ������ Doc.cpp�� �ּҸ� ����
	POSITION pos = GetFirstViewPosition();					//��ť��Ʈ Ŭ�������� �� Ŭ������ �ν��Ͻ� �����͸� �������� 
	dlg.m_pView = (CMyIPTool2View*)GetNextView(pos);		//GetFirstViewPosition() �Լ��� GetNextView() �Լ��� �����Ͽ� ����ؾ� �Ѵ�. 
															//�̷��� ���������� ������ ��ť��Ʈ �ϳ��� �������� �䰡 ����� �� �ֱ� �����̴�.
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
		dlg.m_pView->Invalidate(false);	//�̰� �־��ָ� ��� �ص� �״�� ���� 
	}
}

void CMyIPTool2Doc::OnAutomaticthresholdEntropy()
{
	AfxPrintInfo(_T("CMyIPTool2Doc::OnAutomaticthresholdEntropy()"));
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	MyIPImage imgcopy;
	imgcopy = m_MyIPImage;
	int threshold;

	//>> Doc, View �ּ� �������ֱ� (������ getpixel()���� ����)
	CThresholdDlg dlg;
	dlg.m_pDoc = this;										//���̾�α��� m_pDoc������ ������ Doc.cpp�� �ּҸ� ����
	POSITION pos = GetFirstViewPosition();					//��ť��Ʈ Ŭ�������� �� Ŭ������ �ν��Ͻ� �����͸� �������� 
	dlg.m_pView = (CMyIPTool2View*)GetNextView(pos);		//GetFirstViewPosition() �Լ��� GetNextView() �Լ��� �����Ͽ� ����ؾ� �Ѵ�. 
															//�̷��� ���������� ������ ��ť��Ʈ �ϳ��� �������� �䰡 ����� �� �ֱ� �����̴�.
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
		dlg.m_pView->Invalidate(false);	//�̰� �־��ָ� ��� �ص� �״�� ���� 
	}
}

void CMyIPTool2Doc::OnCvAutomaticthreshold()
{	
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnCvAutomaticthreshold()"));

	int depth = m_MyIPImage.GetDepth();
	int channel = m_MyIPImage.GetChannels();
	if ((depth != CV_8U && depth != CV_8S) || channel != 1) {	//�̹����� 8��Ʈ�� �ƴ϶�� opencv thresholding �ȵȴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnCvHistoEqualization()"));

	int depth = m_MyIPImage.GetDepth();
	int channel = m_MyIPImage.GetChannels();
	if ((depth != CV_8U && depth != CV_8S) || channel != 1) {	//�̹����� 8��Ʈ�� �ƴ϶�� opencv thresholding �ȵȴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	AfxPrintInfo(_T("CMyIPTool2Doc::OnConvert8bitto16bit()"));
	MyIPImage img;
	if (IpConvert8to16(m_MyIPImage, img)) {
		AfxNewImage(img);
	}
}


void CMyIPTool2Doc::OnSpatialfilterAverage()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
			kernel[y] = buffer + y*w;		//double�̱⿡ 8*y*w��
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
			sigma = dlg.m_dSigmaVal;					//���߿� dlg���� �޾ƿ��Բ�
			h = (int)(n*sigma + 0.5) * 2 + 1;	//3 Sigma
			w = h;
		}

		double *buffer = new double[h*w];
		double **kernel = new double*[h];
		for (int y = 0; y < h; y++) {
			kernel[y] = buffer + y*w;		//double�̱⿡ 8*y*w��
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
			if (IpFilterMax(img, h, w, borderType, value)) {//���ϴ°� �ƴϴϱ� Ŀ���� �ʿ����!! 
				AfxNewImage(_T("MyIPImage Maximum"), img);
			}
		TIMECHECK_END
	}
}

void CMyIPTool2Doc::OnSpatialfilterMinumum()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
			if (IpFilterMin(img, h, w, borderType, value)) {//���ϴ°� �ƴϴϱ� Ŀ���� �ʿ����!! 
				AfxNewImage(_T("MyIPImage Minimum"), img);
			}
		TIMECHECK_END
	}
}


void CMyIPTool2Doc::OnSpatialfilterMedian()		
{
	AfxPrintInfo(_T("CMyIPTool2Doc::OnSpatialfilterMedian()"));
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
			if (IpFilterAdpMed(img, maxsize)) {//���ϴ°� �ƴϴϱ� Ŀ���� �ʿ����!! 
				AfxNewImage(_T("MyIPImage Adp Med"), img);
			}
		TIMECHECK_END
	}
}


void CMyIPTool2Doc::OnCvSpatialfilterAverage()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
						filter2D(img, out, -1, kernel, Point(-1, -1), 0.0, borderType);	//BORDER_WRAP ����X

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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
				sigma = dlg.m_dSigmaVal;					//���߿� dlg���� �޾ƿ��Բ�
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
// 	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
// 	//GaussianRand()�Լ� �̿��� ������ ���� ���캸�� 
// 	// 	for (int noise = 0; noise < 80; noise++) {
// 	// 		double v = GaussianRand(0., 3);
// 	// 		AfxPrintInfo(_T("%d: %f"), noise, v);
// 	// 	}
// }


//void CMyIPTool2Doc::OnAddnoiseSaltpepper()
//{
//	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
		if (!IpGaussianKernel(kernel, h, w, sigma)) 			//����þ� Ŀ�� ����
			return;
		if (!IpFilter2D(GAUimg, h, w, kernel, BORDER_REFLECT)) 	//����þ� ���� ����
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
			kernel[y] = new double[3];		//double�̱⿡ 8*y*w��

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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	POSITION pos = GetFirstViewPosition();					//��ť��Ʈ Ŭ�������� �� Ŭ������ �ν��Ͻ� �����͸� �������� 
	CMyIPTool2View *m_pView = (CMyIPTool2View*)GetNextView(pos);		//GetFirstViewPosition() �Լ��� GetNextView() �Լ��� �����Ͽ� ����ؾ� �Ѵ�. 
															//�̷��� ���������� ������ ��ť��Ʈ �ϳ��� �������� �䰡 ����� �� �ֱ� �����̴�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
			kernel[y] = buffer + y*w;		//double�̱⿡ 8*y*w��
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
			sigma = dlg.m_dSigmaVal;					//���߿� dlg���� �޾ƿ��Բ�
			h = (int)(n*sigma + 0.5) * 2 + 1;	//3 Sigma
			w = h;
		}

		double *buffer = new double[h*w];
		double **kernel = new double*[h];
		for (int y = 0; y < h; y++) {
			kernel[y] = buffer + y*w;		//double�̱⿡ 8*y*w��
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}


void CMyIPTool2Doc::OnContourtracingSingleobject()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
