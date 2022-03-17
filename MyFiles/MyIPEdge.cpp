#include "stdafx.h"
#include "MyIPEdge.h"
#include "MyIPSpetialFilter.h"
#include "MyIPEnhance.h"		//190423 YWCho LoG, DoG 
#include "MyIPConvert.h"		// "

bool IpEdgeDetector(MyIPImage & img, MyIPImage &angleimg, int ntype, BOOL bnormalize)
{
	if (!img.GetPixels())	return false;

	int depth = img.GetDepth();
	int channels = img.GetChannels();
	int height = img.GetHeight();
	int width = img.GetWidth();

	if (channels != 1)						return false;
	if (depth != CV_8U && depth != CV_16U)	return false;

	//angle image
	if (!angleimg.CreateImage(height, width, CV_32F, channels))	return false;
	float** apixel = (float**)angleimg.GetPixels();

	MyIPImage mimg;	//magnitude image
	if (bnormalize == TRUE)
		if (!mimg.CreateImage(height, width, CV_32F, channels))	return false;
	float** mpixel = (float**)mimg.GetPixels();

	MyIPImage timg;	//border image
	if (!IpFillBorder(img, timg, 3, 3, BORDER_REPLICATE))		return false;

	float mag, gx, gy;
	float Max, Min, scale;
	int x, y;
	int offset = 1;	//3*3 mask라서 1이다

	if (depth == CV_8U) {
		byte** pixel = (byte**)img.GetPixels();
		byte** tpixel = (byte**)timg.GetPixels();

		switch (ntype)
		{
		case EDGE_ROBERT:
			if (bnormalize) {
				Max = 0.0;
				Min = 255.0;

				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						gx = (float)(tpixel[y + 1][x + 2] - tpixel[y + 1][x + 1]);		//우중단 - 중  점    -1  1

						gy = (float)(tpixel[y + 2][x + 1] - tpixel[y + 1][x + 1]);		//중하단 - 중  점    -1
																				//					  1

						mag = sqrt((gx*gx) + (gy*gy)) + 0.5f;
						mpixel[y][x] = mag;
						if (mag > Max)		Max = mag;
						else if (mag < Min) Min = mag;
						//apixel[y][x] = atan2(gy, gx);
					}
				}
				if (Max > Min)
					scale = 255.f / (Max - Min);
				else
					scale = 1.;

				//
				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						pixel[y][x] = (byte)((mpixel[y][x] - Min) * scale);
					}
				}
			}

			else {
				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						gx = (float)(tpixel[y + 1][x + 2] - tpixel[y + 1][x + 1]);		//우중단 - 중  점    -1  1

						gy = (float)(tpixel[y + 2][x + 1] - tpixel[y + 1][x + 1]);		//중하단 - 중  점    -1
																				//					  1

						mag = sqrt((gx*gx) + (gy*gy)) + 0.5f;

						pixel[y][x] = (byte)limit(mag);
						apixel[y][x] = atan2(gy, gx);
					}
				}
			}
			break;
		case EDGE_PREWITT:
			if (bnormalize) {
				Max = 0.0;
				Min = 255.0;

				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						gx = (float)(tpixel[y][x + 2] - tpixel[y][x]);					//우상단 - 좌상단 -1  0  1
						gx += (float)(tpixel[y + 1][x + 2] - tpixel[y + 1][x]);			//우중단 - 좌중단 -1  0  1
						gx += (float)(tpixel[y + 2][x + 2] - tpixel[y + 2][x]);			//우하단 - 좌하단 -1  0  1

						gy =  (float)(tpixel[y + 2][x] - tpixel[y][x]);					//좌하단 - 좌상단 -1 -1 -1
						gy += (float)(tpixel[y + 2][x + 1] - tpixel[y][x + 1]);			//중하단 - 중상단  0  0  0
						gy += (float)(tpixel[y + 2][x + 2] - tpixel[y][x + 2]);			//우하단 - 우상단  1  1  1

						mag = sqrt((gx*gx) + (gy*gy)) + 0.5f;
						mpixel[y][x] = mag;
						if (mag > Max)		Max = mag;
						else if (mag < Min) Min = mag;
						//apixel[y][x] = atan2(gy, gx);
					}
				}
				if (Max > Min)
					scale = 255.f / (Max - Min);
				else
					scale = 1.;

				//
				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						pixel[y][x] = (byte)((mpixel[y][x] - Min) * scale);
					}
				}
			}

			else {
				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						gx =  (float)(tpixel[y][x + 2] - tpixel[y][x]);					//우상단 - 좌상단 -1  0  1
						gx += (float)(tpixel[y + 1][x + 2] - tpixel[y + 1][x]);			//우중단 - 좌중단 -1  0  1
						gx += (float)(tpixel[y + 2][x + 2] - tpixel[y + 2][x]);			//우하단 - 좌하단 -1  0  1

						gy =  (float)(tpixel[y + 2][x] - tpixel[y][x]);					//좌하단 - 좌상단 -1 -1 -1
						gy += (float)(tpixel[y + 2][x + 1] - tpixel[y][x + 1]);			//중하단 - 중상단  0  0  0
						gy += (float)(tpixel[y + 2][x + 2] - tpixel[y][x + 2]);			//우하단 - 우상단  1  1  1

						mag = sqrt((gx*gx) + (gy*gy)) + 0.5f;

						pixel[y][x] = (byte)limit(mag);
						//apixel[y][x] = atan2(gy, gx);
					}
				}
			}
			break;
		case EDGE_SOBEL:
			if (bnormalize) {
				Max = 0.f;
				Min = 255.f;

				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						gx =  (float)(tpixel[y][x + 2] - tpixel[y][x]);					//우상단 - 좌상단 -1  0  1
						gx += (tpixel[y + 1][x + 2] - tpixel[y + 1][x]) * 2.f;	//우중단 - 좌중단 -2  0  2
						gx += (float)(tpixel[y + 2][x + 2] - tpixel[y + 2][x]);			//우하단 - 좌하단 -1  0  1

						gy = (float)(tpixel[y + 2][x] - tpixel[y][x]);					//좌하단 - 좌상단 -1 -2 -1
						gy += (tpixel[y + 2][x + 1] - tpixel[y][x + 1])*2.f;		//중하단 - 중상단  0  0  0
						gy += (float)(tpixel[y + 2][x + 2] - tpixel[y][x + 2]);			//우하단 - 우상단  1  2  1

						mag = sqrt((gx*gx) + (gy*gy)) + 0.5f;
						mpixel[y][x] = mag;
						if (mag > Max)		Max = mag;
						else if (mag < Min) Min = mag;
						//apixel[y][x] = atan2(gy, gx);
					}
				}
				if (Max > Min)
					scale = 255.f / (Max - Min);
				else
					scale = 1.f;

				//
				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						pixel[y][x] = (byte)((mpixel[y][x] - Min) * scale);
					}
				}
			}

			else {
				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						gx = (float)(tpixel[y][x + 2] - tpixel[y][x]);					//우상단 - 좌상단 -1  0  1
						gx += (tpixel[y + 1][x + 2] - tpixel[y + 1][x]) * 2.f;	//우중단 - 좌중단 -2  0  2
						gx += (float)(tpixel[y + 2][x + 2] - tpixel[y + 2][x]);			//우하단 - 좌하단 -1  0  1

						gy = (float)(tpixel[y + 2][x] - tpixel[y][x]);					//좌하단 - 좌상단 -1 -2 -1
						gy += (tpixel[y + 2][x + 1] - tpixel[y][x + 1])*2.f;		//중하단 - 중상단  0  0  0
						gy += (float)(tpixel[y + 2][x + 2] - tpixel[y][x + 2]);			//우하단 - 우상단  1  2  1

						mag = sqrt((gx*gx) + (gy*gy)) + 0.5f;

						pixel[y][x] = (byte)limit(mag);
						apixel[y][x] = atan2(gy, gx);
					}
				}
			}
			break;
		default:
			break;
		}

	}
	else if (depth == CV_16U) {
		WORD** pixel = (WORD**)img.GetPixels();
		WORD** tpixel = (WORD**)timg.GetPixels();

		switch (ntype)
		{
		case EDGE_ROBERT:
			if (bnormalize) {
				Max = 0.f;
				Min = 65535.f;

				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						gx = (float)(tpixel[y + 1][x + 2] - tpixel[y + 1][x + 1]);		//우중단 - 중  점    -1  1

						gy = (float)(tpixel[y + 2][x + 1] - tpixel[y + 1][x + 1]);		//중하단 - 중  점    -1
																				//					  1

						mag = sqrt((gx*gx) + (gy*gy)) + 0.5f;
						mpixel[y][x] = mag;
						if (mag > Max)		Max = mag;
						else if (mag < Min) Min = mag;
						//apixel[y][x] = atan2(gy, gx);
					}
				}
				if (Max > Min)
					scale = 65535.f / (Max - Min);
				else
					scale = 1.f;

				//
				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						pixel[y][x] = (WORD)((mpixel[y][x] - Min) * scale);
					}
				}
			}

			else {
				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						gx = (float)(tpixel[y + 1][x + 2] - tpixel[y + 1][x + 1]);		//우중단 - 중  점    -1  1

						gy = (float)(tpixel[y + 2][x + 1] - tpixel[y + 1][x + 1]);		//중하단 - 중  점    -1
																				//					  1

						mag = sqrt((gx*gx) + (gy*gy)) + 0.5f;

						pixel[y][x] = (WORD)limit16bit(mag);
						apixel[y][x] = atan2(gy, gx);
					}
				}
			}
			break;
		case EDGE_PREWITT:
			if (bnormalize) {
				Max = 0.0;
				Min = 65535;

				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						gx =  (float)(tpixel[y][x + 2] - tpixel[y][x]);					//우상단 - 좌상단 -1  0  1
						gx += (float)(tpixel[y + 1][x + 2] - tpixel[y + 1][x]);			//우중단 - 좌중단 -1  0  1
						gx += (float)(tpixel[y + 2][x + 2] - tpixel[y + 2][x]);			//우하단 - 좌하단 -1  0  1

						gy =  (float)(tpixel[y + 2][x] - tpixel[y][x]);					//좌하단 - 좌상단 -1 -1 -1
						gy += (float)(tpixel[y + 2][x + 1] - tpixel[y][x + 1]);			//중하단 - 중상단  0  0  0
						gy += (float)(tpixel[y + 2][x + 2] - tpixel[y][x + 2]);			//우하단 - 우상단  1  1  1

						mag = sqrt((gx*gx) + (gy*gy)) + 0.5f;
						mpixel[y][x] = mag;
						if (mag > Max)		Max = mag;
						else if (mag < Min) Min = mag;
						apixel[y][x] = atan2(gy, gx);
					}
				}
				if (Max > Min)
					scale = 65535.f / (Max - Min);
				else
					scale = 1.f;

				//
				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						pixel[y][x] = (WORD)((mpixel[y][x] - Min) * scale);
					}
				}
			}

			else {
				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						gx =  (float)(tpixel[y][x + 2] - tpixel[y][x]);					//우상단 - 좌상단 -1  0  1
						gx += (float)(tpixel[y + 1][x + 2] - tpixel[y + 1][x]);			//우중단 - 좌중단 -1  0  1
						gx += (float)(tpixel[y + 2][x + 2] - tpixel[y + 2][x]);			//우하단 - 좌하단 -1  0  1

						gy =  (float)(tpixel[y + 2][x] - tpixel[y][x]);					//좌하단 - 좌상단 -1 -1 -1
						gy += (float)(tpixel[y + 2][x + 1] - tpixel[y][x + 1]);			//중하단 - 중상단  0  0  0
						gy += (float)(tpixel[y + 2][x + 2] - tpixel[y][x + 2]);			//우하단 - 우상단  1  1  1

						mag = sqrt((gx*gx) + (gy*gy)) + 0.5f;

						pixel[y][x] = (WORD)limit16bit(mag);
						apixel[y][x] = atan2(gy, gx);
					}
				}
			}
			break;
		case EDGE_SOBEL:
			if (bnormalize) {
				Max = 0.0;
				Min = 65535;

				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						gx = (float)(tpixel[y][x + 2] - tpixel[y][x]);					//우상단 - 좌상단 -1  0  1
						gx += (tpixel[y + 1][x + 2] - tpixel[y + 1][x]) * 2.f;	//우중단 - 좌중단 -2  0  2
						gx += (float)(tpixel[y + 2][x + 2] - tpixel[y + 2][x]);			//우하단 - 좌하단 -1  0  1

						gy = (float)(tpixel[y + 2][x] - tpixel[y][x]);					//좌하단 - 좌상단 -1 -2 -1
						gy += (tpixel[y + 2][x + 1] - tpixel[y][x + 1])*2.f;		//중하단 - 중상단  0  0  0
						gy += (float)(tpixel[y + 2][x + 2] - tpixel[y][x + 2]);			//우하단 - 우상단  1  2  1

						mag = sqrt((gx*gx) + (gy*gy)) + 0.5f;
						mpixel[y][x] = mag;
						if (mag > Max)		Max = mag;
						else if (mag < Min) Min = mag;
						apixel[y][x] = atan2(gy, gx);
					}
				}
				if (Max > Min)
					scale = 65535.f / (Max - Min);
				else
					scale = 1.f;

				//
				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						pixel[y][x] = (WORD)((mpixel[y][x] - Min) * scale);
					}
				}
			}

			else {
				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						gx = (float)(tpixel[y][x + 2] - tpixel[y][x]);					//우상단 - 좌상단 -1  0  1
						gx += (tpixel[y + 1][x + 2] - tpixel[y + 1][x]) * 2.f;			//우중단 - 좌중단 -2  0  2
						gx += (float)(tpixel[y + 2][x + 2] - tpixel[y + 2][x]);			//우하단 - 좌하단 -1  0  1

						gy = (float)(tpixel[y + 2][x] - tpixel[y][x]);					//좌하단 - 좌상단 -1 -2 -1
						gy += (tpixel[y + 2][x + 1] - tpixel[y][x + 1])*2.f;			//중하단 - 중상단  0  0  0
						gy += (float)(tpixel[y + 2][x + 2] - tpixel[y][x + 2]);			//우하단 - 우상단  1  2  1

						mag = sqrt((gx*gx) + (gy*gy)) + 0.5f;

						pixel[y][x] = (WORD)limit16bit(mag);
						apixel[y][x] = atan2(gy, gx);
					}
				}
			}
			break;
		default:
			break;
		}

	}
	else
		return false;

	return true;
}

