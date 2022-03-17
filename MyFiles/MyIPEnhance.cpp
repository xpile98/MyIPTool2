#include "stdafx.h"
#include "MyIPEnhance.h"

bool IP_Inverse(MyIPImage & img)
{
	// 1. image가 잘 들어왔는지 확인 
	if (!img.GetPixels())	return false;

	int width = img.GetWidth();
	int height = img.GetHeight();
	int depth = img.GetDepth();
	int channel = img.GetChannels();
	void** pixel = img.GetPixels();
	byte** pb_pixel = (byte**)pixel;			//8UC1
	WORD** pw_pixel = (WORD**)pixel;			//16UC1
	RGBBYTE** pbRGB_pixel = (RGBBYTE**)pixel;	//8UC3
	RGBWORD** pwRGB_pixel = (RGBWORD**)pixel;	//16UC3
	switch (depth)
	{
	case CV_8U:
		byte LUT[256];
		for (int i = 0; i < 256; i++) {
			LUT[i] = 255 - i;
		}

		switch (channel)
		{
		case 1:
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					pb_pixel[y][x] = LUT[pb_pixel[y][x]];			//LUT사용하는 경우
				}
			}
			break;

		case 3:
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					pbRGB_pixel[y][x].b = LUT[pbRGB_pixel[y][x].b];
					pbRGB_pixel[y][x].g = LUT[pbRGB_pixel[y][x].g];
					pbRGB_pixel[y][x].r = LUT[pbRGB_pixel[y][x].r];
					//		pb_pixel[y][x] = LUT[pb_pixel[y][x]];			//이거하면 영사의 왼쪽 3분의 1만 반전되서 나온당
				}
			}
			break;

		default:
			return false;
		}
		break;

	case CV_16U:
		WORD  LUT_16[65536];
		for (int i = 0; i < 65536; i++) {
			LUT_16[i] = 65535 - i;
		}

		switch (channel)
		{
		case 1:
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width * channel; x++) {
					pw_pixel[y][x] = LUT_16[pw_pixel[y][x]];		//LUT 사용
				}
			}
			break;

		case 3:
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					pwRGB_pixel[y][x].b = LUT_16[pwRGB_pixel[y][x].b];
					pwRGB_pixel[y][x].g = LUT_16[pwRGB_pixel[y][x].g];
					pwRGB_pixel[y][x].r = LUT_16[pwRGB_pixel[y][x].r];
					//		pb_pixel[y][x] = LUT[pb_pixel[y][x]];			//이거하면 영사의 왼쪽 3분의 1만 반전되서 나온당
				}
			}
			break;

		default:
			return false;
		}
		break;

	default:
		return false;
	}
	return true;
}

bool IP_Inverse(Mat & mat)//검증필요
{
	if (!mat.data)	return false;

	int width = mat.cols;
	int height = mat.rows;
	int depth = mat.depth();
	int channel = mat.channels();

	switch (depth)
	{
	case CV_8U:
		byte LUT[256];
		for (int i = 0; i < 256; i++) {
			LUT[i] = 255 - i;
		}

		switch (channel)
		{
		case 1:
		case 2:
		case 3:
		case 4:
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width * channel; x++) {
					mat.at<byte>(y, x) = LUT[mat.at<byte>(y, x)];	//LUT 사용 
				}
			}
			break;

		default:
			return false;
		}
		break;

	case CV_16U:
		WORD LUT_16[65536];
		for (int i = 0; i < 65536; i++) {
			LUT_16[i] = 65535 - i;
		}

		switch (channel)
		{
		case 1:
		case 2:
		case 3:
		case 4:
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width * channel; x++) {
					mat.at<WORD>(y, x) = LUT_16[mat.at<WORD>(y, x)];	//LUT 사용 
				}
			}
			break;

		default:
			return false;
		}
		break;

	default:
		return false;
	}
	return true;
}

bool IP_BrightnessContrast(MyIPImage &img, int offset, float gain)
{
	if (!img.GetPixels())	
		return false;
	if (gain == 0.f)
		return false;
	
	int width = img.GetWidth();
	int height = img.GetHeight();
	int depth = img.GetDepth();
	int channel = img.GetChannels();
	void** pixel = img.GetPixels();
	byte** pb_pixel = (byte**)pixel;			//8UC1
	WORD** pw_pixel = (WORD**)pixel;			//16UC1
	RGBBYTE** pbRGB_pixel = (RGBBYTE**)pixel;	//8UC3
	RGBWORD** pwRGB_pixel = (RGBWORD**)pixel;	//16UC3

	switch (depth)
	{
	case CV_8U:
		byte LUT[256];
		for (int i = 0; i < 256; i++) {
			float fval = ((float)(i + offset)) * gain;

			if (fval > 255)
				LUT[i] = (byte)255;
			else if (fval < 0)
				LUT[i] = (byte)0;
			else
				LUT[i] = (byte)fval;
		}

		switch (channel)
		{
		case 1:
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					pb_pixel[y][x] = LUT[pb_pixel[y][x]];
				}
			}
			break;
		case 2:
		case 3:
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					pbRGB_pixel[y][x].b = LUT[pbRGB_pixel[y][x].b];
					pbRGB_pixel[y][x].g = LUT[pbRGB_pixel[y][x].g];
					pbRGB_pixel[y][x].r = LUT[pbRGB_pixel[y][x].r];
				}
			}
			break;
		case 4:
			break;
		default:
			return false;
		}
		break;

	case CV_16U:
		WORD LUT_16[65536];
		for (int i = 0; i < 65535; i++) {
			float fval = ((float)(i + offset)) * gain;

			if (fval > 65535)
				LUT_16[i] = (WORD)65535;
			else if (fval < 0)
				LUT_16[i] = (WORD)0;
			else
				LUT_16[i] = (WORD)fval;
		}
		
		switch (channel)
		{
		case 1:
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					pw_pixel[y][x] = LUT_16[pw_pixel[y][x]];
				}
			}
			break;
		case 2:
		case 3:
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					pwRGB_pixel[y][x].b = LUT_16[pwRGB_pixel[y][x].b];
					pwRGB_pixel[y][x].g = LUT_16[pwRGB_pixel[y][x].g];
					pwRGB_pixel[y][x].r = LUT_16[pwRGB_pixel[y][x].r];
				}
			}
			break;
		case 4:
		default:
			return false;
		}
		break;

	default:
		return false;
	}

	return true;
}

