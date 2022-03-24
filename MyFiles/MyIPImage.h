#pragma once
#include <opencv2/opencv.hpp>
#include "atlimage.h"	//CImage Class Header File
#include "RGBBYTE.h"
#include "RGBWORD.h"
#include "ShapeElement.h"	//190425 YWCho Morph Class


using namespace cv;
using namespace std;

#define USE_IPIMAGE

class MyIPImage : public CImage
{

protected:			//클래스 내부에서는 자유롭게 이용가능하지만, 외부에서는 사용 불가 
	int m_nDepth;
	int m_nChannels;		
	void **m_pPixels;

	double m_dMean;		//평균
	double m_dStdDest;	//표준편차 

public:				//아무데서나 사용 가능 
	MyIPImage();
	~MyIPImage();
	MyIPImage(const MyIPImage &img);
	bool CreateImage(int h, int w, int depth, int channel);



	void SetDepth(int depth) { m_nDepth = depth; };
	int GetDepth() const { return m_nDepth; };
	void SetChannels(int channel) { m_nChannels = channel; };
	int GetChannels() const { return m_nChannels; };

	void SetMean(double mean) { m_dMean = mean; };
	double GetMean() const { return m_dMean; };
	void SetStddDest(double sd) { m_dStdDest = sd; };
	double GetStddDest() const { return m_dStdDest; };


//  void* GetBits() const { return m_pBits; };
// 	int GetPitch() const { return m_nPitch; };

	void** GetPixels() const { return m_pPixels; };

	MyIPImage& operator=(const MyIPImage& img);
	
	void Destroy();
	bool Load(LPCTSTR lpszPathName);
	bool Copy(const MyIPImage &img);
	bool Draw(HDC hDestDC, int xDest, int yDest);
	bool Draw(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, DWORD dwRop = SRCCOPY);
	bool Draw(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight);
	bool GetROI(MyIPImage &img, CRect rect);
};


// template<typename T> inline T limit(const T& value);

//inline T MyIPImage::limit(const T & value)
template<typename T>
inline T limit(const T & value)
{
	return ((value) > 255 ? (T)255 : ((value < 0) ? (T)0 : value));
}

template<typename T>
inline T limit16bit(const T & value)
{
	return ((value) > 65535 ? 65535 : ((value < 0) ? 0 : value));
}

template<typename T>
inline T limit(const T& value, const T& lower, const T& upper)
{
	return ((value > upper) ? upper : ((value < lower) ? lower : value));	
}