bool IpLaplacian(MyIPImage & img, int ntype, BOOL bnormalize)
{
	if (!img.GetPixels())	return false;

	int depth = img.GetDepth();
	int channels = img.GetChannels();
	int height = img.GetHeight();
	int width = img.GetWidth();

	if (channels != 1)						return false;
	if (depth != CV_8U && depth != CV_16U)	return false;

	MyIPImage mimg;	//magnitude image
	if (bnormalize == TRUE)
		if (!mimg.CreateImage(height, width, CV_32F, channels))	return false;
	float** mpixel = (float**)mimg.GetPixels();

	MyIPImage timg;	//border image
	if (!IpFillBorder(img, timg, 3, 3, BORDER_REPLICATE))		return false;

	float mag, G;	//magnitude
	float Max, Min, scale;
	int x, y, fx, fy;
	int offset = 1;	//3*3 mask라서 1이다

	if (depth == CV_8U) {
		byte** pixel = (byte**)img.GetPixels();
		byte** tpixel = (byte**)timg.GetPixels();

		switch (ntype)
		{
		case EDGE_LAPLACIAN4:
			if (bnormalize) {
				Max = 0.0;
				Min = 255.0;

				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						G = 0;
						for (fy = 0; fy < 3; fy++)
							G += tpixel[y + fy][x + 1];
						for (fx = 0; fx < 3; fx++)
							G += tpixel[y + 1][x + fx];
						G -= (tpixel[y + 1][x + 1] * 6.f);
						mag = G;	//-값을 0으로 만들기 위해서 절대값을 취하지 않는다 -> 0을 128로 만들기 위해서 
						mpixel[y][x] = mag;
						if (mag > Max)
							Max = mag;
						else if (mag < Min)
							Min = mag;
					}
				}
				if (Max > Min)
					scale = 255.f / (Max - Min);
				else
					scale = 1.;

				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						pixel[y][x] = (byte)((mpixel[y][x] - Min) * scale);
					}
				}
			}
			else {
				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						G = 0;
						for (fy = 0; fy < 3; fy++)
							G += tpixel[y + fy][x + 1];
						for (fx = 0; fx < 3; fx++)
							G += tpixel[y + 1][x + fx];
						G -= (tpixel[y + 1][x + 1] * 6.f);
						mag = abs(G);
						pixel[y][x] = (byte)limit(mag);

					}
				}
			}
			break;
		case EDGE_LAPLACIAN8:
			if (bnormalize) {
				Max = 0.0;
				Min = 255.0;

				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						G = 0;
						for (fy = 0; fy < 3; fy++) {
							for (fx = 0; fx < 3; fx++) {
								G += tpixel[y + fy][x + fx];
							}
						}
						G -= (tpixel[y + 1][x + 1] * 9.f);
						mag = G;	//-값을 0으로 만들기 위해서 절대값을 취하지 않는다 -> 0을 128로 만들기 위해서 
						mpixel[y][x] = mag;
						if (mag > Max)
							Max = mag;
						else if (mag < Min)
							Min = mag;
					}
				}
				if (Max > Min)
					scale = 255.f / (Max - Min);
				else
					scale = 1.;

				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						pixel[y][x] = (byte)((mpixel[y][x] - Min) * scale);
					}
				}
			}
			else {
				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						G = 0;
						for (fy = 0; fy < 3; fy++) {
							for (fx = 0; fx < 3; fx++) {
								G += tpixel[y + fy][x + fx];
							}
						}
						G -= (tpixel[y + 1][x + 1] * 9.f);
						mag = abs(G);
						pixel[y][x] = (byte)limit(mag);

					}
				}
			}
			break;
		default:
			break;
		}

	}
	else if (depth == CV_16U) {
		WORD** pixel = (WORD**)img.GetPixels();
		WORD** tpixel = (WORD**)timg.GetPixels();

		switch (ntype)
		{
		case EDGE_LAPLACIAN4:
			if (bnormalize) {
				Max = 0.0;
				Min = 65535.;

				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						G = 0;
						for (fy = 0; fy < 3; fy++)
							G += tpixel[y + fy][x + 1];
						for (fx = 0; fx < 3; fx++)
							G += tpixel[y + 1][x + fx];
						G -= (tpixel[y + 1][x + 1] * 6.f);
						mag = G;	//-값을 0으로 만들기 위해서 절대값을 취하지 않는다 -> 0을 128로 만들기 위해서 
						mpixel[y][x] = mag;
						if (mag > Max)
							Max = mag;
						else if (mag < Min)
							Min = mag;
					}
				}
				if (Max > Min)
					scale = 65535.f / (Max - Min);
				else
					scale = 1.;

				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						pixel[y][x] = (WORD)((mpixel[y][x] - Min) * scale);
					}
				}
			}
			else {
				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						G = 0;
						for (fy = 0; fy < 3; fy++)
							G += tpixel[y + fy][x + 1];
						for (fx = 0; fx < 3; fx++)
							G += tpixel[y + 1][x + fx];
						G -= (tpixel[y + 1][x + 1] * 6.f);
						mag = abs(G);
						pixel[y][x] = (WORD)limit16bit(mag);

					}
				}
			}
			break;
		case EDGE_LAPLACIAN8:
			if (bnormalize) {
				Max = 0.0;
				Min = 65535.;

				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						G = 0;
						for (fy = 0; fy < 3; fy++) {
							for (fx = 0; fx < 3; fx++) {
								G += tpixel[y + fy][x + fx];
							}
						}
						G -= (tpixel[y + 1][x + 1] * 9.f);
						mag = G;	//-값을 0으로 만들기 위해서 절대값을 취하지 않는다 -> 0을 128로 만들기 위해서 
						mpixel[y][x] = mag;
						if (mag > Max)
							Max = mag;
						else if (mag < Min)
							Min = mag;
					}
				}
				if (Max > Min)
					scale = 65535.f / (Max - Min);
				else
					scale = 1.;

				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						pixel[y][x] = (WORD)((mpixel[y][x] - Min) * scale);
					}
				}
			}
			else {
				for (y = 0; y < height; y++) {
					for (x = 0; x < width; x++) {
						G = 0;
						for (fy = 0; fy < 3; fy++) {
							for (fx = 0; fx < 3; fx++) {
								G += tpixel[y + fy][x + fx];
							}
						}
						G -= (tpixel[y + 1][x + 1] * 9.f);
						mag = abs(G);
						pixel[y][x] = (WORD)limit16bit(mag);

					}
				}
			}
			break;
		default:
			break;
		}
	}
	else
		return false;

	return true;
}

