#include "stdafx.h"
#include "MyIPConvert.h"

const double PI = 3.14159265358979323846;

bool IpMatToImage(Mat& mat, MyIPImage& img)	//직접 인자를 바꿈
{
	if (!mat.data)
		return false;

	//img copy
	int depth = mat.depth();
	int channel = mat.channels();
	int width = mat.cols;
	int height = mat.rows;

	if (!img.CreateImage(height, width, depth, channel))
		return false;
	void** Pixels = img.GetPixels();

	//한 줄에 copy해야 할 byte개수를 계산해 내자 
	int nbyte;

	//data copy
	switch (depth)
	{
	case CV_8U:
	case CV_8S:
		nbyte = channel*width;
		break;

	case CV_16U:
	case CV_16S:
		nbyte = 2*channel*width;
		break;

	case CV_32F:
	case CV_32S:
		nbyte = 4 * channel*width;
		break;

	default:
		return false;
	}


	for (int y = 0; y < height; y++) {
		memcpy(Pixels[y], mat.ptr(y), nbyte);
	}

	if (Pixels == (void**)NULL)	//memcpy 가 제대로 되지 않았다면 return false;
		return false;

	return true;
}

bool IpImageToMat(MyIPImage& img, Mat& mat)
{
	if (img.IsNull())
		return false;

	int width = img.GetWidth();
	int height = img.GetHeight();
	int depth = img.GetDepth();
	int bpp = img.GetBPP();
	int channel = img.GetChannels();
	void** Pixels = img.GetPixels();

	//한 줄에 copy해야 할 byte개수를 계산해 내자 
	int nbyte;

	//data copy
	switch (depth)
	{
	case CV_8U:
		nbyte = channel*width;
		switch (channel) {
		case 1: mat = Mat(height, width, CV_8UC1); break;
		case 2: mat = Mat(height, width, CV_8UC2); break;
		case 3: mat = Mat(height, width, CV_8UC3); break;
		case 4: mat = Mat(height, width, CV_8UC4); break;
		default: return false;
		}
		break;
	case CV_8S:
		nbyte = channel*width;
		switch (channel) {
		case 1: mat = Mat(height, width, CV_8SC1); break;
		case 2: mat = Mat(height, width, CV_8SC2); break;
		case 3: mat = Mat(height, width, CV_8SC3); break;
		case 4: mat = Mat(height, width, CV_8SC4); break;
		default: return false;
		}
		break;
	case CV_16U:
		nbyte = 2*channel*width;
		switch (channel) {
		case 1: mat = Mat(height, width, CV_16UC1); break;
		case 2: mat = Mat(height, width, CV_16UC2); break;
		default: return false;
		}
		break;
	case CV_16S:
		nbyte = 2 * channel*width;
		switch (channel) {
		case 1: mat = Mat(height, width, CV_16SC1); break;
		case 2: mat = Mat(height, width, CV_16SC2); break;
		default: return false;
		}
		break;
	case CV_32F:
		nbyte = 4 * channel*width;
		mat = Mat(height, width, CV_32FC1); break;
		break;
	case CV_32S:
		nbyte = 4 * channel*width;
		mat = Mat(height, width, CV_32SC1); break;
		break;

	default:
		return false;
	}

	if (!mat.data)			//잘 만드어졌는지 체크!!!!
		return false;

	for (int y = 0; y < height; y++) {
		memcpy(mat.ptr(y), Pixels[y], nbyte);
	}

	return true;
}

