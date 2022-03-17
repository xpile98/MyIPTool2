#include "stdafx.h"
#include "MyIPGeometry.h"

bool IpTranslate(MyIPImage & img, MyIPImage & dst, int sx, int sy)
{
	if (!img.GetPixels())	return false;

	int depth = img.GetDepth();
	int channels = img.GetChannels();
	int height = img.GetHeight();
	int width = img.GetWidth();

	if (!dst.CreateImage(height, width, depth, channels))	return false;

	BYTE** imgPixels;
	BYTE** dstPixels;
	RGBBYTE** rgb_imgPixels;
	RGBBYTE** rgb_dstPixels;

	if (channels == 1)
	{
		imgPixels = (BYTE**)img.GetPixels();
		dstPixels = (BYTE**)dst.GetPixels();
	}
	else if (channels == 3)
	{
		rgb_imgPixels = (RGBBYTE**)img.GetPixels();
		rgb_dstPixels = (RGBBYTE**)dst.GetPixels();
	}

	int i, j, x, y;
	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			x = i - sx;
			y = j - sy;
			if (x >= 0 && y >= 0 && x < width && y < height)
			{
				if (channels == 1)
					dstPixels[j][i] = imgPixels[y][x];
				else if (channels == 3)
				{
					rgb_dstPixels[j][i].r = rgb_imgPixels[y][x].r;
					rgb_dstPixels[j][i].g = rgb_imgPixels[y][x].g;
					rgb_dstPixels[j][i].b = rgb_imgPixels[y][x].b;
				}
			}
		}
	}
}

bool IpTranslateROI(MyIPImage & img, MyIPImage & dst, int sx, int sy, int rx, int ry, int rw, int rh)
{
	if (!img.GetPixels())	return false;

	int depth = img.GetDepth();
	int channels = img.GetChannels();
	int height = img.GetHeight();
	int width = img.GetWidth();

	if (!dst.CreateImage(height, width, depth, channels))	return false;
	dst.Copy(img);

	BYTE** imgPixels;
	BYTE** dstPixels;
	RGBBYTE** rgb_imgPixels;
	RGBBYTE** rgb_dstPixels;

	if (channels == 1)
	{
		imgPixels = (BYTE**)img.GetPixels();
		dstPixels = (BYTE**)dst.GetPixels();
	}
	else if (channels == 3)
	{
		rgb_imgPixels = (RGBBYTE**)img.GetPixels();
		rgb_dstPixels = (RGBBYTE**)dst.GetPixels();
	}


	int i, j, x, y;
	for (j = ry - sy; j < ry + rh; j++)		//이동량이 적은 경우 이동할 방향의 반대로 이동량 만큼의 ROI를 추가로 잡아서 빈 공간이 어색해지지 않게 처리
	{
		for (i = rx - sx; i < rx + rw; i++)
		{
			x = i + sx;
			y = j + sy;

			if (x >= 0 && y >= 0 && x < width && y < height)
			{
				if (channels == 1)
				{
					dstPixels[y][x] = imgPixels[j][i];
				}
				else if (channels == 3)
				{
					rgb_dstPixels[y][x].r = rgb_imgPixels[j][i].r;
					rgb_dstPixels[y][x].g = rgb_imgPixels[j][i].g;
					rgb_dstPixels[y][x].b = rgb_imgPixels[j][i].b;
				}
			}
		}
	}
}