bool IP_BrightnessContrast(Mat & mat, int offset, float gain)
{
	if (!mat.data)	
		return false;

	if (gain == 0.f)
		return false;

	int width = mat.cols;
	int height = mat.rows;
	int depth = mat.depth();
	int channel = mat.channels();

	switch (depth)
	{
	case CV_8U:
		byte LUT[256];
		for (int i = 0; i < 256; i++) {
			float fval = ((float)(i + offset)) * gain;

			if (fval > 255)
				LUT[i] = (byte)255;
			else if (fval < 0)
				LUT[i] = (byte)0;
			else
				LUT[i] = (byte)fval;
		}

		switch (channel)
		{
		case 1:
		case 2:
		case 3:
		case 4:
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width * channel; x++) {
					mat.at<byte>(y, x) = LUT[mat.at<byte>(y, x)];
				}
			}
			break;

		default:
			return false;
		}
		break;

	case CV_16U:
		WORD LUT_16[65536];
		for (int i = 0; i < 65535; i++) {
			float fval = ((float)(i + offset)) * gain;

			if (fval > 65535)
				LUT_16[i] = (WORD)65535;
			else if (fval < 0)
				LUT_16[i] = (WORD)0;
			else
				LUT_16[i] = (WORD)fval;
		}
		
		switch (channel)
		{
		case 1:
		case 2:
		case 3:
		case 4:
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					mat.at<WORD>(y, x) = LUT_16[mat.at<WORD>(y, x)];
				}
			}
			break;

		default:
			return false;
		}
		break;

	default:
		return false;
	}
	return true;
}

bool IP_GammaCorrection(MyIPImage & img, float gamma)
{
	//1. img 잘 들어왔는지 확인 
	if (!img.GetPixels())	
		return false;
	//2. 감마 0이면 안되요
	if (gamma == 0.f)
		return false;

	//3. LUT 만들기 -> switch case 안에

	//4. 영상 데이터 LUT 이용하여 넣어주기
	int width = img.GetWidth();
	int height = img.GetHeight();
	int depth = img.GetDepth();
	int channel = img.GetChannels();
	void** pixel = img.GetPixels();
	byte** pb_pixel = (byte**)pixel;			//8UC1
	WORD** pw_pixel = (WORD**)pixel;			//16UC1
	RGBBYTE** pbRGB_pixel = (RGBBYTE**)pixel;	//8UC3
	RGBWORD** pwRGB_pixel = (RGBWORD**)pixel;	//16UC3

	switch (depth)
	{
	case CV_8U:
		byte LUT[256];
		for (int i = 0; i < 256; i++) {
			double inv_gamma = 1. / (double)gamma;	//어... 감마에 0 들어오면 안되는데??? -> (2)
			double dval = (pow((i / 255.f), inv_gamma) * 255) + 0.5;

			if (dval < 0)			LUT[i] = (byte)0;
			else if (dval > 255)	LUT[i] = (byte)255;
			else					LUT[i] = (byte)dval;
		}

		switch (channel)
		{
		case 1:
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					pb_pixel[y][x] = LUT[pb_pixel[y][x]];
				}
			}
			break;
		case 2:
		case 3:
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					pbRGB_pixel[y][x].r = LUT[pbRGB_pixel[y][x].r];
					pbRGB_pixel[y][x].g = LUT[pbRGB_pixel[y][x].g];
					pbRGB_pixel[y][x].b = LUT[pbRGB_pixel[y][x].b];
				}
			}
			break;
		case 4:
			break;

		default:
			return false;
		}
		break;

	case CV_16U:
		WORD LUT_16[65536];
		for (int x = 0; x < 65536; x++) {
			double inv_gamma = 1. / (double)gamma;	//어... 감마에 0 들어오면 안되는데??? -> (2)
			double dval = (pow((x / 65535.f), inv_gamma) * 65535) + 0.5;

			if (dval < 0)			LUT_16[x] = (WORD)0;
			else if (dval > 65535)	LUT_16[x] = (WORD)65535;
			else					LUT_16[x] = (WORD)dval;
		}
	
		switch (channel)
		{
		case 1:
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					pw_pixel[y][x] = LUT_16[pw_pixel[y][x]];
				}
			}
			break;
		case 2:
		case 3:
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					pwRGB_pixel[y][x].r = LUT_16[pwRGB_pixel[y][x].r];
					pwRGB_pixel[y][x].g = LUT_16[pwRGB_pixel[y][x].g];
					pwRGB_pixel[y][x].b = LUT_16[pwRGB_pixel[y][x].b];
				}
			}
			   break;
		case 4:
			break;

		default:
			return false;
		}
		break;

	default:
		return false;
	}

	return true;
}

bool IP_GammaCorrection(Mat & mat, float gamma)
{
	//1. img 잘 들어왔는지 확인 
	if (!mat.data)
		return false;

	//2. 감마 0이면 안되요
	if (gamma == 0.f)
		return false;

	//3. LUT 만들기 -> switch case 안에

	//4. 영상 데이터 LUT 이용하여 넣어주기
	int width = mat.cols;
	int height = mat.rows;
	int depth = mat.depth();
	int channel = mat.channels();

	switch (depth)
	{
	case CV_8U:
		byte LUT[256];
		for (int i = 0; i < 256; i++) {
			double inv_gamma = 1. / (double)gamma;	//어... 감마에 0 들어오면 안되는데??? -> (2)
			double dval = (pow((i / 255.f), inv_gamma) * 255) + 0.5;

			if (dval < 0)			LUT[i] = (byte)0;
			else if (dval > 255)	LUT[i] = (byte)255;
			else					LUT[i] = (byte)dval;
		}

		switch (channel)
		{
		case 1:
		case 2:
		case 3:
		case 4:
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width * channel; x++) {
					mat.at<byte>(y, x) = LUT[mat.at<byte>(y, x)];
				}
			}
			break;

		default:
			return false;
		}
		break;

	case CV_16U:
		WORD LUT_16[65536];
		for (int i = 0; i < 65536; i++) {
			double inv_gamma = 1. / (double)gamma;	//어... 감마에 0 들어오면 안되는데??? -> (2)
			double dval = (pow((i / 65535.f), inv_gamma) * 65535) + 0.5;

			if (dval < 0)			LUT_16[i] = (WORD)0;
			else if (dval > 65535)	LUT_16[i] = (WORD)65535;
			else					LUT_16[i] = (WORD)dval;
		}

		switch (channel)
		{
		case 1:
		case 2:
		case 3:
		case 4:
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width * channel; x++) {
					mat.at<WORD>(y, x) = LUT[mat.at<WORD>(y, x)];
				}
			}
			break;

		default:
			return false;
		}
		break;

	default:
		return false;
	}

	return true;
}

bool IP_Histogram(MyIPImage &img, float *histo)
{
	if (!img.GetPixels())
		return false;

	int width = img.GetWidth();
	int height = img.GetHeight();
	int depth = img.GetDepth();
	int channel = img.GetChannels();
	void** pixel = img.GetPixels();
	BYTE** pb_pixel = (BYTE**)pixel;
	WORD** pw_pixel = (WORD**)pixel;//추가

// 	int cnt[256];
// 	int cnt_16[65536];	//추가
	int *cnt;
	switch (depth)
	{
	case CV_8U:
		cnt = new int[256];
		if (!cnt) return false;

		memset(cnt, 0, sizeof(int) * 256);
		//히스토그램 계산
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				int n = ((int)pb_pixel[y][x]);
				cnt[n]++;
			}
		}
		//히스토그램 정규화
		for (int i = 0; i < 256; i++)
			histo[i] = static_cast<float>(cnt[i]) / (width*height);

		delete[] cnt;
		break;
	case CV_16U:
		cnt = new int[65536];
		if (!cnt) return false;
		memset(cnt, 0, sizeof(int) * 65536);	//추가 swith case 로 
		//히스토그램 계산
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				int n = ((int)pw_pixel[y][x]);
				cnt[n]++;
			}
		}
		//히스토그램 정규화
		for (int i = 0; i < 65536; i++)
			histo[i] = static_cast<float>(cnt[i]) / (width*height);
		delete[] cnt;
		break;
	default:
		break;
	}
	return true;
}