bool IpImageToMat2(MyIPImage & img, Mat & mat)
{
	int depth = img.GetDepth();
	int channel = img.GetBPP() / 8;
	int width = img.GetWidth();
	int height = img.GetHeight();
	void** Pixel = img.GetPixels();

	switch (depth)
	{
	case CV_8U:
		switch (img.GetBPP()/8)		
		{
		case 1:
			mat = Mat(height, width, CV_8UC1);
			break;

		case 3:
			mat = Mat(height, width, CV_8UC3);
			break;

		default:
			return false;
		}
		break;

	case CV_16U:
		switch (img.GetBPP()/16)	//getbpp/8을 하면 16비트 1채널 이미지는 결과가 2가 나오기 때문에 안될거같은데.. 질문드리자 
		{
		case 1:
			mat = Mat(height, width, CV_16UC1);
			break;

		case 3:
			mat = Mat(height, width, CV_16UC3);
			break;

		default:
			return false;
		}
		break;

	default:
		return false;
	}


	if (!mat.data)			//잘 만드어졌는지 체크!!!!
		return false;

	Pixel = new void*[height];
	if (Pixel == (void**)NULL)
		return false;

	for (int y = 0; y < height; y++) {
		memcpy(Pixel[y], mat.ptr(y), width);
	}

	return true;
}


/*--------------------MyIPImage Color Conversion--------------------*/
bool IpRGBToGray(MyIPImage & rgb, MyIPImage & gray)
{
	if (!rgb.GetPixels())
		return false;

	int width = rgb.GetWidth();
	int height = rgb.GetHeight();
	int depth = rgb.GetDepth();
	int channel = rgb.GetChannels();
	RGBBYTE** prgb_pixel = (RGBBYTE**)rgb.GetPixels();

	if (depth != CV_8U && depth != CV_16U)	//8,16비트 아니면 return false;
		return false;

	if (channel != 3)						//3ch ( true color ) 이 아니면 return false
		return false;

	if (!gray.CreateImage(height, width, depth, 1))
		return false;

	float Y;
	byte** pb_Pixel;			//8비트용
	//unsigned short** pus_Pixel;	//16비트용

	switch (depth)
	{
	case CV_8U:
		pb_Pixel = (byte**)gray.GetPixels();
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				Y = (float)((0.299 * (float)prgb_pixel[y][x].r) + (0.587 * (float)prgb_pixel[y][x].g) + (0.114 * (float)prgb_pixel[y][x].b) + 0.5f);
				if (Y > 255.f)	pb_Pixel[y][x] = (byte)255;
				else pb_Pixel[y][x] = (byte)Y;
			}
		}
		break;

	case CV_16U:
		pb_Pixel = (byte**)gray.GetPixels();
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
			}
		}
		break;

	default:
		return false;
	}


	return true;
}

bool IpRGBToHSI(double R, double G, double B, double & H, double & S, double & I)
{
	/*
	Hue :			색상	빨,노,녹,파,자주 등 색의 종류를 의미 
	Saturation :	채도	색의 탁하고 선명한 정도를 표현. 빨간색의 채도가 높으면 밝은 선홍색에 가까움.
							빨간색의 채도가 낮으면 뿌연 빨간색으로 표시됨 
	Intensity :		명도	빛의 밝기를 나타냄 
	*/
	I = (R + G + B) / 3;

	if ((R == G) && (G == B))	//r,g,b, 모두 같으면 -> 그레이 스케일 인 경우 
	{
		S = 0;
		H = 0;
		return false;
	}
	else
	{
		double min_rgb;
		double temp;
		
		min_rgb = min(min(R, G), B);	//R,G,B중 작은 값 반환.
		S = 1 - (min_rgb / I);
		temp = ((R - G) + (R - B)) / (2 * sqrt((R - G)*(R - G) + (R - B)*(G - B)));

		H = acos(temp) * 180 / PI;

		if (B > G)
			H = 360 - H;

		H /= 360;
	}
	return true;
}

bool IpHSIToRGB(double H, double S, double I, double & R, double & G, double & B)
{
	if (I == 0.0)	//검정색
	{
		R = 0.0;
		G = 0.0;
		B = 0.0;
		return false;
	}

	if (S == 0.0)	//그레이스케일
	{
		R = I;
		G = I;
		B = I;
		return false;
	}

	H *= 360;	//[0~1] 범위를 [0~360] 각도 범위로 변경 

	if (H <= 120)
	{
		B = I*(1 - S);
		R = I*(1 + S*cos(H*PI / 180) / cos((60 - H)*PI / 180));
		R = limit(R, 0., 1.);
		G = 3 * I - (R + B);
	}
	else if (H <= 240)
	{
		H -= 120;
		R = I*(1 - S);
		G = I*(1 + S*cos(H*PI / 180) / cos((60 - H)*PI / 180));
		G = limit(G, 0., 1.);
		B = 3 * I - (R + G);
	}
	else
	{
		H -= 240;
		G = I*(1 - S);
		B = I*(1 + S*cos(H*PI / 180) / cos((60 - H)*PI / 180));
		B = limit(B, 0., 1.);
		R = 3 * I - (G + B);
	}
	return true;
}

