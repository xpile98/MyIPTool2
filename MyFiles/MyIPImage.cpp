#include "stdafx.h"
#include "MyIPImage.h"


MyIPImage::MyIPImage()
{
	m_nDepth = 0;
	m_nChannels = 1;
	m_pPixels = NULL;
	m_dMean = 0;
	m_dStdDest = 0;
}


MyIPImage::~MyIPImage()
{
	if (!GetPixels())
		Destroy();
}

MyIPImage::MyIPImage(const MyIPImage & img)
{
	m_nDepth = 0;
	m_nChannels = 1;
	m_pPixels = NULL;
	m_dMean = 0;
	m_dStdDest = 0;
	Copy(img);
}

bool MyIPImage::CreateImage(int h, int w, int depth, int channel)
{
	if (h <= 0 || w <= 0 || depth < 0 || channel <= 0)
		return false;

	RGBQUAD table[256];
	int bpp = 0;

	//만들 수 있는 것은 bpp를 계산하고, 안되는 것은 false를 return 한다 
	switch (depth)		//depth에 지정되지 않은 값이 들어오면 return false 
	{
	case CV_8U:
	case CV_8S:
		if (channel > 4)	return false;
		else bpp = 8 * channel;
		break;

	case CV_16U:
	case CV_16S:
		if (channel > 2)	return false;
		else bpp = 16 * channel;
		break;

	case CV_32S:
	case CV_32F:
		if (channel > 1)	return false;
		else bpp = 32 * channel;
		break;

	default:
		return false;
	}
	if (m_pPixels)	Destroy();

	if (!Create(w, h, bpp))	return false;


	m_nDepth = depth;
	m_nChannels = channel;
	m_pPixels = new void*[h];
	if (m_pPixels == (void **)NULL)
		return false;	//자료형 맞춰주기, new(메모리 할당하고 무조건 에러체크)

	if (channel == 1 && ((depth == CV_8U) || (depth == CV_8S))) {
		//>> grayscale영상은 color table 적용해야되염 190225
		for (int i = 0; i < 256; i++)	//color table 만들고 
		{
			table[i].rgbBlue = table[i].rgbGreen = table[i].rgbRed = (byte)i;
			table[i].rgbReserved = (byte)0;
		}
		SetColorTable(0, 256, table);	//color table 적용
	}

 	int nBPP = GetBPP();							//한 픽셀당 bit 크기. 영상에 따라 1,4,8,24가 있음
	int widthStep = ((w*nBPP / 8 + 3)&~3);			//한 줄의 byte 크기	
													//DIB 구조에서는 영상의 가로 한 줄의 바이트 수가 4의 배수 형태가 되어야 한다...
													//애매하게 37비트! 이러면 9바이트가 아닌 10바이트가 되어야 하는 것 
	BYTE *pData = (BYTE *)GetBits();
	if (GetPitch() < 0) {
		for (int y = 0; y < h; y++)
			m_pPixels[y] = pData - y*widthStep;
	}
	else {
		for (int y = 0; y < h; y++)
			m_pPixels[y] = pData + y*widthStep;
	}

	return true;
}



MyIPImage& MyIPImage::operator=(const MyIPImage& img)
{
	if (this == &img)	
		return *this;					//쪼다같은놈이 있을 수 있으니 ㅋㅋㅋ!	img=img;

	if (!img.GetPixels()) {				//쪼다 2		img2=img, img는 선언만 된 상태고 정의 안됨.
		Destroy();						//죽이고 반환 하자!!!!!!!!!!멍충이
		return *this;
	}

	if (m_pPixels)
		Destroy();

	this->Copy(img);

	return *this; //자기 자신 반환 
}


void MyIPImage::Destroy()
{
	CImage::Destroy();
	if (m_pPixels) {
		m_nDepth = 0;
		m_nChannels = 1;
		delete[] m_pPixels;
	}
}