bool IP_Histogram(Mat & mat, Mat & himg)
{
	if (!mat.data) 
		return false;

	if (mat.channels() != 1) 
		return false;

	if ((mat.depth() != CV_8U) && (mat.depth() != CV_16U)) 
		return false;

	Mat hist;
	int depth = mat.depth();
	int channels[] = { 0 };

	if (depth == CV_8U)
	{
		int histSize[] = { 256 };
		float range[] = { 0.f, 256.f };
		const float* ranges[] = { range };	//주소값을 넣어줌~

		calcHist(&mat, 1, channels, Mat(), hist, 1, histSize, ranges);
	}
	else if (depth == CV_16U) {
		int histSize[] = { 65536 };
		float range[] = { 0.f, 65536.f };
		const float* ranges[] = { range };	//주소값을 넣어줌~

		calcHist(&mat, 1, channels, Mat(), hist, 1, histSize, ranges);
	}

	
	//최대, 최소값 구하기
	double hmin, hmax;
	Point minloc, maxloc;
	minMaxLoc(hist, &hmin, &hmax, &minloc, &maxloc);
	//평균 구하기
	Scalar mean_val = mean(mat);

	//16비트 영상인 경우 0~65535 -> 0~255로 변환 
	if (depth == CV_16U) {
		Mat temp = hist;

 		float *ptemp = temp.ptr<float>(0);
 		float *phist = hist.ptr<float>(0);
		for (int i = 0; i < 65536; i++) {
			//temp.at<float>(i/256, 0) += hist.at<float>(i,0);	
			*(ptemp+(i/256)) += *(phist+i);
		}
		hist = temp;
	}

	//히스토그램 배경 그리기
	himg = Mat(400, 400, CV_8UC3, Scalar(255,255,255));	//400.400하얀 영상으로 만들기 완료 
	normalize(hist, hist, 0, 300, NORM_MINMAX);

	//>>외각 사각형 그리기
	float x_offset = 30;
	float y_offset = 10;
	Point2f corner1(x_offset, y_offset);
	Point2f corner2(x_offset + 255.f, y_offset + 300.f);
	rectangle(himg, corner1, corner2, Scalar(125, 125, 125), 0);

	//>>그래프 그리기 
	float *phist2 = hist.ptr<float>(0);
	for (int i = 0; i < 256; i++) {
		float start_x = (float)i + x_offset;
		float end_x = i + 1 + x_offset;
		//float *phist = hist.ptr<float>(i);	//for 문 밖으로 빼기 
		Point2f pt1(start_x, 0 + y_offset);
		//Point2f pt2(end_x, hist.at<float>(i, 0) + y_offset);	// 190409 YWCho Mat.at -> Mat.ptr 
		Point2f pt2(end_x, *(phist2 + i)+y_offset);
		rectangle(himg, pt1, pt2, Scalar(0,0,0), -1);
	}
	flip(himg, himg, 0);
	
	//최대,최소값, 평균 글씨쓰기
	char str[80];
	sprintf_s(str, "Max Val(%d : %d), Average: %f", maxloc.y, (int)hmax, mean_val[0]);
	putText(himg, str, Point(20, 20), FONT_HERSHEY_PLAIN, 0.8, g_MyFunction.RED);

	return true;
}

bool IP_Histogram(MyIPImage & img, unsigned int * histo)
{
	if (!img.GetPixels())
		return false;

	int width = img.GetWidth();
	int height = img.GetHeight();
	int depth = img.GetDepth();
	int channel = img.GetChannels();
	void** pixel = img.GetPixels();
	BYTE** pb_pixel = (BYTE**)pixel;
	WORD** pw_pixel = (WORD**)pixel;//추가

	int *cnt;
	switch (depth)
	{
	case CV_8U:
		cnt = new int[256];
		if (!cnt) return false;

		memset(cnt, 0, sizeof(int) * 256);
		//히스토그램 계산
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				int n = ((int)pb_pixel[y][x]);
				cnt[n]++;
			}
		}
		//히스토그램 정규화
		for (int i = 0; i < 256; i++)
			histo[i] = (cnt[i]);

		delete[] cnt;
		break;
	case CV_16U:
		cnt = new int[65536];
		if (!cnt) return false;
		memset(cnt, 0, sizeof(int) * 65536);	//추가 swith case 로 
												//히스토그램 계산
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				int n = ((int)pw_pixel[y][x]);
				cnt[n]++;
			}
		}
		//히스토그램 정규화
		for (int i = 0; i < 65536; i++)
			histo[i] = (cnt[i]);
		delete[] cnt;
		break;
	default:
		break;
	}
	return true;
}

/*bool IP_Histogram(MyIPImage & img, float histo[65536])
{
	if (!img.GetPixels())
		return false;

	int width = img.GetWidth();
	int height = img.GetHeight();
	int depth = img.GetDepth();
	int channel = img.GetChannels();
	void** pixel = img.GetPixels();
	WORD** pw_pixel = (WORD**)pixel;

	//히스토그램 계산
	int cnt[65536];
	memset(cnt, 0, sizeof(int) * 65536);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int n = ((int)pw_pixel[y][x]);
			cnt[n]++;
		}
	}

	//히스토그램 정규화
	for (int i = 0; i < 65536; i++)
		histo[i] = static_cast<float>(cnt[i]) / (width*height);

	return true;
}*/