bool IpRGBToYUV(byte r, byte g, byte b, byte & y, byte & u, byte & v)
{
	y = (byte)limit(0.299*r + 0.587*g + 0.114*b + 0.5);
	u = (byte)limit(-0.169*r - 0.331*g + 0.500*b + 128 + 0.5);
	v = (byte)limit(0.500*r - 0.419*g - 0.081*b + 128 + 0.5);

	return true;
}

bool IpYUVToRGB(byte y, byte u, byte v, byte & r, byte & g, byte & b)
{
	r = (byte)limit(y + 1.4075*(v - 128) + 0.5);
	g = (byte)limit(y - 0.3455*(u - 128) - 0.7169*(v - 128) + 0.5);
	b = (byte)limit(y + 1.7790*(u - 128) + 0.5);

	return true;
}

bool IpColorSplitRGB(MyIPImage & rgb, MyIPImage & rimg, MyIPImage & gimg, MyIPImage & bimg)
{
	if (!rgb.GetPixels())
		return false;

	int width = rgb.GetWidth();
	int height = rgb.GetHeight();
	int depth = rgb.GetDepth();
	int channel = rgb.GetChannels();
	RGBBYTE** prgb_Pixel = (RGBBYTE**)rgb.GetPixels();
	BYTE** pb_rPixel;
	BYTE** pb_gPixel;
	BYTE** pb_bPixel;


	if (depth != CV_8U && depth != CV_16U)	//8,16비트 아니면 return false;
		return false;

	if (channel != 3)						//3ch ( true color ) 이 아니면 return false
		return false;

	if (!rimg.CreateImage(height, width, depth, 1))
		return false;

	if (!gimg.CreateImage(height, width, depth, 1))
		return false;

	if (!bimg.CreateImage(height, width, depth, 1))
		return false;

	switch (depth)
	{
	case CV_8U:
		pb_rPixel = (BYTE**)rimg.GetPixels();
		pb_gPixel = (BYTE**)gimg.GetPixels();
		pb_bPixel = (BYTE**)bimg.GetPixels();

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				pb_rPixel[y][x] = prgb_Pixel[y][x].r;
				pb_gPixel[y][x] = prgb_Pixel[y][x].g;
				pb_bPixel[y][x] = prgb_Pixel[y][x].b;
			}
		}
		break;

	case CV_16U:	//아직 안함 
		break;

	default:
		return false;
	}
	return true;
}

bool IpColorSplitHSI(MyIPImage &rgb, MyIPImage &himg, MyIPImage &simg, MyIPImage &iimg)
{
	if (!rgb.GetPixels())
		return false;

	int width = rgb.GetWidth();
	int height = rgb.GetHeight();
	int depth = rgb.GetDepth();
	int channel = rgb.GetChannels();
	RGBBYTE** prgb_Pixel = (RGBBYTE**)rgb.GetPixels();
	BYTE** pb_hPixel;
	BYTE** pb_sPixel;
	BYTE** pb_iPixel;


	if (depth != CV_8U && depth != CV_16U)	//8비트도,16비트도 아니면 return false;
		return false;

	if (channel != 3)						//3ch ( true color ) 이 아니면 return false
		return false;

	if (!himg.CreateImage(height, width, depth, 1))
		return false;

	if (!simg.CreateImage(height, width, depth, 1))
		return false;

	if (!iimg.CreateImage(height, width, depth, 1))
		return false;

	double r, g, b, h, s, i;

	switch (depth)
	{
	case CV_8U:					
		pb_hPixel = (BYTE**)himg.GetPixels();
		pb_sPixel = (BYTE**)simg.GetPixels();
		pb_iPixel = (BYTE**)iimg.GetPixels();

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				r = (double)prgb_Pixel[y][x].r / 255.;	//double 안해주면 안되여~~~~
				g = (double)prgb_Pixel[y][x].g / 255.;
				b = (double)prgb_Pixel[y][x].b / 255.;

				IpRGBToHSI(r, g, b, h, s, i);

				pb_hPixel[y][x] = static_cast<BYTE>(limit(h * 255 + 0.5));
				pb_sPixel[y][x] = static_cast<BYTE>(limit(s * 255 + 0.5));
				pb_iPixel[y][x] = static_cast<BYTE>(limit(i * 255 + 0.5));
			}
		}
		break;

	case CV_16U:	//아직 안함 
		break;

	default:
		return false;
	}
	return true;
}

