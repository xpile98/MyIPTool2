#pragma once

#include "MyIPTool2.h"	//afxprintinfo
#include "MyIPImage.h"
#include "MyIPEnhance.h"

bool IP_AutoThreshold_Iterative(MyIPImage &img, int *pnThreshold);
bool IP_AutoThreshold_Otsu(MyIPImage &img, int *pnThreshold);
bool IP_AutoThreshold_Entropy(MyIPImage &img, int *pnThreshold);
bool IP_AutoThreshold_OpenCV(Mat &img, int type, int method);