bool IP_HistogramStretching(MyIPImage & img)
{
	if (!img.GetPixels())
		return false;

	int width = img.GetWidth();
	int height = img.GetHeight();
	int depth = img.GetDepth();
	int channel = img.GetChannels();
	BYTE** pb_pixel = (BYTE**)img.GetPixels();
	WORD** pw_pixel = (WORD**)img.GetPixels();

	//최대, 최소 그레이스케일 값 계산
	BYTE gray_max, gray_min;
	WORD gray_max_16, gray_min_16;
	gray_max = gray_min = pb_pixel[0][0];
	gray_max_16 = gray_min_16 = pw_pixel[0][0];

	switch (depth)
	{
	case CV_8U:
	case CV_8S:
		//최대, 최소값 구하기
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				if (gray_max < pb_pixel[y][x])	gray_max = pb_pixel[y][x];
				if (gray_min > pb_pixel[y][x])	gray_min = pb_pixel[y][x];
			}
		}

		if (gray_max == gray_min)
			return false;

		//히스토그램 스트레칭
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				pb_pixel[y][x] = (pb_pixel[y][x] - gray_min) * 255 / (gray_max - gray_min);
			}
		}
		break;

	case CV_16U:
	case CV_16S:
		//최대, 최소값 구하기
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				if (gray_max_16 < pw_pixel[y][x])	gray_max_16 = pw_pixel[y][x];
				if (gray_min_16 > pw_pixel[y][x])	gray_min_16 = pw_pixel[y][x];
			}
		}

		if (gray_max_16 == gray_min_16)
			return false;

		//히스토그램 스트레칭
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				//pw_pixel[y][x] = (WORD)((unsigned int)((pw_pixel[y][x] - gray_min_16) * 65536) / (gray_max_16 - gray_min_16));	//unsigned int 변환하지 않으면 범위 오버되는 부분 생김
				pw_pixel[y][x] = (pw_pixel[y][x] - gray_min_16) * (unsigned int)65535 / (gray_max_16 - gray_min_16);
			}
		}
		break;
	default:
		return false;
	}
	return true;
}

bool IP_HistogramEqualization(MyIPImage & img)
{
	if (!img.GetPixels())
		return false;

	int width = img.GetWidth();
	int height = img.GetHeight();
	int depth = img.GetDepth();
	int channel = img.GetChannels();
	void** pixel = img.GetPixels();
	BYTE** pb_pixel = (BYTE**)pixel;
	WORD** pw_pixel = (WORD**)pixel;

	//히스토그램 계산
// 	float hist[256];
// 	float hist_16[65536];
// 	float cdf[256] = { 0.0, };
// 	float cdf_16[65536] = { 0.0, };
	float *hist;
	float *cdf;
	
	switch (depth)
	{
	case CV_8U:
		//동적 할당
		hist = new float[256];
		if (!hist)	return false;
		cdf = new float[256];
		if (!cdf)	return false;
		memset(cdf, 0, sizeof(float) * 256);


		//히스토그램 구하기
		IP_Histogram(img, hist);

		//히스토그램 누적 함수 계산
		cdf[0] = hist[0];
		for (int i = 1; i < 256; i++)
			cdf[i] = cdf[i - 1] + hist[i];

		//히스토그램 균등화
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				BYTE n = (BYTE)pb_pixel[y][x];
				pb_pixel[y][x] = static_cast<BYTE>(limit(cdf[n] * 255));
			}
		}
		break;
	case CV_16U:
		//동적 할당
		hist = new float[65536];
		if (!hist)	return false;
		cdf = new float[65536];
		if (!cdf)	return false;
		memset(cdf, 0, sizeof(float) * 65536);

		//히스토그램 구하기
		IP_Histogram(img, hist);

		//히스토그램 누적 함수 계산
		cdf[0] = hist[0];
		for (int i = 1; i < 65536; i++)
			cdf[i] = cdf[i - 1] + hist[i];

		//히스토그램 균등화
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				WORD n = (WORD)pw_pixel[y][x];
				pw_pixel[y][x] = static_cast<WORD>(limit16bit(cdf[n] * 65536));		//limit쓰면 값 오버되면 255로 바꿔주니 새로만든 limit16bit 사용 
			}
		}
		break;
	default:
		break;
	}
	return true;
}

bool IP_Mean_StandardDeviation(MyIPImage & img)
{
	if (!img.GetPixels())
		return false;

	int width = img.GetWidth();
	int height = img.GetHeight();
	int depth = img.GetDepth();
	int channel = img.GetChannels();
	void** pixel = img.GetPixels();
	BYTE** pb_pixel = (BYTE**)pixel;
	WORD** pw_pixel = (WORD**)pixel;

	unsigned int sum = 0;
	double sum2 = 0;
	double mean(0), sd(0);

	switch (depth)
	{
	case CV_8U:
	case CV_8S:
		//평균 구하기(모두 더하기)
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				sum += pb_pixel[y][x];
			}
		}
		mean = static_cast<double>(sum) / (width*height);

		//표준편차 구하기 
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				sum2 += pow((pb_pixel[y][x] - mean), 2);
			}
		}
		sum2 /= (width*height);
		sd = (double)sqrt(sum2);
		break;

	case CV_16U:
	case CV_16S:
		//평균 구하기(모두 더하기)
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				sum += pw_pixel[y][x];
			}
		}
		mean = static_cast<double>(sum) / (width*height);

		//표준편차 구하기 
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				sum2 += pow((pw_pixel[y][x] - mean), 2);
			}
		}
		sum2 /= (width*height);
		sd = (double)sqrt(sum2);
		break;

	default:
		break;
	}


	


	img.SetMean(mean);
	img.SetStddDest(sd);

	return true;
}