bool IpColorSplitYUV(MyIPImage &rgb, MyIPImage &yimg, MyIPImage &uimg, MyIPImage &vimg)
{
	if (!rgb.GetPixels())
		return false;

	int width = rgb.GetWidth();
	int height = rgb.GetHeight();
	int depth = rgb.GetDepth();
	int channel = rgb.GetChannels();
	RGBBYTE** prgb_Pixel = (RGBBYTE**)rgb.GetPixels();
	BYTE** pb_yPixel;
	BYTE** pb_uPixel;
	BYTE** pb_vPixel;


	if (depth != CV_8U && depth != CV_16U)	//8비트도,16비트도 아니면 return false;
		return false;

	if (channel != 3)						//3ch ( true color ) 이 아니면 return false
		return false;

	if (!yimg.CreateImage(height, width, depth, 1))
		return false;

	if (!uimg.CreateImage(height, width, depth, 1))
		return false;

	if (!vimg.CreateImage(height, width, depth, 1))
		return false;

	switch (depth)
	{
	case CV_8U:
		pb_yPixel = (BYTE**)yimg.GetPixels();
		pb_uPixel = (BYTE**)uimg.GetPixels();
		pb_vPixel = (BYTE**)vimg.GetPixels();

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				IpRGBToYUV(prgb_Pixel[y][x].r, prgb_Pixel[y][x].g, prgb_Pixel[y][x].b, 
					pb_yPixel[y][x], pb_uPixel[y][x], pb_vPixel[y][x]);
			}
		}
		break;

	case CV_16U:	//아직 안함 
		break;

	default:
		return false;
	}
	return true;
}

bool IpColorConbineRGB(MyIPImage & rimg, MyIPImage & gimg, MyIPImage & bimg, MyIPImage & rgb)
{
	if (!rimg.GetPixels())
		return false;

	int width = rimg.GetWidth();
	int height = rimg.GetHeight();
	int depth = rimg.GetDepth();
	int channel = rimg.GetChannels();

	if (rimg.GetChannels() != 1 || gimg.GetChannels() != 1 || bimg.GetChannels() != 1)	//영상이 하나라도 흑백이 아니면
		return false;

	if (gimg.GetWidth() != width || bimg.GetWidth() != width ||							//영상의 크기가 다르면 
		gimg.GetHeight() != height || bimg.GetHeight() != height)
		return false;

	rgb.CreateImage(width, height,depth,3);

	BYTE** pb_rPixel = (BYTE**)rimg.GetPixels();
	BYTE** pb_gPixel = (BYTE**)gimg.GetPixels();
	BYTE** pb_bPixel = (BYTE**)bimg.GetPixels();
	RGBBYTE** pbRGB_Pixel = (RGBBYTE**)rgb.GetPixels();
	WORD** pw_rPixel = (WORD**)rimg.GetPixels();
	WORD** pw_gPixel = (WORD**)gimg.GetPixels();
	WORD** pw_bPixel = (WORD**)bimg.GetPixels();
	RGBWORD** pwRGB_Pixel = (RGBWORD**)rgb.GetPixels();

	switch (depth)
	{
	case CV_8U:
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				pbRGB_Pixel[y][x].r = pb_rPixel[y][x];
				pbRGB_Pixel[y][x].g = pb_gPixel[y][x];
				pbRGB_Pixel[y][x].b = pb_bPixel[y][x];
			}
		}
		break;
	case CV_16U:	//확인필요
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				pwRGB_Pixel[y][x].r = pw_rPixel[y][x];
				pwRGB_Pixel[y][x].g = pw_gPixel[y][x];
				pwRGB_Pixel[y][x].b = pw_bPixel[y][x];
			}
		}
		break;
	default:
		return false;
	}

	return true;
}

