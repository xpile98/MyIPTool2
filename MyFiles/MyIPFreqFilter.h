#pragma once
#include "MyIPTool2.h"
#include "MyIPImage.h"

bool IpFFT2D(Mat &src, Mat  &dst);
bool IpIFFT2D(Mat &src, Mat &ref, bool DoScale = false);
bool IpFFTSpectrum(Mat &src, Mat &dst);
bool IpFreqFilter(Mat &src, Mat &filter, BOOL scale, BOOL average);
bool IpFilter2D_DFT(Mat &img, Mat &dst, int h, int w, double ** kernel);


Mat IpZeroPadding(Mat &src);
Mat IpMakeFilter(Mat & src, int filterType, int filterShape, int cofVal, int bpfVal, int butVal);