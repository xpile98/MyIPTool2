#pragma once
#include "MyIPTool2.h"
#include "MyIPImage.h"

bool IpPatternMatching(Mat &img, Mat &pattern, Mat &out, int nObjects, float fNCCThreshold, int match_method = 5);
bool IpPatternMatching_Thresold(Mat &img, Mat &pattern, Mat &out, float fNCCThreshold, int match_method = 5);	//fncc value보다 크면 계속 찾는다. 
bool IpHoughLine(Mat &img, Mat &dst, double rho, double theta, int threshold, int mask_height, int mask_width);
bool IpContourTracing(Mat &img, Mat &pattern);
bool IpContourTracingOutline(Mat &img, Mat &pattern);
bool IpContourTracingEuclidean(Mat &img, Mat &pattern);
bool IpContourTracingEuclideanFast(Mat &img, Mat &pattern, int threshold);
//bool IpContourTracingEuclideanFastwithData(Mat &img, Mat &pattern, int threshold, vector<strContourInfo> &info);
