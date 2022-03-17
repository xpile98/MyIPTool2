#pragma once
#include "MyIPTool2.h"
#include "MyIPImage.h"

//smoothing 
bool IpFilter2D(MyIPImage &img, int h, int w, double **kernel, int borderType, int value = 0);	//8bit 1,3	//16bit 1
bool IpFilterMax(MyIPImage &img, int h, int w, int borderType, int value = 0);					//8bit 1,3	//16bit 1
bool IpFilterMin(MyIPImage &img, int h, int w, int borderType, int value = 0);					//8bit 1,3	//16bit 1
bool IpFilterMed(MyIPImage &img, int h, int w);													//8bit 1,3	//16bit 1
bool IpFilterMed(Mat &img, int h, int w);
bool IpFilterAdpMed(MyIPImage &img, int maxsize);
bool IpFilterAdpMed(Mat &img, int maxsize);

bool IpFillBorder(MyIPImage &img, MyIPImage &oimg, int h, int w, int borderType, int value=0);
bool IpGaussianKernel(double **kernel, int h, int w, double sigma);
bool IpLoGaussianKernel(double **kernel, int h, int w, double sigma);

bool IpAddGaussianNoise(MyIPImage &img, double sigma);											//8bit 1,3	//16bit 1
bool IpAddSaltPepperNoise(MyIPImage &img, double ps, double pp, BOOL rgb = FALSE);								//8bit 1,3	//16bit 1
double GaussianRand(double mean, double sigma);

//sharpening
bool IpFilterHighboost(MyIPImage &img, double sigma, double k);