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

	//���� �� �ִ� ���� bpp�� ����ϰ�, �ȵǴ� ���� false�� return �Ѵ� 
	switch (depth)		//depth�� �������� ���� ���� ������ return false 
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
		return false;	//�ڷ��� �����ֱ�, new(�޸� �Ҵ��ϰ� ������ ����üũ)

	if (channel == 1 && ((depth == CV_8U) || (depth == CV_8S))) {
		//>> grayscale������ color table �����ؾߵǿ� 190225
		for (int i = 0; i < 256; i++)	//color table ����� 
		{
			table[i].rgbBlue = table[i].rgbGreen = table[i].rgbRed = (byte)i;
			table[i].rgbReserved = (byte)0;
		}
		SetColorTable(0, 256, table);	//color table ����
	}

 	int nBPP = GetBPP();							//�� �ȼ��� bit ũ��. ���� ���� 1,4,8,24�� ����
	int widthStep = ((w*nBPP / 8 + 3)&~3);			//�� ���� byte ũ��	
													//DIB ���������� ������ ���� �� ���� ����Ʈ ���� 4�� ��� ���°� �Ǿ�� �Ѵ�...
													//�ָ��ϰ� 37��Ʈ! �̷��� 9����Ʈ�� �ƴ� 10����Ʈ�� �Ǿ�� �ϴ� �� 
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
		return *this;					//�ɴٰ������� ���� �� ������ ������!	img=img;

	if (!img.GetPixels()) {				//�ɴ� 2		img2=img, img�� ���� �� ���°� ���� �ȵ�.
		Destroy();						//���̰� ��ȯ ����!!!!!!!!!!������
		return *this;
	}

	if (m_pPixels)
		Destroy();

	this->Copy(img);

	return *this; //�ڱ� �ڽ� ��ȯ 
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
	if (m_pPixels)	//���� ���� ������ 
		Destroy();	//������

	HRESULT result = CImage::Load(lpszPathName);	//�ƹ������� �о�޶��ϰ�
	if (FAILED(result))								//�����غ���
		return false;

	RGBQUAD	table[256];

	int height = GetHeight();	//�ƹ��� ���� ������ ����~
	int width = GetWidth();
	int bpp = GetBPP();

	switch (bpp)				//bpp�� depth, channel�� �����ͺ���
	{
	case 8: m_nDepth = CV_8U;m_nChannels = 1; 
		for (int i = 0; i < 256; i++)	//color table ����� 
		{
			table[i].rgbBlue = table[i].rgbGreen = table[i].rgbRed = (byte)i;
			table[i].rgbReserved = (byte)0;
		}
		SetColorTable(0, 256, table);	//color table ����

		break;
	case 16: m_nDepth = CV_16U; m_nChannels = 1; break;
	case 24: m_nDepth = CV_8U; m_nChannels = 3; break;
	case 48: m_nDepth = CV_16U; m_nChannels = 3; break;
	default: return false;
	}

	m_pPixels = new void *[height];		//m_pPixels ����!!�����߿� 
	if (m_pPixels == (void **)NULL)
		return false;	//�ڷ��� �����ֱ�, new(�޸� �Ҵ��ϰ� ������ ����üũ)



	int widthStep = ((width*bpp / 8 + 3)&~3);		//�� ���� byte ũ��	
													//DIB ���������� ������ ���� �� ���� ����Ʈ ���� 4�� ��� ���°� �Ǿ�� �Ѵ�...
													//�ָ��ϰ� 37��Ʈ! �̷��� 9����Ʈ�� �ƴ� 10����Ʈ�� �Ǿ�� �ϴ� �� 
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
	if (!img.GetPixels()) {				//�ɴ� 2		img2=img, img�� ���� �� ���°� ���� �ȵ�.
		Destroy();						//���̰� ��ȯ ����!!!!!!!!!!������
		return false;
	}

	if (m_pPixels)				//���� �� ������������ ����� ����¡ 
		Destroy();				

	int width = img.GetWidth();
	int height = img.GetHeight();
	int nBPP = img.GetBPP();							//�� �ȼ��� bit ũ��. ���� ���� 1,4,8,24�� ����
													// 	int nBPPxW = (w*GetBPP());						//�� ���� bit ũ�� 
	m_nDepth = img.GetDepth();
	m_nChannels = img.GetChannels();
	m_pPixels = new void *[height];		//m_pPixels ����!!�����߿� 
	if (m_pPixels == (void **)NULL)
		return false;	//�ڷ��� �����ֱ�, new(�޸� �Ҵ��ϰ� ������ ����üũ)