bool IP_ARLogic(MyIPImage & img1, MyIPImage & img2, MyIPImage & oimg, int op, bool useConstant, int Constant, double dConst)
{
	if (!img1.GetPixels() || !img2.GetPixels())		return false;
	if (img1.GetDepth() != img2.GetDepth())			return false;
	if (img1.GetChannels() != img2.GetChannels())	return false;
	if (img1.GetHeight() != img2.GetHeight())		return false;
	if (img1.GetWidth() != img2.GetWidth())			return false;

	int depth = img1.GetDepth();
	int channels = img1.GetChannels();
	int width = img1.GetWidth();
	int height = img1.GetHeight();
	if (!oimg.CreateImage(height, width, depth, channels))	return false;

	int result;
	BYTE **Pixel1, **Pixel2, **PixelO;
	WORD **wPixel1, **wPixel2, **wPixelO;

	int resultR, resultG, resultB;
	RGBBYTE **rgbPixel1, **rgbPixel2, **rgbPixelO;
	if (depth == CV_8U || depth == CV_8S) {	//8비트
		if (channels == 1) {
			Pixel1 = (byte**)img1.GetPixels();
			Pixel2 = (byte**)img2.GetPixels();
			PixelO = (byte**)oimg.GetPixels();
		}
		else if (channels == 3) {
			rgbPixel1 = (RGBBYTE**)img1.GetPixels();
			rgbPixel2 = (RGBBYTE**)img2.GetPixels();
			rgbPixelO = (RGBBYTE**)oimg.GetPixels();
		}
	}
	else {									//16비트
		wPixel1 = (WORD**)img1.GetPixels();
		wPixel2 = (WORD**)img2.GetPixels();
		wPixelO = (WORD**)oimg.GetPixels();
	}

	switch (op)
	{
	case IOP_ADD:
		switch (depth)
		{
		case CV_8U:
		case CV_8S:
			if (channels == 1) {
				for (int y = 0; y < height; y++) {
					for (int x = 0; x < width; x++) {
						//>>Constant
						if (useConstant == false)	//Constant 미사용 
							result = (int)Pixel1[y][x] + (int)Pixel2[y][x];
						else
							result = (int)Pixel1[y][x] + Constant;
						//<<
						if (result > 255)	PixelO[y][x] = (byte)255;
						else PixelO[y][x] = (byte)result;
					}
				}
			}
			else if (channels == 3) {
				for (int y = 0; y < height; y++) {
					for (int x = 0; x < width; x++) {
						//>>Constant
						if (useConstant == false) {	//Constant 미사용 
							resultR = (int)rgbPixel1[y][x].r + (int)rgbPixel2[y][x].r;
							resultG = (int)rgbPixel1[y][x].g + (int)rgbPixel2[y][x].g;
							resultB = (int)rgbPixel1[y][x].b + (int)rgbPixel2[y][x].b;
						}
						else {
							resultR = (int)rgbPixel1[y][x].r + Constant;
							resultG = (int)rgbPixel1[y][x].g + Constant;
							resultB = (int)rgbPixel1[y][x].b + Constant;
						}
						//<<
						if (resultR > 255)	rgbPixelO[y][x].r = (byte)255;
						else rgbPixelO[y][x].r = (byte)resultR;
						if (resultG > 255)	rgbPixelO[y][x].g = (byte)255;
						else rgbPixelO[y][x].g = (byte)resultG;
						if (resultB > 255)	rgbPixelO[y][x].b = (byte)255;
						else rgbPixelO[y][x].b = (byte)resultB;
					}
				}
			}
			break;

		case CV_16U:
		case CV_16S:
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					//>>Constant
					if (useConstant == false)	//Constant 미사용 
						result = (int)wPixel1[y][x] + (int)wPixel2[y][x];
					else
						result = (int)wPixel1[y][x] + Constant;
					//<<
					if (result > 65535)	wPixelO[y][x] = (WORD)65535;
					else wPixelO[y][x] = (WORD)result;
				}
			}
			break;

		default:
			return false;
		}
		break;
	case IOP_SUB:
		switch (depth)
		{
		case CV_8U:
		case CV_8S:
			if (channels == 1) {
				for (int y = 0; y < height; y++) {
					for (int x = 0; x < width; x++) {
						//>>Constant
						if (useConstant == false)	//Constant 미사용 
							result = limit(Pixel1[y][x] - Pixel2[y][x]);
						else
							result = (int)Pixel1[y][x] - Constant;
						//<<
						if (result < 0)	PixelO[y][x] = (byte)0;
						else PixelO[y][x] = (byte)result;
					}
				}
			}
			else if (channels == 3) {
				for (int y = 0; y < height; y++) {
					for (int x = 0; x < width; x++) {
						//>>Constant
						if (useConstant == false) {	//Constant 미사용 
							resultR = limit(rgbPixel1[y][x].r - rgbPixel2[y][x].r);
							resultG = limit(rgbPixel1[y][x].g - rgbPixel2[y][x].g);
							resultB = limit(rgbPixel1[y][x].b - rgbPixel2[y][x].b);
						}
						else {
							resultR = (int)rgbPixel1[y][x].r - Constant;
							resultG = (int)rgbPixel1[y][x].g - Constant;
							resultB = (int)rgbPixel1[y][x].b - Constant;
						}
						//<<
						if (resultR < 0)	rgbPixelO[y][x].r = (byte)0;
						else rgbPixelO[y][x].r = (byte)resultR;
						if (resultG < 0)	rgbPixelO[y][x].g = (byte)0;
						else rgbPixelO[y][x].g = (byte)resultG;
						if (resultB < 0)	rgbPixelO[y][x].b = (byte)0;
						else rgbPixelO[y][x].b = (byte)resultB;
					}
				}
			}
			break;

		case CV_16U:
		case CV_16S:
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					//>>Constant
					if (useConstant == false)	//Constant 미사용 
						result = limit16bit(wPixel1[y][x] - wPixel2[y][x]);
					else
						result = (int)wPixel1[y][x] - Constant;
					//<<
					if (result < 0)	wPixelO[y][x] = (WORD)0;
					else wPixelO[y][x] = (WORD)result;
				}
			}
			break;

		default:
			return false;
		}
		break;
	case IOP_AVR:
		switch (depth)
		{
		case CV_8U:
		case CV_8S:
			if (channels == 1) {
				for (int y = 0; y < height; y++) {
					for (int x = 0; x < width; x++) {
						//>>Constant
						if (useConstant == false) {	//Constant 미사용 
							result = Pixel1[y][x] + Pixel2[y][x];
							PixelO[y][x] = (result + 1) / 2;
						}
						else {
							result = Pixel1[y][x] + Constant;
							PixelO[y][x] = (result + 1) / 2;
						}
						//<<
					}
				}
			}
			else if (channels == 3) {
				for (int y = 0; y < height; y++) {
					for (int x = 0; x < width; x++) {
						//>>Constant
						if (useConstant == false) {	//Constant 미사용 
							resultR = rgbPixel1[y][x].r + rgbPixel2[y][x].r;
							resultG = rgbPixel1[y][x].g + rgbPixel2[y][x].g;
							resultB = rgbPixel1[y][x].b + rgbPixel2[y][x].b;
							rgbPixelO[y][x].r = (resultR + 1) / 2;
							rgbPixelO[y][x].g = (resultG + 1) / 2;
							rgbPixelO[y][x].b = (resultB + 1) / 2;
						}
						else {
							resultR = rgbPixel1[y][x].r + Constant;
							resultG = rgbPixel1[y][x].g + Constant;
							resultB = rgbPixel1[y][x].b + Constant;
							rgbPixelO[y][x] = (resultR + 1) / 2;
							rgbPixelO[y][x] = (resultG + 1) / 2;
							rgbPixelO[y][x] = (resultB + 1) / 2;
						}
						//<<
					}
				}
			}
			break;

		case CV_16U:
		case CV_16S:
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					//>>Constant
					if (useConstant == false)	//Constant 미사용 
						wPixelO[y][x] = (WORD)(((int)wPixel1[y][x] + (int)wPixel2[y][x] + 1) / 2);
					else
						wPixelO[y][x] = (WORD)(((int)wPixel1[y][x] + Constant + 1) / 2);
					//<<
				}
			}
			break;

		default:
			return false;
		}
		break;
	case IOP_DIF:
		switch (depth)
		{
		case CV_8U:
		case CV_8S:
			if (channels == 1) {
				for (int y = 0; y < height; y++) {
					for (int x = 0; x < width; x++) {
						//>>Constant
						if (useConstant == false) {	//Constant 미사용 
							result = (int)Pixel1[y][x] - (int)Pixel2[y][x];
							PixelO[y][x] = static_cast<BYTE>((result >= 0) ? result : -result);
						}
						else {
							result = (int)Pixel1[y][x] - Constant;
							PixelO[y][x] = static_cast<BYTE>((result >= 0) ? result : -result);
						}
						//<<
					}
				}
			}
			else if (channels == 3) {
				for (int y = 0; y < height; y++) {
					for (int x = 0; x < width; x++) {
						//>>Constant
						if (useConstant == false) {	//Constant 미사용 
							resultR = (int)rgbPixel1[y][x].r - (int)rgbPixel2[y][x].r;
							rgbPixelO[y][x].r = static_cast<BYTE>((resultR >= 0) ? resultR : -resultR);
							resultG = (int)rgbPixel1[y][x].g - (int)rgbPixel2[y][x].g;
							rgbPixelO[y][x].g = static_cast<BYTE>((resultG >= 0) ? resultG : -resultG);
							resultB = (int)rgbPixel1[y][x].b - (int)rgbPixel2[y][x].b;
							rgbPixelO[y][x].b = static_cast<BYTE>((resultB >= 0) ? resultB : -resultB);
						}
						else {
							resultR = (int)rgbPixel1[y][x].r - Constant;
							rgbPixelO[y][x].r = static_cast<BYTE>((resultR >= 0) ? resultR : -resultR);
							resultG = (int)rgbPixel1[y][x].g - Constant;
							rgbPixelO[y][x].g = static_cast<BYTE>((resultG >= 0) ? resultG : -resultG);
							resultB = (int)rgbPixel1[y][x].b - Constant;
							rgbPixelO[y][x].b = static_cast<BYTE>((resultB >= 0) ? resultB : -resultB);
						}
						//<<
					}
				}
			}
			break;

		case CV_16U:
		case CV_16S:
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					//>>Constant
					if (useConstant == false) {	//Constant 미사용 
						result = (int)wPixel1[y][x] - (int)wPixel2[y][x];
						wPixelO[y][x] = static_cast<WORD>((result >= 0) ? result : -result);
					}
					else {
						result = (int)wPixel1[y][x] - Constant;
						wPixelO[y][x] = static_cast<WORD>((result >= 0) ? result : -result);
					}
					//<<
				}
			}
			break;

		default:
			return false;
		}
		break;
	case IOP_AND:
		switch (depth)
		{
		case CV_8U:
		case CV_8S:
			if (channels == 1) {
				for (int y = 0; y < height; y++) {
					for (int x = 0; x < width; x++) {
						//>>Constant
						if (useConstant == false)	//Constant 미사용 
							PixelO[y][x] = static_cast<BYTE>(Pixel1[y][x] & Pixel2[y][x]);
						else
							PixelO[y][x] = static_cast<BYTE>(Pixel1[y][x] & Constant);
						//<<
					}
				}
			}
			else if (channels == 3) {
				for (int y = 0; y < height; y++) {
					for (int x = 0; x < width; x++) {
						//>>Constant
						if (useConstant == false) {	//Constant 미사용 
							rgbPixelO[y][x].r = static_cast<BYTE>(rgbPixel1[y][x].r & rgbPixel2[y][x].r);
							rgbPixelO[y][x].g = static_cast<BYTE>(rgbPixel1[y][x].g & rgbPixel2[y][x].g);
							rgbPixelO[y][x].b = static_cast<BYTE>(rgbPixel1[y][x].b & rgbPixel2[y][x].b);
						}
						else {
							rgbPixelO[y][x].r = static_cast<BYTE>(rgbPixel1[y][x].r & Constant);
							rgbPixelO[y][x].g = static_cast<BYTE>(rgbPixel1[y][x].g & Constant);
							rgbPixelO[y][x].b = static_cast<BYTE>(rgbPixel1[y][x].b & Constant);
						}
						//<<
					}
				}
			}
			break;

		case CV_16U:
		case CV_16S:
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					//>>Constant
					if (useConstant == false)	//Constant 미사용 
						wPixelO[y][x] = static_cast<WORD>(wPixel1[y][x] & wPixel2[y][x]);
					else
						wPixelO[y][x] = static_cast<WORD>(wPixel1[y][x] & Constant);
					//<<
				}
			}
			break;

		default:
			return false;
		}
		break;
	case IOP_OR:
		switch (depth)
		{
		case CV_8U:
		case CV_8S:
			if (channels == 1) {
				for (int y = 0; y < height; y++) {
					for (int x = 0; x < width; x++) {
						//>>Constant
						if (useConstant == false)	//Constant 미사용 
							PixelO[y][x] = static_cast<BYTE>(Pixel1[y][x] | Pixel2[y][x]);
						else
							PixelO[y][x] = static_cast<BYTE>(Pixel1[y][x] | Constant);
						//<<
					}
				}
			}
			else if (channels == 3) {
				for (int y = 0; y < height; y++) {
					for (int x = 0; x < width; x++) {
						//>>Constant
						if (useConstant == false) {	//Constant 미사용 
							rgbPixelO[y][x].r = static_cast<BYTE>(rgbPixel1[y][x].r | rgbPixel2[y][x].r);
							rgbPixelO[y][x].g = static_cast<BYTE>(rgbPixel1[y][x].g | rgbPixel2[y][x].g);
							rgbPixelO[y][x].b = static_cast<BYTE>(rgbPixel1[y][x].b | rgbPixel2[y][x].b);
						}
						else {
							rgbPixelO[y][x].r = static_cast<BYTE>(rgbPixel1[y][x].r | Constant);
							rgbPixelO[y][x].g = static_cast<BYTE>(rgbPixel1[y][x].g | Constant);
							rgbPixelO[y][x].b = static_cast<BYTE>(rgbPixel1[y][x].b | Constant);
						}
						//<<
					}
				}
			}
			break;

		case CV_16U:
		case CV_16S:
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					//>>Constant
					if (useConstant == false)	//Constant 미사용 
						wPixelO[y][x] = static_cast<WORD>(wPixel1[y][x] | wPixel2[y][x]);
					else
						wPixelO[y][x] = static_cast<WORD>(wPixel1[y][x] | Constant);
					//<<
				}
			}
			break;

		default:
			return false;
		}
		break;
	case IOP_MIN:
		switch (depth)
		{
		case CV_8U:
		case CV_8S:
			if (channels == 1) {
				for (int y = 0; y < height; y++) {
					for (int x = 0; x < width; x++) {
						//>>Constant
						if (useConstant == false) {	//Constant 미사용 
							if (Pixel1[y][x] < Pixel2[y][x])	PixelO[y][x] = Pixel1[y][x];
							else PixelO[y][x] = Pixel2[y][x];
						}
						else {
							if (Pixel1[y][x] < Constant)	PixelO[y][x] = Pixel1[y][x];
							else PixelO[y][x] = Constant;
						}
						//<<
					}
				}
			}
			else if (channels == 3) {
				for (int y = 0; y < height; y++) {
					for (int x = 0; x < width; x++) {
						//>>Constant
						if (useConstant == false) {	//Constant 미사용 
							if (rgbPixel1[y][x].r < rgbPixel2[y][x].r)	rgbPixelO[y][x].r = rgbPixel1[y][x].r;
							else rgbPixelO[y][x].r = rgbPixel2[y][x].r;
							if (rgbPixel1[y][x].g < rgbPixel2[y][x].g)	rgbPixelO[y][x].g = rgbPixel1[y][x].g;
							else rgbPixelO[y][x].g = rgbPixel2[y][x].g;
							if (rgbPixel1[y][x].b < rgbPixel2[y][x].b)	rgbPixelO[y][x].b = rgbPixel1[y][x].b;
							else rgbPixelO[y][x].b = rgbPixel2[y][x].b;
						}
						else {
							if (rgbPixel1[y][x].r < Constant)	rgbPixelO[y][x].r = rgbPixel1[y][x].r;
							else rgbPixelO[y][x].r = Constant;
							if (rgbPixel1[y][x].g < Constant)	rgbPixelO[y][x].g = rgbPixel1[y][x].g;
							else rgbPixelO[y][x].g = Constant;
							if (rgbPixel1[y][x].b < Constant)	rgbPixelO[y][x].b = rgbPixel1[y][x].b;
							else rgbPixelO[y][x].b = Constant;
						}
						//<<
					}
				}
			}
			break;

		case CV_16U:
		case CV_16S:
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					//>>Constant
					if (useConstant == false) {	//Constant 미사용 
						if (wPixel1[y][x] < wPixel2[y][x])	wPixelO[y][x] = wPixel1[y][x];
						else wPixelO[y][x] = wPixel2[y][x];
					}
					else {
						if (wPixel1[y][x] < Constant)	wPixelO[y][x] = wPixel1[y][x];
						else wPixelO[y][x] = Constant;
					}
					//<<
				}
			}
			break;

		default:
			return false;
		}
		break;
	case IOP_MAX:
		switch (depth)
		{
		case CV_8U:
		case CV_8S:
			if (channels == 1) {
				for (int y = 0; y < height; y++) {
					for (int x = 0; x < width; x++) {
						//>>Constant
						if (useConstant == false) {	//Constant 미사용 
							if (Pixel1[y][x] > Pixel2[y][x])	PixelO[y][x] = Pixel1[y][x];
							else PixelO[y][x] = Pixel2[y][x];
						}
						else {
							if (Pixel1[y][x] > Constant)	PixelO[y][x] = Pixel1[y][x];
							else PixelO[y][x] = Constant;
						}
						//<<
					}
				}
			}
			else if (channels == 3) {
				for (int y = 0; y < height; y++) {
					for (int x = 0; x < width; x++) {
						//>>Constant
						if (useConstant == false) {	//Constant 미사용 
							if (rgbPixel1[y][x].r > rgbPixel2[y][x].r)	rgbPixelO[y][x].r = rgbPixel1[y][x].r;
							else rgbPixelO[y][x].r = rgbPixel2[y][x].r;
							if (rgbPixel1[y][x].g > rgbPixel2[y][x].g)	rgbPixelO[y][x].g = rgbPixel1[y][x].g;
							else rgbPixelO[y][x].g = rgbPixel2[y][x].g;
							if (rgbPixel1[y][x].b > rgbPixel2[y][x].b)	rgbPixelO[y][x].b = rgbPixel1[y][x].b;
							else rgbPixelO[y][x].b = rgbPixel2[y][x].b;
						}
						else {
							if (rgbPixel1[y][x].r > Constant)	rgbPixelO[y][x].r = rgbPixel1[y][x].r;
							else rgbPixelO[y][x].r = Constant;
							if (rgbPixel1[y][x].g > Constant)	rgbPixelO[y][x].g = rgbPixel1[y][x].g;
							else rgbPixelO[y][x].g = Constant;
							if (rgbPixel1[y][x].b > Constant)	rgbPixelO[y][x].b = rgbPixel1[y][x].b;
							else rgbPixelO[y][x].b = Constant;
						}
						//<<
					}
				}
			}
			break;

		case CV_16U:
		case CV_16S:
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					//>>Constant
					if (useConstant == false) {	//Constant 미사용 
						if (wPixel1[y][x] > wPixel2[y][x])	wPixelO[y][x] = wPixel1[y][x];
						else wPixelO[y][x] = wPixel2[y][x];
					}
					else {
						if (wPixel1[y][x] > Constant)	wPixelO[y][x] = wPixel1[y][x];
						else wPixelO[y][x] = Constant;
					}
					//<<
				}
			}
			break;

		default:
			return false;
		}
		break;
	case IOP_XOR:
		switch (depth)
		{
		case CV_8U:
		case CV_8S:
			if (channels == 1) {
				for (int y = 0; y < height; y++) {
					for (int x = 0; x < width; x++) {
						//>>Constant
						if (useConstant == false)	//Constant 미사용 
							PixelO[y][x] = static_cast<BYTE>(Pixel1[y][x] ^ Pixel2[y][x]);
						else
							PixelO[y][x] = static_cast<BYTE>(Pixel1[y][x] ^ Constant);
						//<<
					}
				}
			}
			else if (channels == 3) {
				for (int y = 0; y < height; y++) {
					for (int x = 0; x < width; x++) {
						//>>Constant
						if (useConstant == false) {	//Constant 미사용 
							rgbPixelO[y][x].r = static_cast<BYTE>(rgbPixel1[y][x].r ^ rgbPixel2[y][x].r);
							rgbPixelO[y][x].g = static_cast<BYTE>(rgbPixel1[y][x].g ^ rgbPixel2[y][x].g);
							rgbPixelO[y][x].b = static_cast<BYTE>(rgbPixel1[y][x].b ^ rgbPixel2[y][x].b);
						}
						else {
							rgbPixelO[y][x] = static_cast<BYTE>(rgbPixel1[y][x].r ^ Constant);
							rgbPixelO[y][x] = static_cast<BYTE>(rgbPixel1[y][x].g ^ Constant);
							rgbPixelO[y][x] = static_cast<BYTE>(rgbPixel1[y][x].b ^ Constant);
						}
							//<<
					}
				}
			}
			break;

		case CV_16U:
		case CV_16S:
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					//>>Constant
					if (useConstant == false)	//Constant 미사용 
						wPixelO[y][x] = static_cast<WORD>(wPixel1[y][x] ^ wPixel2[y][x]);
					else
						wPixelO[y][x] = static_cast<WORD>(wPixel1[y][x] ^ Constant);
					//<<
				}
			}
			break;

		default:
			return false;
		}
		break;
	case IOP_MUL:
		switch (depth)
		{
		case CV_8U:
		case CV_8S:
			if (channels == 1) {
				for (int y = 0; y < height; y++) {
					for (int x = 0; x < width; x++) {
						//>>Constant
						if (useConstant == false)	//Constant 미사용 
							result = (int)Pixel1[y][x] * (int)Pixel2[y][x];
						else
							result = (int)((double)Pixel1[y][x] * dConst + 0.5);
						//<<
						if (result > 255)	PixelO[y][x] = (byte)255;
						else PixelO[y][x] = (byte)result;
					}
				}
			}
			else if (channels == 3) {
				for (int y = 0; y < height; y++) {
					for (int x = 0; x < width; x++) {
						//>>Constant
						if (useConstant == false) {	//Constant 미사용 
							resultR = (int)rgbPixel1[y][x].r * (int)rgbPixel2[y][x].r;
							resultG = (int)rgbPixel1[y][x].g * (int)rgbPixel2[y][x].g;
							resultB = (int)rgbPixel1[y][x].b * (int)rgbPixel2[y][x].b;
						}
						else {
							resultR = (int)((double)rgbPixel1[y][x].r * dConst + 0.5);
							resultG = (int)((double)rgbPixel1[y][x].g * dConst + 0.5);
							resultB = (int)((double)rgbPixel1[y][x].b * dConst + 0.5);
						}
						//<<
						if (resultR > 255)	rgbPixelO[y][x].r = (byte)255;
						else rgbPixelO[y][x].r = (byte)resultR;
						if (resultG > 255)	rgbPixelO[y][x].g = (byte)255;
						else rgbPixelO[y][x].g = (byte)resultG;
						if (resultB > 255)	rgbPixelO[y][x].b = (byte)255;
						else rgbPixelO[y][x].b = (byte)resultB;
					}
				}
			}
			break;

		case CV_16U:
		case CV_16S:
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					//>>Constant
					if (useConstant == false)	//Constant 미사용 
						result = (int)wPixel1[y][x] + (int)wPixel2[y][x];
					else
						result = (int)((double)wPixel1[y][x] * dConst + 0.5);
					//<<
					if (result > 65535)	wPixelO[y][x] = (WORD)65535;
					else wPixelO[y][x] = (WORD)result;
				}
			}
			break;

		default:
			return false;
		}
		break;

	default:
		return false;
	}
	return true;
}

