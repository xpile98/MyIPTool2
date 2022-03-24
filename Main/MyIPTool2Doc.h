
// MyIPTool2Doc.h : CMyIPTool2Doc Ŭ������ �������̽�
//


#pragma once
//#include <opencv2/opencv.hpp>
#include "MyIPImage.h"
#include "HistogramDlg.h"
class CMyIPTool2Doc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CMyIPTool2Doc();
	DECLARE_DYNCREATE(CMyIPTool2Doc)

// Ư���Դϴ�.
public:
	CHistogramDlg *m_pHistogramDlg; 
	Mat m_Mat;
	MyIPImage m_MyIPImage;
	MyIPImage m_MyIPImage_org;
	CString m_Title;
	double m_dZoom;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// �����Դϴ�.
public:
	virtual ~CMyIPTool2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:

	/*			 �Լ���						//   ���		//	 ���		// Depth, Channel ����					*/		
	afx_msg void OnInverse();				//���� ����		// MyIPImage	// 8UC1,3 ����, 16UC1,3 ����,�׽�ƮX
	afx_msg void OnBrightnessContrast();	//���,�������	// MyIPImage	// 8UC1,3 ����, 16UC1,3 ����,�׽�ƮX
	afx_msg void OnGamma();					//���� ����		// MyIPImage	// 8UC1,3 ����, 16UC1,3 ����,�׽�ƮX
	afx_msg void OnViewHistogram();			//Histogram ���// MyIPImage	// 8UC1 ����, 16UC1 �׽�ƮX 
	afx_msg void OnHistoStretching();		//Histo ��Ʈ��Ī// MyIPImage	// 8UC1 ����, 16UC1 �׽�ƮX 
	afx_msg void OnHistoEqualization();		//Histo �յ�ȭ	// MyIPImage	// 8UC1 ����, 16UC1 �׽�ƮX

	/*-MyIPImage Color Conversion-*/
	afx_msg void OnRgbtogray();				//RGB > GRAY	// MyIPImage	// 8UC3 ����, 16U3 �̱���
	afx_msg void OnColorSplitRGB();			//IMG > RGB		// MyIPImage	// 8UC3 ����, 16U3 �̱���
	afx_msg void OnColorSplitHSI();			//IMG > HSI		// MyIPImage	// 8UC3 ����, 16U3 �̱���
	afx_msg void OnColorSplitYuv();			//IMG > YUV		// MyIPImage	// 8UC3 ����, 16U3 �̱���
	afx_msg void OnColorCombineRgb();		//RGB > IMG		// MyIPImage	// 8UC1 ����, 16U1 �̱���
	afx_msg void OnColorCombineHsi();		//HSI > IMG		// MyIPImage	// 8UC1 ����, 16U1 �̱���
	afx_msg void OnColorCombineYuv();		//YUV > IMG		// MyIPImage	// 8UC1 ����, 16U1 �̱���

	/*-Open CV Color Conversion-*/
	afx_msg void OnOpencvRgbtogray();		//���ǵǾ��ִµ� ifdef~ else ~ endif �Ἥ �ν��� ���ϴµ�.. ������ �̻� X
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

	//����
	afx_msg void OnSpatialfilterAverage();
	afx_msg void OnSpatialfilterGaussian();
	afx_msg void OnSpatialfilterMaximum();
	afx_msg void OnSpatialfilterMinumum();
	//���� cv
	afx_msg void OnCvSpatialfilterAverage();
	afx_msg void OnCvSpatialfilterGaussian();
	afx_msg void OnCvSpatialfilterMaximum();
	afx_msg void OnCvSpatialfilterMinumum();
	afx_msg void OnCvSpatialfilterMedian();

	//	afx_msg void OnAddnoiseGaussian();		//>> Addnoise�� ����
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

	//���ļ� ����
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