bool MyIPImage::Load(LPCTSTR lpszPathName)
{
	if (m_pPixels)	//만약 영상 있으면 
		Destroy();	//지우자

	HRESULT result = CImage::Load(lpszPathName);	//아버지한테 읽어달라하고
	if (FAILED(result))								//점검해보자
		return false;

	RGBQUAD	table[256];

	int height = GetHeight();	//아버지 한테 정보를 뽑자~
	int width = GetWidth();
	int bpp = GetBPP();

	switch (bpp)				//bpp로 depth, channel을 가져와보자
	{
	case 8: m_nDepth = CV_8U;m_nChannels = 1; 
		for (int i = 0; i < 256; i++)	//color table 만들고 
		{
			table[i].rgbBlue = table[i].rgbGreen = table[i].rgbRed = (byte)i;
			table[i].rgbReserved = (byte)0;
		}
		SetColorTable(0, 256, table);	//color table 적용

		break;
	case 16: m_nDepth = CV_16U; m_nChannels = 1; break;
	case 24: m_nDepth = CV_8U; m_nChannels = 3; break;
	case 48: m_nDepth = CV_16U; m_nChannels = 3; break;
	default: return false;
	}

	m_pPixels = new void *[height];		//m_pPixels 세팅!!가장중요 
	if (m_pPixels == (void **)NULL)
		return false;	//자료형 맞춰주기, new(메모리 할당하고 무조건 에러체크)



	int widthStep = ((width*bpp / 8 + 3)&~3);		//한 줄의 byte 크기	
													//DIB 구조에서는 영상의 가로 한 줄의 바이트 수가 4의 배수 형태가 되어야 한다...
													//애매하게 37비트! 이러면 9바이트가 아닌 10바이트가 되어야 하는 것 
	BYTE *pData = (BYTE *)GetBits();
	if (GetPitch() < 0) {
		for (int y = 0; y < height; y++)
			m_pPixels[y] = pData - y*widthStep;
	}
	else {
		for (int y = 0; y < height; y++)
			m_pPixels[y] = pData + y*widthStep;
	}

	return true;
}

bool MyIPImage::Copy(const MyIPImage & img)
{
	if (!img.GetPixels()) {				//쪼다 2		img2=img, img는 선언만 된 상태고 정의 안됨.
		Destroy();						//죽이고 반환 하자!!!!!!!!!!멍충이
		return false;
	}

	if (m_pPixels)				//내가 뭘 가지고있으면 지우고 들어가야징 
		Destroy();				

	int width = img.GetWidth();
	int height = img.GetHeight();
	int nBPP = img.GetBPP();							//한 픽셀당 bit 크기. 영상에 따라 1,4,8,24가 있음
													// 	int nBPPxW = (w*GetBPP());						//한 줄의 bit 크기 
	m_nDepth = img.GetDepth();
	m_nChannels = img.GetChannels();
	m_pPixels = new void *[height];		//m_pPixels 세팅!!가장중요 
	if (m_pPixels == (void **)NULL)
		return false;	//자료형 맞춰주기, new(메모리 할당하고 무조건 에러체크)

/*----------------------------------------------------------------------------------*/
	RGBQUAD *table;
	if (!Create(width, height, nBPP))	//Create 실패 시 return false
		return false;

	if (m_nChannels == 1) {
		switch (m_nDepth)
		{
		case CV_8U:
		case CV_8S:
			table = new RGBQUAD[256];
			if (!table) return false;
			img.GetColorTable(0, 256, table);
			SetColorTable(0, 256, table);
			delete[] table;
			break;
// 		case CV_16U:
// 		case CV_16S:
// 			table = new RGBQUAD[65536];
// 			if (!table) return false;
// 			img.GetColorTable(0, 65536, table);
// 			SetColorTable(0, 65536, table);
// 			delete[] table;
// 			break;
		}
	}
	
	int widthStep = ((width*nBPP / 8 + 3)&~3);		//한 줄의 byte 크기	
													//DIB 구조에서는 영상의 가로 한 줄의 바이트 수가 4의 배수 형태가 되어야 한다...
													//애매하게 37비트! 이러면 9바이트가 아닌 10바이트가 되어야 하는 것 
	for (int y = 0; y < height; y++) {
		m_pPixels[y] = GetPixelAddress(0, y);							//내 시작 픽셀 좌표를 구하고
		memcpy(m_pPixels[y], img.GetPixelAddress(0, y), widthStep);		//받은 이미지의 값을 내 좌표에 넣어줌!
	}

	return true; 
}