/*----------------------------------------------------------------------------------*/
	RGBQUAD *table;
	if (!Create(width, height, nBPP))	//Create ���� �� return false
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
	
	int widthStep = ((width*nBPP / 8 + 3)&~3);		//�� ���� byte ũ��	
													//DIB ���������� ������ ���� �� ���� ����Ʈ ���� 4�� ��� ���°� �Ǿ�� �Ѵ�...
													//�ָ��ϰ� 37��Ʈ! �̷��� 9����Ʈ�� �ƴ� 10����Ʈ�� �Ǿ�� �ϴ� �� 
	for (int y = 0; y < height; y++) {
		m_pPixels[y] = GetPixelAddress(0, y);							//�� ���� �ȼ� ��ǥ�� ���ϰ�
		memcpy(m_pPixels[y], img.GetPixelAddress(0, y), widthStep);		//���� �̹����� ���� �� ��ǥ�� �־���!
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
		//16bit �� 8bit�������� ����� Draw�Ѵ�.
		int width = GetWidth();
		int height = GetHeight();
		MyIPImage img;
		if (!img.CreateImage(height, width, CV_8U, 1))	return false;

		byte **bPixels = (byte**)img.GetPixels();					//���� ����Ŵϱ� getpixels
		unsigned short **usPixels = (unsigned short**)m_pPixels;	//MyŬ���� �ȿ��� ���ϱ� m_pPixels
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
	if (!img.GetPixels()) {				//�ɴ� 2		img2=img, img�� ���� �� ���°� ���� �ȵ�.
		Destroy();						//���̰� ��ȯ ����!!!!!!!!!!������
		return false;
	}

	if (m_pPixels)				//���� �� ������������ ����� ����¡ 
		Destroy();


	int ROIwidth = rect.Width();
	int ROIheight = rect.Height();

	int width = img.GetWidth();
	int height = img.GetHeight();
	int nBPP = img.GetBPP();							//�� �ȼ��� bit ũ��. ���� ���� 1,4,8,24�� ����
														// 	int nBPPxW = (w*GetBPP());						//�� ���� bit ũ�� 
	m_nDepth = img.GetDepth();
	m_nChannels = img.GetChannels();
	m_pPixels = new void *[height];		//m_pPixels ����!!�����߿� 
	if (m_pPixels == (void **)NULL)
		return false;	//�ڷ��� �����ֱ�, new(�޸� �Ҵ��ϰ� ������ ����üũ)

						/*----------------------------------------------------------------------------------*/
	RGBQUAD *table;
	if (!Create(ROIwidth, ROIheight, nBPP))	//Create ���� �� return false
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

	int widthStep = ((ROIwidth*nBPP / 8 + 3)&~3);		//�� ���� byte ũ��	
													//DIB ���������� ������ ���� �� ���� ����Ʈ ���� 4�� ��� ���°� �Ǿ�� �Ѵ�...
													//�ָ��ϰ� 37��Ʈ! �̷��� 9����Ʈ�� �ƴ� 10����Ʈ�� �Ǿ�� �ϴ� �� 
	for (int y = 0; y < ROIheight; y++) {
		m_pPixels[y] = GetPixelAddress(0, y);							//�� ���� �ȼ� ��ǥ�� ���ϰ�
		memcpy(m_pPixels[y], img.GetPixelAddress(rect.left, rect.top + y), widthStep);		//���� �̹����� ���� �� ��ǥ�� �־���!
	}

	return true;
}
