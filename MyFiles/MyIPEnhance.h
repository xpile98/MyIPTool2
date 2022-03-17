#pragma once

#include "MyFunction.h"	//scalar color 

#include "MyIPImage.h"
#include "MyIPTool2.h"	//afxprintinfo
#include "MyIPTool2Doc.h"
#include "math.h"

/*----------영상 반전----------*/
bool IP_Inverse(MyIPImage &img);
bool IP_Inverse(Mat &mat);	//검증필요
//bool IP_Inverse(const MyIPImage img, MyIPImage &outimg);	//(사본, 원본) 


/*----------밝기/명암 조절----------*/
bool IP_BrightnessContrast(MyIPImage &img, int offset, float gain);
bool IP_BrightnessContrast(Mat &mat, int offset, float gain);	//검증필요


/*----------감마 보정----------*/
bool IP_GammaCorrection(MyIPImage &img, float gamma);
bool IP_GammaCorrection(Mat &mat, float gamma);	//검증필요


/*----------히스토그램 구하는 함수----------*/
bool IP_Histogram(MyIPImage &img, float *histo);
bool IP_Histogram(Mat &mat, Mat& himg);
bool IP_Histogram(MyIPImage &img, unsigned int *histo);	//얘는 정규화 안하는아이
	/*히스토그램 스트레칭*/
bool IP_HistogramStretching(MyIPImage &img);
	/*히스토그램 균등화*/
bool IP_HistogramEqualization(MyIPImage &img);
	/*평균 및 표준편차 구하기*/
bool IP_Mean_StandardDeviation(MyIPImage &img);
	


/*----------영상의 산술 및 논리연산----------*/
bool IP_ARLogic(MyIPImage& img1, MyIPImage& img2, MyIPImage& oimg, int op, bool useConstant=false, int Constant=0, double dConst=0.0);
bool IP_ARLogic(Mat& img1, Mat& img2, Mat& oimg, int op, bool useConstant = false, int Constant = 0);


/*----------이진화----------*/
bool IP_Threshold(MyIPImage &img, int threshold);


/*----------비트 평면 분할----------*/
bool IP_Bitplane(MyIPImage &img1, MyIPImage &img2, int bit);