bool IP_ARLogic(Mat & img1, Mat & img2, Mat & oimg, int op, bool useConstant, int Constant)
{
	if (!img1.data || !img2.data)		return false;
	if (img1.depth() != img2.depth())			return false;
	if (img1.channels() != img2.channels())	return false;
	if (img1.rows != img2.rows)		return false;
	if (img1.cols != img2.cols)			return false;

	int depth = img1.depth();
	int channels = img1.channels();
	int type = img1.type();
	int width = img1.rows;
	int height = img1.cols;
	
	oimg = Mat(width, height, type);
	if (!oimg.data) return false;

	switch (op)
	{
	case IOP_ADD:
		if (useConstant == false)	//Constant 미사용 
			add(img1, img2, oimg);
		else
			add(img1, Constant, oimg);
		break;
	case IOP_SUB:
		if (useConstant == false)	//Constant 미사용 
			subtract(img1, img2, oimg);
		else
			subtract(img1, Constant, oimg);
		break;
	case IOP_AVR:
		if (useConstant == false)	//Constant 미사용 
		{
			int depth = img1.depth();
			img1.convertTo(img1, CV_32F);
			img2.convertTo(img2, CV_32F);
			oimg.convertTo(oimg, CV_32F);
			addWeighted(img1, 0.5, img2, 0.5, 0.0, oimg);
			oimg.convertTo(oimg, depth);
			img1.convertTo(img1, depth);
			img2.convertTo(img2, depth);
		}
		else {
			addWeighted(img1, 0.5, Constant, 0.5, 0.0, oimg);
		}
		//oimg.convertTo(oimg, CV_32F);	//형변환 해서도 안됨...
		//add(img1, img2, oimg, noArray(), oimg.depth());
		//divide(oimg, 2, oimg);
		//oimg.convertTo(oimg, img1.depth());

		//oimg = cv::mean(img1, img2);	//이미지 전체 픽셀값의 평균을 구한다음 float형 값 하나를 return -> 취지에 맞지 않음 

		//addWeighted(img1, 0.5, img2, 0.5, 0.0, oimg);	//가중치 연산 함수이지만 이상하게도 몇몇 픽셀의 값이 평균값처럼 나오지 않음 
		break;
	case IOP_DIF:
		if (useConstant == false)	//Constant 미사용 
			absdiff(img1, img2, oimg);
		else
			absdiff(img1, Constant, oimg);
		break;
	case IOP_AND:
		if (useConstant == false)	//Constant 미사용 
			bitwise_and(img1, img2, oimg);
		else
			bitwise_and(img1, Constant, oimg);
		break;
	case IOP_OR:
		if (useConstant == false)	//Constant 미사용 
			bitwise_or(img1, img2, oimg);
		else
			bitwise_or(img1, Constant, oimg);
		break;
	case IOP_MIN:
		if (useConstant == false)	//Constant 미사용 
			min(img1, img2, oimg);
		else
			min(img1, Constant, oimg);
		break;
	case IOP_MAX:
		if (useConstant == false)	//Constant 미사용 
			max(img1, img2, oimg);
		else
			max(img1, Constant, oimg);
		break;
	case IOP_XOR:
		if (useConstant == false)	//Constant 미사용 
			bitwise_xor(img1, img2, oimg);
		else
			bitwise_xor(img1, Constant, oimg);
		break;
	default:
		return false;
	}
	return true;
}

