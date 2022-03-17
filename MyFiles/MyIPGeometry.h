#pragma once
#include "MyIPTool2.h"	//afxprintinfo
#include "MyIPImage.h"

bool IpTranslate(MyIPImage & img, MyIPImage &dst, int sx, int sy);
bool IpTranslateROI(MyIPImage & img, MyIPImage &dst, int sx, int sy, int rx, int ry, int rw, int rh);