bool IpCanny(MyIPImage & img, int lowth, int highth)
{
	if (!img.GetPixels()) return false;
	if (lowth >= highth) return false;

	// 1. Gaussian Filter with sigma = 1.4;
	//
	double sigma = 1.4;
	int h, w;
	h = (int)(3.* sigma + 0.5) * 2 + 1;
	w = h;
	double *buffer = new double[h*w];
	double **kernel = new double*[h];
	for (int y = 0; y < h; y++)
		kernel[y] = buffer + y*w;

	if (!IpGaussianKernel(kernel, h, w, sigma)) return false;
	if (!IpFilter2D(img, h, w, kernel, IOP_BORDER_REPLICATE)) return false;

	delete[] kernel;
	delete[] buffer;

	// 2. Sobel Edge detector to compute Edge strength(mimg) and direction(aimg).
	//
	int depth = img.GetDepth();
	int channels = img.GetChannels();
	if ((depth != CV_8U) && (depth != CV_16U)) return false;
	if (channels != 1) return false;


	int width = img.GetWidth();
	int height = img.GetHeight();

	MyIPImage timg, mimg, aimg;
	if (!IpFillBorder(img, timg, 3, 3, IOP_BORDER_REPLICATE)) return false;
	if (!mimg.CreateImage(height, width, CV_32F, 1)) return false;
	if (!aimg.CreateImage(height, width, CV_32F, 1)) return false;
	float **mpixel = (float **)mimg.GetPixels();
	float **apixel = (float **)aimg.GetPixels();

	// Image(simg) for Weak Edge position indicator (initially all zeros)
	MyIPImage simg;
	if (!simg.CreateImage(height, width, CV_8U, 1)) return false;
	byte **spixel = (byte **)simg.GetPixels();
	for (int y = 0; y < height; y++)
		memset(spixel[y], 0, width);

	// Vector of Strong Edge coordinates
	vector<CPoint> strong_edges;
	strong_edges.reserve(width*height);

	int x, y;
	float gx, gy;

	if (depth == CV_8U) 
	{
		byte **pixel = (byte**)img.GetPixels();
		byte **tpixel = (byte**)timg.GetPixels();

		for (y = 0; y < height; y++) {
			for (x = 0; x < width; x++) {
				gx = (float)tpixel[y][x + 2] - (float)tpixel[y][x] +
					2.0f*((float)tpixel[y + 1][x + 2] - (float)tpixel[y + 1][x]) +
					(float)tpixel[y + 2][x + 2] - (float)tpixel[y + 2][x];
				gy = (float)tpixel[y + 2][x] - (float)tpixel[y][x] +
					2.0f*((float)tpixel[y + 2][x + 1] - (float)tpixel[y][x + 1]) +
					(float)tpixel[y + 2][x + 2] - (float)tpixel[y][x + 2];

				mpixel[y][x] = (float)sqrt((double)(gx*gx + gy*gy));
				apixel[y][x] = atan2(gy, gx);
			}
		}

		//3. Non Maximum Suppression
		for (int y = 0; y < height; y++)
			memset(pixel[y], 0, width * sizeof(byte));	//pixel은 input과 동시에 output도 하기에 0으로 초기화 

		int direction;
		float v1, v2;
		float r2d = 180.f / (float)CV_PI;
		for (y = 1; y < height - 1; y++) {
			for (x = 1; x < width - 1; x++) {
				if (mpixel[y][x] <= (float)lowth) continue;

				if (apixel[y][x] < 0)	apixel[y][x] += (float)CV_PI;
				direction = ((int)(r2d * apixel[y][x] + 22.5f) / 45) % 4;

				switch (direction) {
				case 0:
					v1 = mpixel[y][x - 1];
					v2 = mpixel[y][x + 1];
					break;
				case 1:
					v1 = mpixel[y - 1][x - 1];
					v2 = mpixel[y + 1][x + 1];
					break;

				case 2:
					v1 = mpixel[y - 1][x];
					v2 = mpixel[y + 1][x];
					break;
				case 3:
					v1 = mpixel[y - 1][x + 1];
					v2 = mpixel[y + 1][x - 1];
					break;
				}

				if ((mpixel[y][x] < v1) || (mpixel[y][x] < v2)) continue;

				if (mpixel[y][x] > (float)highth) {
					pixel[y][x] = (byte)255;
					strong_edges.push_back(CPoint(x, y));
				}
				else
					spixel[y][x] = (byte)255;	//week edge 
			}
		}

		//4. Trace 
		CPoint pt;
		while (!strong_edges.empty()) {
			pt = strong_edges.back();
			strong_edges.pop_back();

			for (y = pt.y - 1; y <= pt.y + 1; y++) {				//strong edge 주위 픽셀에
				for (x = pt.x - 1; x <= pt.x + 1; x++) {
					if (spixel[y][x]) {							//week edge가 있으면 
						spixel[y][x] = (byte)0;					//너 이제 week edge 아니야
						pixel[y][x] = (byte)255;				//너도 Output에 표시될거야
						strong_edges.push_back(CPoint(x, y));	//다 했으니 끄집어내자
					}
				}
			}
		}
	}
	else {
		WORD **pixel = (WORD**)img.GetPixels();
		WORD **tpixel = (WORD**)timg.GetPixels();

		for (y = 0; y < height; y++) {
			for (x = 0; x < width; x++) {
				gx = (float)tpixel[y][x + 2] - (float)tpixel[y][x] +
					2.0f*((float)tpixel[y + 1][x + 2] - (float)tpixel[y + 1][x]) +
					(float)tpixel[y + 2][x + 2] - (float)tpixel[y + 2][x];
				gy = (float)tpixel[y + 2][x] - (float)tpixel[y][x] +
					2.0f*((float)tpixel[y + 2][x + 1] - (float)tpixel[y][x + 1]) +
					(float)tpixel[y + 2][x + 2] - (float)tpixel[y][x + 2];

				mpixel[y][x] = (float)sqrt((double)(gx*gx + gy*gy));
				apixel[y][x] = atan2(gy, gx);
			}
		}

		//3. Non Maximum Suppression
		for (int y = 0; y < height; y++)
			memset(pixel[y], 0, width * sizeof(unsigned short));	//pixel은 input과 동시에 output도 하기에 0으로 초기화 

		int direction;
		float v1, v2;
		float r2d = 180.f / (float)CV_PI;
		for (y = 1; y < height - 1; y++) {
			for (x = 1; x < width - 1; x++) {
				if (mpixel[y][x] <= (float)lowth) continue;
				
				if (apixel[y][x] < 0)	apixel[y][x] += (float)CV_PI;
				direction = ((int)(r2d * apixel[y][x] + 22.5f) / 45) % 4;

				switch (direction) {
				case 0:
					v1 = mpixel[y][x - 1];
					v2 = mpixel[y][x + 1];
					break;
				case 1:
					v1 = mpixel[y - 1][x - 1];
					v2 = mpixel[y + 1][x + 1];
					break;

				case 2:
					v1 = mpixel[y - 1][x];
					v2 = mpixel[y + 1][x];
					break;
				case 3:
					v1 = mpixel[y - 1][x + 1];
					v2 = mpixel[y + 1][x - 1];
					break;
				}

				if ((mpixel[y][x] < v1) || (mpixel[y][x] < v2)) continue;

				if (mpixel[y][x] > (float)highth) {
					pixel[y][x] = (WORD)65535;
					strong_edges.push_back(CPoint(x, y));
				}
				else
					spixel[y][x] = (byte)255;	//week edge 
			}
		}

		//4. Trace 
		CPoint pt;
		while (!strong_edges.empty()) {
			pt = strong_edges.back();
			strong_edges.pop_back();

			for (y = pt.y - 1; y <= pt.y + 1; y++) {				//strong edge 주위 픽셀에
				for (x = pt.x - 1; x <= pt.x + 1; x++) {
					if (spixel[y][x]) {							//week edge가 있으면 
						spixel[y][x] = (byte)0;					//너 이제 week edge 아니야
						pixel[y][x] = (WORD)65535;				//너도 Output에 표시될거야
						strong_edges.push_back(CPoint(x, y));	//다 했으니 끄집어내자
					}
				}
			}
		}
	}
	return true;
}