bool IpColorConbineHSI(MyIPImage & himg, MyIPImage & simg, MyIPImage & iimg, MyIPImage & rgb)
{
	if (!himg.GetPixels())
		return false;

	int width = himg.GetWidth();
	int height = himg.GetHeight();
	int depth = himg.GetDepth();
	int channel = himg.GetChannels();

	if (himg.GetChannels() != 1 || simg.GetChannels() != 1 || iimg.GetChannels() != 1)	//영상이 하나라도 흑백이 아니면
		return false;

	if (simg.GetWidth() != width || iimg.GetWidth() != width ||							//영상의 크기가 다르면 
		simg.GetHeight() != height || iimg.GetHeight() != height)
		return false;

	rgb.CreateImage(width, height, depth, 3);

	BYTE** pb_hPixel = (BYTE**)himg.GetPixels();
	BYTE** pb_sPixel = (BYTE**)simg.GetPixels();
	BYTE** pb_iPixel = (BYTE**)iimg.GetPixels();
	RGBBYTE** pbRGB_Pixel = (RGBBYTE**)rgb.GetPixels();
	WORD** pw_hPixel = (WORD**)himg.GetPixels();
	WORD** pw_sPixel = (WORD**)simg.GetPixels();
	WORD** pw_iPixel = (WORD**)iimg.GetPixels();
	RGBWORD** pwRGB_Pixel = (RGBWORD**)rgb.GetPixels();

	double r, g, b, hh, ss, ii;

	switch (depth)
	{
	case CV_8U:
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {

				hh = pb_hPixel[y][x] / 255.;
				ss = pb_sPixel[y][x] / 255.;
				ii = pb_iPixel[y][x] / 255.;
				IpHSIToRGB(hh, ss, ii, r, g, b);
				pbRGB_Pixel[y][x].r = static_cast<BYTE>(limit(r * 255 + 0.5f));
				pbRGB_Pixel[y][x].g = static_cast<BYTE>(limit(g * 255 + 0.5f));
				pbRGB_Pixel[y][x].b = static_cast<BYTE>(limit(b * 255 + 0.5f));
			}
		}
		break;
	case CV_16U:	//확인필요 
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {

				hh = pw_hPixel[y][x] / 65535;
				ss = pw_sPixel[y][x] / 65535;
				ii = pw_iPixel[y][x] / 65535;
				IpHSIToRGB(hh, ss, ii, r, g, b);
				pwRGB_Pixel[y][x].r = static_cast<WORD>(limit(r * 65535 + 0.5f));
				pwRGB_Pixel[y][x].g = static_cast<WORD>(limit(g * 65535 + 0.5f));
				pwRGB_Pixel[y][x].b = static_cast<WORD>(limit(b * 65535 + 0.5f));
			}
		}
		break;
	default:
		return false;
	}


	return true;
}

