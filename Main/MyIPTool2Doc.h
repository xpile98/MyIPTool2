
// MyIPTool2Doc.h : CMyIPTool2Doc 클래스의 인터페이스
//


#pragma once
//#include <opencv2/opencv.hpp>
#include "MyIPImage.h"
#include "HistogramDlg.h"
class CMyIPTool2Doc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CMyIPTool2Doc();
	DECLARE_DYNCREATE(CMyIPTool2Doc)

// 특성입니다.
public:
	CHistogramDlg *m_pHistogramDlg; 
	Mat m_Mat;
	MyIPImage m_MyIPImage;
	MyIPImage m_MyIPImage_org;
	CString m_Title;
	double m_dZoom;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CMyIPTool2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:

	/*			 함수명						//   기능		//	 방식		// Depth, Channel 대응					*/		
	afx_msg void OnInverse();				//영상 반전		// MyIPImage	// 8UC1,3 대응, 16UC1,3 구현,테스트X
	afx_msg void OnBrightnessContrast();	//밝기,대비조절	// MyIPImage	// 8UC1,3 대응, 16UC1,3 구현,테스트X
	afx_msg void OnGamma();					//감마 조절		// MyIPImage	// 8UC1,3 대응, 16UC1,3 구현,테스트X
	afx_msg void OnViewHistogram();			//Histogram 출력// MyIPImage	// 8UC1 대응, 16UC1 테스트X 
	afx_msg void OnHistoStretching();		//Histo 스트레칭// MyIPImage	// 8UC1 대응, 16UC1 테스트X 
	afx_msg void OnHistoEqualization();		//Histo 균등화	// MyIPImage	// 8UC1 대응, 16UC1 테스트X

	/*-MyIPImage Color Conversion-*/
	afx_msg void OnRgbtogray();				//RGB > GRAY	// MyIPImage	// 8UC3 대응, 16U3 미구현
	afx_msg void OnColorSplitRGB();			//IMG > RGB		// MyIPImage	// 8UC3 대응, 16U3 미구현
	afx_msg void OnColorSplitHSI();			//IMG > HSI		// MyIPImage	// 8UC3 대응, 16U3 미구현
	afx_msg void OnColorSplitYuv();			//IMG > YUV		// MyIPImage	// 8UC3 대응, 16U3 미구현
	afx_msg void OnColorCombineRgb();		//RGB > IMG		// MyIPImage	// 8UC1 대응, 16U1 미구현
	afx_msg void OnColorCombineHsi();		//HSI > IMG		// MyIPImage	// 8UC1 대응, 16U1 미구현
	afx_msg void OnColorCombineYuv();		//YUV > IMG		// MyIPImage	// 8UC1 대응, 16U1 미구현

	/*-Open CV Color Conversion-*/
	afx_msg void OnOpencvRgbtogray();		//정의되어있는데 ifdef~ else ~ endif 써서 인식을 못하는듯.. 구현엔 이상 X
	afx_msg void OnOpencvColorSplitRgb();
	afx_msg void OnOpencvColorSplitHsi();
	afx_msg void OnOpencvColorSplitYuv();
	afx_msg void OnOpencvColorSplitHsv();
	afx_msg void OnOpencvColorSplitYcrcb();
	afx_msg void OnOpencvColorSplitLab();
	afx_msg void OnOpencvColorSplitCmy();
	afx_msg void OnOpencvColorSplitCmyk();
	afx_msg void OnOpencvHistogram();


	afx_msg void OnArithmeticlogical();
	afx_msg void OnCvArithmeticlogical();
	afx_msg void OnThreshold();
	afx_msg void OnBitplane();
	afx_msg void OnAutomaticthresholdIterative();
	afx_msg void OnAutomaticthresholdOtsu();
	afx_msg void OnAutomaticthresholdEntropy();
	afx_msg void OnCvAutomaticthreshold();
	afx_msg void OnCvHistoEqualization();
	afx_msg void OnConvert8bitto16bit();

	//필터
	afx_msg void OnSpatialfilterAverage();
	afx_msg void OnSpatialfilterGaussian();
	afx_msg void OnSpatialfilterMaximum();
	afx_msg void OnSpatialfilterMinumum();
	//필터 cv
	afx_msg void OnCvSpatialfilterAverage();
	afx_msg void OnCvSpatialfilterGaussian();
	afx_msg void OnCvSpatialfilterMaximum();
	afx_msg void OnCvSpatialfilterMinumum();
	afx_msg void OnCvSpatialfilterMedian();

	//	afx_msg void OnAddnoiseGaussian();		//>> Addnoise로 통합
	//	afx_msg void OnAddnoiseSaltpepper();
	afx_msg void OnMyipimageAddnoise();
	afx_msg void OnSpatialfilterAdaptiveMedian();
	afx_msg void OnSpatialfilterMedian();
	afx_msg void OnSharpeningHighboostfilter();
	afx_msg void OnSharpeningLaplaciansharpening();
	afx_msg void OnCvSpatialfilterAdaptiveMedian();
	afx_msg void OnSpatialfilterEdgedetector();
	afx_msg void OnMorphology();
	afx_msg void OnGrayscaleMorphology();
	afx_msg void OnCvMorphology();
	afx_msg void OnMorphologyTest();
	afx_msg void OnWebcamLivecam();

	//주파수 영역
	afx_msg void OnFourierTransform();
	afx_msg void OnFreqFilter();
	afx_msg void OnDftfilter2dAverage();
	afx_msg void OnDftfilter2dGaussian();
	afx_msg void OnObjectdetectionPatternmatching();
	afx_msg void OnHoughTransform();
	afx_msg void OnUpdateOpencvRgbtogray(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRgbtogray(CCmdUI *pCmdUI);
	afx_msg void OnContourTracing();
	afx_msg void OnContourtracingEuclidean();
	afx_msg void OnContourtracingSingleobject();
	afx_msg void OnGeometrictransformationsTranslation();
	afx_msg void OnUserfeaturePeriodictranslation();
	afx_msg void OnZoomX1();
	afx_msg void OnZoomX2();
	afx_msg void OnZoomX3();
	afx_msg void OnZoomX4();
};
