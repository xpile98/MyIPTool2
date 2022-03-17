#pragma once

#include "MyIPImage.h"



bool IpMatToImage(Mat& mat, MyIPImage& img);
bool IpImageToMat(MyIPImage& img, Mat& mat);
bool IpImageToMat2(MyIPImage& img, Mat& mat);


bool IpRGBToGray(MyIPImage& rgb, MyIPImage& gray);
bool IpRGBToHSI(double R, double G, double B, double& H, double& S, double&I);
bool IpHSIToRGB(double H, double S, double I, double& R, double& G, double&B);
bool IpRGBToYUV(byte r, byte g, byte b, byte& y, byte& u, byte& v);
bool IpYUVToRGB(byte y, byte u, byte v, byte& r, byte& g, byte& b);
bool IpColorSplitRGB(MyIPImage& rgb, MyIPImage& rimg, MyIPImage& gimg, MyIPImage& bimg);
bool IpColorSplitHSI(MyIPImage& rgb, MyIPImage& himg, MyIPImage& simg, MyIPImage& iimg);
bool IpColorSplitYUV(MyIPImage& rgb, MyIPImage& yimg, MyIPImage& uimg, MyIPImage& vimg);
bool IpColorConbineRGB(MyIPImage& rimg, MyIPImage& gimg, MyIPImage& bimg, MyIPImage& rgb);
bool IpColorConbineHSI(MyIPImage& himg, MyIPImage& simg, MyIPImage& iimg, MyIPImage& rgb);
bool IpColorConbineYUV(MyIPImage& yimg, MyIPImage& uimg, MyIPImage& vimg, MyIPImage& rgb);


bool IpRGBToGray(Mat& rgb, Mat& gray);
bool IpRGBToHSI(Mat rgb, Mat& hsi);
bool IpColorSplitRGB(Mat& rgb, Mat& out);
bool IpColorSplitHSI(Mat& rgb, Mat& hsi);
bool IpColorSplitYUV(Mat& rgb, Mat& yuv);
bool IpColorSplitHSV(Mat& rgb, Mat& hsv);
bool IpColorSplitYCrCB(Mat& rgb, Mat& YCrCB);
bool IpColorSplitLAB(Mat& rgb, Mat& lab);
bool IpColorSplitCMY(Mat& rgb, Mat& cmy);
bool IpColorSplitCMYK(Mat& rgb, Mat& cmyk);



bool IpConvert8to16(MyIPImage &img, MyIPImage &out);
bool IpResizeNearest(MyIPImage &img, MyIPImage &out, int nh, int nw);