bool MyIPImage::Draw(HDC hDestDC, int xDest, int yDest)
{
	BOOL bRet;
	if ((m_nDepth == CV_8U) && ((m_nChannels == 1) || (m_nChannels == 3))) {
		bRet = CImage::Draw(hDestDC, 0, 0);
		if (bRet == TRUE)
			return true;
		else
			return false;
	}

	else if ((m_nDepth == CV_16U) && (m_nChannels == 1)) {
		//16bit 를 8bit영상으로 만들어 Draw한다.
		int width = GetWidth();
		int height = GetHeight();
		MyIPImage img;
		if (!img.CreateImage(height, width, CV_8U, 1))	return false;

		byte **bPixels = (byte**)img.GetPixels();					//새로 만든거니까 getpixels
		unsigned short **usPixels = (unsigned short**)m_pPixels;	//My클래스 안에서 쓰니까 m_pPixels
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				bPixels[y][x] = (byte)(usPixels[y][x] / (unsigned short)256);
				//bPixels[y][x] = usPixels[y][x] << 8;
			}
		}
		return img.Draw(hDestDC, xDest, yDest);
	}
	return true;
}

bool MyIPImage::GetROI(MyIPImage & img, CRect rect)
{
	if (!img.GetPixels()) {				//쪼다 2		img2=img, img는 선언만 된 상태고 정의 안됨.
		Destroy();						//죽이고 반환 하자!!!!!!!!!!멍충이
		return false;
	}

	if (m_pPixels)				//내가 뭘 가지고있으면 지우고 들어가야징 
		Destroy();


	int ROIwidth = rect.Width();
	int ROIheight = rect.Height();

	int width = img.GetWidth();
	int height = img.GetHeight();
	int nBPP = img.GetBPP();							//한 픽셀당 bit 크기. 영상에 따라 1,4,8,24가 있음
														// 	int nBPPxW = (w*GetBPP());						//한 줄의 bit 크기 
	m_nDepth = img.GetDepth();
	m_nChannels = img.GetChannels();
	m_pPixels = new void *[height];		//m_pPixels 세팅!!가장중요 
	if (m_pPixels == (void **)NULL)
		return false;	//자료형 맞춰주기, new(메모리 할당하고 무조건 에러체크)

						/*----------------------------------------------------------------------------------*/
	RGBQUAD *table;
	if (!Create(ROIwidth, ROIheight, nBPP))	//Create 실패 시 return false
		return false;

	if (m_nChannels == 1) {
		switch (m_nDepth)
		{
		case CV_8U:
		case CV_8S:
			table = new RGBQUAD[256];
			if (!table) return false;
			img.GetColorTable(0, 256, table);
			SetColorTable(0, 256, table);
			delete[] table;
			break;
			// 		case CV_16U:
			// 		case CV_16S:
			// 			table = new RGBQUAD[65536];
			// 			if (!table) return false;
			// 			img.GetColorTable(0, 65536, table);
			// 			SetColorTable(0, 65536, table);
			// 			delete[] table;
			// 			break;
		}
	}

	int widthStep = ((ROIwidth*nBPP / 8 + 3)&~3);		//한 줄의 byte 크기	
													//DIB 구조에서는 영상의 가로 한 줄의 바이트 수가 4의 배수 형태가 되어야 한다...
													//애매하게 37비트! 이러면 9바이트가 아닌 10바이트가 되어야 하는 것 
	for (int y = 0; y < ROIheight; y++) {
		m_pPixels[y] = GetPixelAddress(0, y);							//내 시작 픽셀 좌표를 구하고
		memcpy(m_pPixels[y], img.GetPixelAddress(rect.left, rect.top + y), widthStep);		//받은 이미지의 값을 내 좌표에 넣어줌!
	}

	return true;
}