bool IpColorConbineYUV(MyIPImage & yimg, MyIPImage & uimg, MyIPImage & vimg, MyIPImage & rgb)
{
	if (!yimg.GetPixels())
		return false;

	int width = yimg.GetWidth();
	int height = yimg.GetHeight();
	int depth = yimg.GetDepth();
	int channel = yimg.GetChannels();

	if (uimg.GetWidth() != width || vimg.GetWidth() != width ||
		uimg.GetHeight() != height || vimg.GetHeight() != height)
		return false;

	rgb.CreateImage(width, height, depth, 3);

	BYTE** pb_yPixel = (BYTE**)yimg.GetPixels();
	BYTE** pb_uPixel = (BYTE**)uimg.GetPixels();
	BYTE** pb_vPixel = (BYTE**)vimg.GetPixels();
	RGBBYTE** pbrgb_Pixel = (RGBBYTE**)rgb.GetPixels();
	WORD** pw_yPixel = (WORD**)yimg.GetPixels();
	WORD** pw_uPixel = (WORD**)uimg.GetPixels();
	WORD** pw_vPixel = (WORD**)vimg.GetPixels();
	RGBWORD** pwRGB_Pixel = (RGBWORD**)rgb.GetPixels();

	switch (depth)
	{
	case CV_8U:
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++)
			{
				IpYUVToRGB(pb_yPixel[y][x], pb_uPixel[y][x], pb_vPixel[y][x],
					pbrgb_Pixel[y][x].r, pbrgb_Pixel[y][x].g, pbrgb_Pixel[y][x].b);
			}
		}
		break;
	case CV_16U:	//아직 안함 
		break;
	default:
		return false;
	}

	return true;
}




/*--------------------Open CV Color Conversion-----------------------*/
bool IpRGBToGray(Mat & rgb, Mat & gray)
{
	if (!rgb.data)
		return false;

	int width = rgb.cols;
	int height = rgb.rows;
	int depth = rgb.depth();
	int channel = rgb.channels();

	if (depth != CV_8U && depth != CV_16U)	//8,16비트 아니면 return false;
		return false;

	if (channel != 3)						//3ch ( true color ) 이 아니면 return false
		return false;

	switch (depth)
	{
	case CV_8U:
		cvtColor(rgb, gray, COLOR_BGR2GRAY);
		return true;
		break;

	case CV_16U:	//
		break;

	default:
		return false;
	}

	return true;
}

bool IpRGBToHSI(Mat rgb, Mat & hsi)
{
	if (!rgb.data)
		return false;

	int width = rgb.cols;
	int height = rgb.rows;
	int depth = rgb.depth();
	int channel = rgb.channels();


	hsi = Mat(rgb.size(), CV_32FC3);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			float R = rgb.at<Vec3b>(y, x)[2];
			float G = rgb.at<Vec3b>(y, x)[1];
			float B = rgb.at<Vec3b>(y, x)[0];

			float S = 1 - (3 * min(R, min(G, B)) / (R + B + G));
			float I = (R + G + B) / 3.f;

			float temp1 = ((R - G) + (R - B)) * 0.5f;
			float temp2 = sqrt((R - G)*(R - G) + (R - B)*(G - B));
			float angle = (float)(acos(temp1 / temp2) * 180.f / PI);
			float H = (B <= G) ? angle : 360 - angle;

			//hsi.at<Vec3f>(y, x) = Vec3f(H/2, S*255., I);	//Open CV교재
			hsi.at<Vec3f>(y, x) = Vec3f(H/360.f*255.f, S*255.f, I);	//Visual C++ 교재
		}
	}
	hsi.convertTo(hsi, CV_8U);
	return true;
}

bool IpColorSplitRGB(Mat & rgb, Mat &out)
{
	if (!rgb.data)
		return false;

	int width = rgb.cols;
	int height = rgb.rows;
	int depth = rgb.depth();
	int channel = rgb.channels();

	rgb.copyTo(out);

	if (depth != CV_8U && depth != CV_16U)	//8비트도,16비트도 아니면 return false;
		return false;

	if (channel != 3)						//3ch ( true color ) 이 아니면 return false
		return false;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			out.at<Vec3b>(y, x)[0] = (byte)rgb.at<Vec3b>(y, x)[2];
			out.at<Vec3b>(y, x)[1] = (byte)rgb.at<Vec3b>(y, x)[1];
			out.at<Vec3b>(y, x)[2] = (byte)rgb.at<Vec3b>(y, x)[0];
		}
	}

	return true;
}

