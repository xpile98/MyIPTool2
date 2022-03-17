#pragma once
#include "MyIPTool2.h"	//afxprintinfo
#include "MyIPImage.h"

bool IpEdgeDetector(MyIPImage &img, MyIPImage &angleimg, int ntype, BOOL bnormalize);
bool IpLaplacian(MyIPImage &img, int ntype, BOOL bnormalize);
bool IpCanny(MyIPImage &img, int lowth, int highth);
bool IpLoG(MyIPImage &img, double sigma);
bool IpDoG(MyIPImage &img, double sigma1, double sigma2);