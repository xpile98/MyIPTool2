#pragma once
#include "MyIPTool2.h"
#include "MyIPImage.h"

bool IpDilation(MyIPImage &img, CShapeElement &se);
bool IpErotion(MyIPImage &img, CShapeElement &se);
bool IpConditional(MyIPImage &img, MyIPImage &ref);


bool IpGrayDilation(MyIPImage &img, CShapeElement &se);
bool IpGrayErotion(MyIPImage &img, CShapeElement &se);