bool IpLoG(MyIPImage & img, double sigma)
{
	if (!img.GetPixels())	return false;
	if (sigma <= 0)			return false;


	int h = (int)(3 * sigma + 0.5) * 2 + 1;	//get kernel height from big sigma
	int w = h;

	double *buffer = new double[h*w];
	double **kernel = new double*[h];
	for (int y = 0; y < h; y++)
		kernel[y] = buffer + y*w;		//double이기에 8*y*w임


	if (IpLoGaussianKernel(kernel, h, w, sigma)) {
		if (!IpFilter2D(img, h, w, kernel, BORDER_REFLECT))
			return false;
	}

	delete[] kernel;
	delete[] buffer;
	return true;
}

bool IpDoG(MyIPImage & img, double sigma1, double sigma2)
{
	if (!img.GetPixels())	return false;
	if (sigma1 < 0 || sigma2 < 0)			return false;

	MyIPImage BigSigImg, SmallSigImg;
	BigSigImg = img;
	SmallSigImg = img;

	double sigmaBig = (sigma1 > sigma2) ? sigma1 : sigma2;
	double sigmaSmall = (sigma1 <= sigma2) ? sigma1 : sigma2;
	int h = (int)(3* sigmaBig + 0.5) * 2 + 1;	//get kernel height from big sigma
	int w = h;

	double *buffer = new double[h*w];
	double **kernel = new double*[h];
	for (int y = 0; y < h; y++) 
		kernel[y] = buffer + y*w;		//double이기에 8*y*w임


	if (IpGaussianKernel(kernel, h, w, sigmaBig)) {
		if (!IpFilter2D(BigSigImg, h, w, kernel, BORDER_REFLECT))
			return false;
	}
	if (IpGaussianKernel(kernel, h, w, sigmaSmall)) {
		if (!IpFilter2D(SmallSigImg, h, w, kernel, BORDER_REFLECT))
			return false;
	}

	if (!IP_ARLogic(SmallSigImg, BigSigImg, img, IOP_SUB))
		return false;


// 	Mat matimg, dst1, dst2;
// 	if (!IpImageToMat(BigSigImg, matimg))
// 		return false;
// 
// 	GaussianBlur(matimg, dst1, Size(9, 9), 0.9);
// 	GaussianBlur(matimg, dst2, Size(9, 9), 1.4);
// 	subtract(dst1, dst2, matimg);
//
// 	AfxNewImage(_T("dst1"), dst1);
// 	AfxNewImage(_T("dst2"), dst2);
// 	AfxNewImage(_T("smallsigma(dst1)"), SmallSigImg);
// 	AfxNewImage(_T("bigsigma(dst2)"), BigSigImg);
// 	AfxNewImage(_T("dst1 - dst2"), matimg);

	delete[] kernel;
	delete[] buffer;
	return true;
}
