#pragma once

#include "MyFunction.h"	//scalar color 

#include "MyIPImage.h"
#include "MyIPTool2.h"	//afxprintinfo
#include "MyIPTool2Doc.h"
#include "math.h"

/*----------���� ����----------*/
bool IP_Inverse(MyIPImage &img);
bool IP_Inverse(Mat &mat);	//�����ʿ�
//bool IP_Inverse(const MyIPImage img, MyIPImage &outimg);	//(�纻, ����) 


/*----------���/��� ����----------*/
bool IP_BrightnessContrast(MyIPImage &img, int offset, float gain);
bool IP_BrightnessContrast(Mat &mat, int offset, float gain);	//�����ʿ�


/*----------���� ����----------*/
bool IP_GammaCorrection(MyIPImage &img, float gamma);
bool IP_GammaCorrection(Mat &mat, float gamma);	//�����ʿ�


/*----------������׷� ���ϴ� �Լ�----------*/
bool IP_Histogram(MyIPImage &img, float *histo);
bool IP_Histogram(Mat &mat, Mat& himg);
bool IP_Histogram(MyIPImage &img, unsigned int *histo);	//��� ����ȭ ���ϴ¾���
	/*������׷� ��Ʈ��Ī*/
bool IP_HistogramStretching(MyIPImage &img);
	/*������׷� �յ�ȭ*/
bool IP_HistogramEqualization(MyIPImage &img);
	/*��� �� ǥ������ ���ϱ�*/
bool IP_Mean_StandardDeviation(MyIPImage &img);
	


/*----------������ ��� �� ������----------*/
bool IP_ARLogic(MyIPImage& img1, MyIPImage& img2, MyIPImage& oimg, int op, bool useConstant=false, int Constant=0, double dConst=0.0);
bool IP_ARLogic(Mat& img1, Mat& img2, Mat& oimg, int op, bool useConstant = false, int Constant = 0);


/*----------����ȭ----------*/
bool IP_Threshold(MyIPImage &img, int threshold);


/*----------��Ʈ ��� ����----------*/
bool IP_Bitplane(MyIPImage &img1, MyIPImage &img2, int bit);