bool IpColorSplitHSI(Mat &rgb, Mat &hsi)
{
	if (!rgb.data)
		return false;

	int width = rgb.cols;
	int height = rgb.rows;
	int depth = rgb.depth();
	int channel = rgb.channels();

	if (depth != CV_8U && depth != CV_16U)	//8비트도,16비트도 아니면 return false;
		return false;

	if (channel != 3)						//3ch ( true color ) 이 아니면 return false
		cvtColor(rgb, rgb, COLOR_GRAY2BGR);

	switch (depth)
	{
	case CV_8U:
		IpRGBToHSI(rgb, hsi);
		break;

	case CV_16U:
		break;

	default:
		return false;
	}
	return true;
}

bool IpColorSplitYUV(Mat &rgb, Mat &yuv)
{
	if (!rgb.data)
		return false;

	if (rgb.depth() != CV_8U && rgb.depth() != CV_16U)	//8비트도,16비트도 아니면 return false;
		return false;

	if (rgb.channels() != 3)							//3ch ( true color ) 이 아니면 return false
		return false;

	cvtColor(rgb, yuv, COLOR_BGR2YUV);
	return true;
}

bool IpColorSplitHSV(Mat & rgb, Mat & hsv)
{
	if (!rgb.data)
		return false;

	if (rgb.depth() != CV_8U && rgb.depth() != CV_16U)	//8비트도,16비트도 아니면 return false;
		return false;

	if (rgb.channels() != 3)							//3ch ( true color ) 이 아니면 return false
		return false;

	cvtColor(rgb, hsv, COLOR_BGR2HSV);
	return true;
}

bool IpColorSplitYCrCB(Mat & rgb, Mat & YCrCB)
{
	if (!rgb.data)
		return false;

	if (rgb.depth() != CV_8U && rgb.depth() != CV_16U)	//8비트도,16비트도 아니면 return false;
		return false;

	if (rgb.channels() != 3)							//3ch ( true color ) 이 아니면 return false
		cvtColor(rgb, rgb, COLOR_GRAY2BGR);

	cvtColor(rgb, YCrCB, COLOR_BGR2YCrCb);
	return true;
}

bool IpColorSplitLAB(Mat & rgb, Mat & lab)
{
	if (!rgb.data)
		return false;

	if (rgb.depth() != CV_8U && rgb.depth() != CV_16U)	//8비트도,16비트도 아니면 return false;
		return false;

	if (rgb.channels() != 3)							//3ch ( true color ) 이 아니면 return false
		cvtColor(rgb, rgb, COLOR_GRAY2BGR);

	cvtColor(rgb, lab, COLOR_BGR2Lab);
	return true;
}

bool IpColorSplitCMY(Mat & rgb, Mat & cmy)
{
	if (!rgb.data)
		return false;

	if (rgb.depth() != CV_8U && rgb.depth() != CV_16U)	//8비트도,16비트도 아니면 return false;
		return false;

	if (rgb.channels() != 3)							//3ch ( true color ) 이 아니면 return false
		return false;

	int width = rgb.cols;
	int height = rgb.rows;

	Scalar white(255, 255, 255);
	byte temp;
	
	cmy = white - rgb;	//변환식
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			temp = (byte)cmy.at<Vec3b>(y, x)[0];
			cmy.at<Vec3b>(y, x)[0] = (byte)cmy.at<Vec3b>(y, x)[2];	//C
			cmy.at<Vec3b>(y, x)[1] = (byte)cmy.at<Vec3b>(y, x)[1];	//M
			cmy.at<Vec3b>(y, x)[2] = temp;							//Y
		}
	}
	return true;
}

bool IpColorSplitCMYK(Mat & rgb, Mat & cmyk)
{
	if (!rgb.data)
		return false;

	if (rgb.depth() != CV_8U && rgb.depth() != CV_16U)	//8비트도,16비트도 아니면 return false;
		return false;

	if (rgb.channels() != 3)							//3ch ( true color ) 이 아니면 return false
		return false;

	int width = rgb.cols;
	int height = rgb.rows;


	cmyk = Mat(height, width, CV_8UC4);

	Mat out, cmy[3], black, C, M, Y;
	if (IpColorSplitCMY(rgb, out)) {
		split(out, cmy);

		min(cmy[0], cmy[1], black);
		min(black, cmy[2], black);
		C = cmy[0] - black;
		M = cmy[1] - black;
		Y = cmy[2] - black;

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				cmyk.at<Vec4b>(y, x)[0] = (byte)C.at<byte>(y, x);		//C
				cmyk.at<Vec4b>(y, x)[1] = (byte)M.at<byte>(y, x);		//M
				cmyk.at<Vec4b>(y, x)[2] = (byte)Y.at<byte>(y, x);		//Y
				cmyk.at<Vec4b>(y, x)[3] = (byte)black.at<byte>(y, x);	//K
			}
		}
	}

	return true;
}