bool IP_Threshold(MyIPImage & img, int threshold)
{
	if (!img.GetPixels())	return false;

	int depth = img.GetDepth();
	int channels = img.GetChannels();
	int width = img.GetWidth();
	int height = img.GetHeight();

	BYTE** Pixels;
	WORD** wPixels;

	switch (depth)
	{
	case CV_8U:
	case CV_8S:
		Pixels = (BYTE**)img.GetPixels();
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				if (Pixels[y][x] > threshold)	Pixels[y][x] = (byte)255;
				else Pixels[y][x] = (byte)0;
			}
		}
	break;

	case CV_16U:
	case CV_16S:
		wPixels = (WORD**)img.GetPixels();
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				if (wPixels[y][x] > threshold)	wPixels[y][x] = (WORD)65535;
				else wPixels[y][x] = (WORD)0;
			}
		}
		break;

	default:
		break;
	}

	return true;
}

bool IP_Bitplane(MyIPImage & img1, MyIPImage & img2, int bit)
{
	if (!img1.GetPixels())	return false;

	int depth = img1.GetDepth();
	int channels = img1.GetChannels();
	int width = img1.GetWidth();
	int height = img1.GetHeight();

	if (!img2.CreateImage(height, width, depth, channels))	return false;

	BYTE** Pixel1;
	BYTE** Pixel2;
	WORD** wPixel1;
	WORD** wPixel2;

	switch (depth)
	{
	case CV_8U:
	case CV_8S:
		Pixel1 = (BYTE**)img1.GetPixels();
		Pixel2 = (BYTE**)img2.GetPixels();
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				Pixel2[y][x] = (Pixel1[y][x] & (1 << bit)) ? 255 : 0;
			}
		}
		break;
		
	case CV_16U:
	case CV_16S:
		wPixel1 = (WORD**)img1.GetPixels();
		wPixel2 = (WORD**)img2.GetPixels();
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				wPixel2[y][x] = (wPixel1[y][x] & (256 << (bit))) ? 65535 : 0;
			}
		}
		break;

	default:
		break;
	}

	return true;
}



	