bool IpConvert8to16(MyIPImage & img, MyIPImage & out)
{
	if (!img.GetPixels() || img.GetDepth() != CV_8U || img.GetChannels() != 1)
		return false;

	int width = img.GetWidth();
	int height = img.GetHeight();
	int depth = img.GetDepth();
	int channel = img.GetChannels();

	if (!out.CreateImage(height, width, CV_16U, 1))
		return false;

	unsigned short** usPixels = (unsigned short**)out.GetPixels();
	BYTE** bPixels = (BYTE**)img.GetPixels();

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			usPixels[y][x] = bPixels[y][x] * 256;
		}
	}

	return true;
}

bool IpResizeNearest(MyIPImage & img, MyIPImage & out, int nh, int nw)
{
	if (!img.GetPixels())	return false;
	if (nh <= 0 || nw <= 0)	return false;

	int w = img.GetWidth();
	int h = img.GetHeight();
	int depth = img.GetDepth();
	int channels = img.GetChannels();

	BYTE **pixel, **opixel;
	WORD **wpixel, **wopixel;
	RGBBYTE **rgbpixel, **rgbopixel;
 
	if (!out.CreateImage(nh, nw, depth, channels)) 
		return false;

	if (depth == CV_8U || depth == CV_8S) {
		if (channels == 1) {
			pixel = (BYTE**)img.GetPixels();
			opixel = (BYTE**)out.GetPixels();

			int i, j, x, y;
			double rx, ry;
			for (j = 0; j < nh; j++) {
				for (i = 0; i < nw; i++) {
					rx = static_cast<double>(w - 1)*i / (nw - 1);
					ry = static_cast<double>(h - 1)*j / (nh - 1);
					x = static_cast<int>(rx + 0.5);
					y = static_cast<int>(ry + 0.5);

					if (x >= w) x = w - 1;
					if (y >= h) y = h - 1;

					opixel[j][i] = pixel[y][x];
				}
			}
		}
		else if (channels == 3) {
			rgbpixel = (RGBBYTE**)img.GetPixels();
			rgbopixel = (RGBBYTE**)out.GetPixels();

			int i, j, x, y;
			double rx, ry;
			for (j = 0; j < nh; j++) {
				for (i = 0; i < nw; i++) {
					rx = static_cast<double>(w - 1)*i / (nw - 1);
					ry = static_cast<double>(h - 1)*j / (nh - 1);
					x = static_cast<int>(rx + 0.5);
					y = static_cast<int>(ry + 0.5);

					if (x >= w) x = w - 1;
					if (y >= h) y = h - 1;

					rgbopixel[j][i].r = rgbpixel[y][x].r;
					rgbopixel[j][i].g = rgbpixel[y][x].g;
					rgbopixel[j][i].b = rgbpixel[y][x].b;
				}
			}
		}
	}
	else if (depth == CV_16U || depth == CV_16S) {
		if (channels == 1) {
			wpixel = (WORD**)img.GetPixels();
			wopixel = (WORD**)out.GetPixels();

			int i, j, x, y;
			double rx, ry;
			for (j = 0; j < nh; j++) {
				for (i = 0; i < nw; i++) {
					rx = static_cast<double>(w - 1)*i / (nw - 1);
					ry = static_cast<double>(h - 1)*j / (nh - 1);
					x = static_cast<int>(rx + 0.5);
					y = static_cast<int>(ry + 0.5);

					if (x >= w) x = w - 1;
					if (y >= h) y = h - 1;


				}
			}
		}
		else if (channels == 3) {

		}
	}

	return true;
}

