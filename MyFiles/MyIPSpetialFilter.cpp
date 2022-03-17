#include "stdafx.h"
#include "MyIPSpetialFilter.h"

bool IpFilter2D(MyIPImage &img, int h, int w, double ** kernel, int borderType, int value)
{
	if (!img.GetPixels())				return false;
	if ((h % 2 == 0) || (w % 2 == 0))	return false;
	if (!kernel)						return false;

	int depth = img.GetDepth();
	int channels = img.GetChannels();
	int width = img.GetWidth();
	int height = img.GetHeight();

	MyIPImage tempimg;
	if (!IpFillBorder(img, tempimg, h, w, borderType, value))	return false;
// 	if (!tempimg.CreateImage((height + h / 2), (width + w / 2), depth, channels)) return false;
//>> ������ �̷� tempimg������ ���� ����� �����ϱ� �Լ��� ���������.(IpFillBorder)
	//AfxNewImage(_T("FillBorder"), tempimg);

// 	bool bIsOverlap = true;
// 	for (int kh = 0; kh < h; kh++) {
// 		for (int kw = 0; kw < w; kw++) {
// 			if (kernel[kh][kw] != kernel[0][0]) {
// 				bIsOverlap = false;
// 				break;
// 			}
// 		}
// 	}


	int x, y;
	if (depth == CV_8U || depth == CV_8S) {

		if (channels == 1) {
			byte** pixel = (byte**)img.GetPixels();
			byte** tpixel = (byte**)tempimg.GetPixels();

			for (y = 0; y < height; y++) {
				for (x = 0; x < width; x++) {
					double sum = 0.5;
					for (int ky = 0; ky < h; ky++) {
						for (int kx = 0; kx < w; kx++) {
							int pixel = tpixel[y + ky][x + kx];	//�� �ȼ����� �Ź� �ٲ� (0~255) 
							sum += pixel*kernel[ky][kx];		//kernel[ky][kx]�� �迭�� ����� ... -> for���� ���鼭 LUT �� ����� ���?
						}
					}
					pixel[y][x] = (byte)limit(sum);
				}
			}
		}
		else if (channels == 3) {
			RGBBYTE** pixel = (RGBBYTE**)img.GetPixels();
			RGBBYTE** tpixel = (RGBBYTE**)tempimg.GetPixels();

			for (y = 0; y < height; y++) {
				for (x = 0; x < width; x++) {
					double Rsum = 0.5, Gsum = 0.5, Bsum = 0.5;
					for (int ky = 0; ky < h; ky++) {
						for (int kx = 0; kx < w; kx++) {
							int Rpixel = tpixel[y + ky][x + kx].r;	//�� �ȼ����� �Ź� �ٲ� (0~255) 
							int Gpixel = tpixel[y + ky][x + kx].g;	//�� �ȼ����� �Ź� �ٲ� (0~255) 
							int Bpixel = tpixel[y + ky][x + kx].b;	//�� �ȼ����� �Ź� �ٲ� (0~255) 
							Rsum += Rpixel*kernel[ky][kx];		//kernel[ky][kx]�� �迭�� ����� ... -> for���� ���鼭 LUT �� ����� ���?
							Gsum += Gpixel*kernel[ky][kx];		//kernel[ky][kx]�� �迭�� ����� ... -> for���� ���鼭 LUT �� ����� ���?
							Bsum += Bpixel*kernel[ky][kx];		//kernel[ky][kx]�� �迭�� ����� ... -> for���� ���鼭 LUT �� ����� ���?
						}
					}
					pixel[y][x].r = (byte)limit(Rsum);
					pixel[y][x].g = (byte)limit(Gsum);
					pixel[y][x].b = (byte)limit(Bsum);
				}
			}
		}
		else 
			return false;
	}

	else if (depth == CV_16U || depth == CV_16S) {

		if (channels == 1) {
			WORD** pixel = (WORD**)img.GetPixels();
			WORD** tpixel = (WORD**)tempimg.GetPixels();

			for (y = 0; y < height; y++) {
				for (x = 0; x < width; x++) {
					double sum = 0.5;
					for (int ky = 0; ky < h; ky++) {
						for (int kx = 0; kx < w; kx++) {
							int pixel = tpixel[y + ky][x + kx];	//�� �ȼ����� �Ź� �ٲ� (0~255) 
							sum += pixel*kernel[ky][kx];		//kernel[ky][kx]�� �迭�� ����� ... -> for���� ���鼭 LUT �� ����� ���?
						}
					}
					pixel[y][x] = (WORD)limit16bit(sum);
				}
			}
		}

		else if (channels == 3) 
			return false;

		else
			return false;
	}

	else
		return false;

	return true;
}

bool IpFilterMax(MyIPImage & img, int h, int w, int borderType, int value)
{
	if (!img.GetPixels())				return false;
	if ((h % 2 == 0) || (w % 2 == 0))	return false;

	int depth = img.GetDepth();
	int channels = img.GetChannels();
	int width = img.GetWidth();
	int height = img.GetHeight();

	int x, y;
	int kh2 = h / 2;					//kernel height / 2 (��, �Ʒ��� ���� �þ�� �ȼ� ��)
	int kw2 = w / 2;					//kernel  width / 2 (���ʿ����� ���� �þ�� �ȼ� ��)
	int newWidth = width + kw2 * 2;		//���� �����Ǵ� �̹����� Width
	int newHeight = height + kh2 * 2;	//���� �����Ǵ� �̹����� Height


	MyIPImage tempimg;
	if (!IpFillBorder(img, tempimg, h, w, borderType, value))	return false;

	if (depth == CV_8U || depth == CV_8S) {

		if (channels == 1) {
			byte** pixel = (byte**)img.GetPixels();
			byte** tpixel = (byte**)tempimg.GetPixels();

			for (y = 0; y < height; y++) {
				for (x = 0; x < width; x++) {
					byte max = 0;
					for (int ky = 0; ky < h; ky++) {
						for (int kx = 0; kx < w; kx++) {
							int pixel = tpixel[y + ky][x + kx];	//�� �ȼ����� �Ź� �ٲ� (0~255) 
							if (pixel > max)
								max = pixel;
						}
					}
					pixel[y][x] = (byte)limit(max);
				}
			}
		}
		else if (channels == 3) {
			RGBBYTE** pixel = (RGBBYTE**)img.GetPixels();
			RGBBYTE** tpixel = (RGBBYTE**)tempimg.GetPixels();

			for (y = 0; y < height; y++) {
				for (x = 0; x < width; x++) {
					RGBBYTE max((byte)0);
					for (int ky = 0; ky < h; ky++) {
						for (int kx = 0; kx < w; kx++) {
							RGBBYTE pixel;
							pixel.r = tpixel[y + ky][x + kx].r;	//�� �ȼ����� �Ź� �ٲ� (0~255) 
							pixel.g = tpixel[y + ky][x + kx].g;	//�� �ȼ����� �Ź� �ٲ� (0~255) 
							pixel.b = tpixel[y + ky][x + kx].b;	//�� �ȼ����� �Ź� �ٲ� (0~255) 
							if (pixel.r > max.r) max.r = pixel.r;
							if (pixel.g > max.g) max.g = pixel.g;
							if (pixel.b > max.b) max.b = pixel.b;
						}
					}
					pixel[y][x].r = (byte)limit((byte)max.r);
					pixel[y][x].g = (byte)limit((byte)max.g);
					pixel[y][x].b = (byte)limit((byte)max.b);
				}
			}
		}
		else
			return false;
	}
	else if (depth == CV_16U || depth == CV_16S) {
		if (channels == 1) {
			WORD** pixel = (WORD**)img.GetPixels();
			WORD** tpixel = (WORD**)tempimg.GetPixels();

			for (y = 0; y < height; y++) {
				for (x = 0; x < width; x++) {
					WORD max = 0;
					for (int ky = 0; ky < h; ky++) {
						for (int kx = 0; kx < w; kx++) {
							int pixel = tpixel[y + ky][x + kx];	//�� �ȼ����� �Ź� �ٲ� (0~255) 
							if (pixel > max)
								max = pixel;
						}
					}
					pixel[y][x] = (WORD)limit16bit(max);
				}
			}
		}
		else if (channels == 3) {
			return false;
		}
		else
			return false;
	}
	else return false;


	return true;
}

bool IpFilterMin(MyIPImage & img, int h, int w, int borderType, int value)
{
	if (!img.GetPixels())				return false;
	if ((h % 2 == 0) || (w % 2 == 0))	return false;

	int depth = img.GetDepth();
	int channels = img.GetChannels();
	int width = img.GetWidth();
	int height = img.GetHeight();

	int x, y;
	int kh2 = h / 2;					//kernel height / 2 (��, �Ʒ��� ���� �þ�� �ȼ� ��)
	int kw2 = w / 2;					//kernel  width / 2 (���ʿ����� ���� �þ�� �ȼ� ��)
	int newWidth = width + kw2 * 2;		//���� �����Ǵ� �̹����� Width
	int newHeight = height + kh2 * 2;	//���� �����Ǵ� �̹����� Height


	MyIPImage tempimg;
	if (!IpFillBorder(img, tempimg, h, w, borderType, value))	return false;

	if (depth == CV_8U || depth == CV_8S) {

		if (channels == 1) {
			byte** pixel = (byte**)img.GetPixels();
			byte** tpixel = (byte**)tempimg.GetPixels();

			for (y = 0; y < height; y++) {
				for (x = 0; x < width; x++) {
					byte min = 255;
					for (int ky = 0; ky < h; ky++) {
						for (int kx = 0; kx < w; kx++) {
							int pixel = tpixel[y + ky][x + kx];	//�� �ȼ����� �Ź� �ٲ� (0~255) 
							if (pixel < min)
								min = pixel;
						}
					}
					pixel[y][x] = (byte)limit(min);
				}
			}
		}
		else if (channels == 3) {
			RGBBYTE** pixel = (RGBBYTE**)img.GetPixels();
			RGBBYTE** tpixel = (RGBBYTE**)tempimg.GetPixels();

			for (y = 0; y < height; y++) {
				for (x = 0; x < width; x++) {
					RGBBYTE min((byte)255);
					for (int ky = 0; ky < h; ky++) {
						for (int kx = 0; kx < w; kx++) {
							RGBBYTE pixel;
							pixel.r = tpixel[y + ky][x + kx].r;	//�� �ȼ����� �Ź� �ٲ� (0~255) 
							pixel.g = tpixel[y + ky][x + kx].g;	//�� �ȼ����� �Ź� �ٲ� (0~255) 
							pixel.b = tpixel[y + ky][x + kx].b;	//�� �ȼ����� �Ź� �ٲ� (0~255) 
							if (pixel.r < min.r) min.r = pixel.r;
							if (pixel.g < min.g) min.g = pixel.g;
							if (pixel.b < min.b) min.b = pixel.b;
						}
					}
					pixel[y][x].r = (byte)limit((byte)min.r);
					pixel[y][x].g = (byte)limit((byte)min.g);
					pixel[y][x].b = (byte)limit((byte)min.b);
				}
			}
		}
		else
			return false;
	}
	else if (depth == CV_16U || depth == CV_16S) {
		if (channels == 1) {
			WORD** pixel = (WORD**)img.GetPixels();
			WORD** tpixel = (WORD**)tempimg.GetPixels();

			for (y = 0; y < height; y++) {
				for (x = 0; x < width; x++) {
					WORD min = 65535;
					for (int ky = 0; ky < h; ky++) {
						for (int kx = 0; kx < w; kx++) {
							int pixel = tpixel[y + ky][x + kx];	//�� �ȼ����� �Ź� �ٲ� (0~255) 
							if (pixel < min)
								min = pixel;
						}
					}
					pixel[y][x] = (WORD)limit16bit(min);
				}
			}
		}
		else if (channels == 3) {
			return false;
		}
		else
			return false;
	}
	else return false;


	return true;
}

bool IpFilterMed(MyIPImage & img, int h, int w)
{
	if (!img.GetPixels())		return false;
	if (h < 3 || h % 2 == 0)	return false;
	if (w < 3 || w % 2 == 0)	return false;

	int y, x, fy, fx;						//parameter for for loop

	int depth = img.GetDepth();				//image depth 
	int channels = img.GetChannels();		//iamge channels 

	int height = img.GetHeight();			//image height 
	int width = img.GetWidth();				//image width
	
	int *arr = new int[h*w];				//dynamic allocation 
	int cnt;								//for count arr 

	//int mVal;								//median value
	int mid = (h*w) / 2;					//middle of array
	MyIPImage timg;
	if (!IpFillBorder(img, timg, h, w, IOP_BORDER_REFLECT))	//Median Filter�� Border Type�� Reflect �̴�. 
		return false;
		
// 	byte **pixel, **tpixel;				//8bit 1ch		//>> move to if~else 
// 	RGBBYTE **rgbpixel, **rgbtpixel;	//8bit 3ch
// 	WORD **wpixel, **wtpixel;			//16bit 1ch

	if (depth == CV_8U || depth == CV_8S) {
		if (channels == 1) {
			byte **pixel = (byte**)img.GetPixels();
			byte **tpixel = (byte**)timg.GetPixels();

			for (y = 0; y < height; y++) {
				for (x = 0; x < width; x++) {
					cnt = 0;
					for (fy = 0; fy < h; fy++) {
						for (fx = 0; fx < w; fx++) {
							arr[cnt] = tpixel[y + fy][x + fx];	//save in array 
							cnt++;								//increase cnt 
						}
					}
					//sort 
					std::sort(arr, arr + (h*w));
					pixel[y][x] = (byte)limit(arr[mid]);
				}
			}
		}
		else if (channels == 3) {
			RGBBYTE **pixel = (RGBBYTE**)img.GetPixels();
			RGBBYTE **tpixel = (RGBBYTE**)timg.GetPixels();
			int *Rarr = new int[h*w];
			int *Garr = new int[h*w];
			int *Barr = new int[h*w];

			for (y = 0; y < height; y++) {
				for (x = 0; x < width; x++) {
					cnt = 0;
					for (fy = 0; fy < h; fy++) {
						for (fx = 0; fx < w; fx++) {
							Rarr[cnt] = tpixel[y + fy][x + fx].r;	//save in array 
							Garr[cnt] = tpixel[y + fy][x + fx].g;	//save in array 
							Barr[cnt] = tpixel[y + fy][x + fx].b;	//save in array 
							cnt++;								//increase cnt 
						}
					}
					//sort 
					std::sort(Rarr, Rarr + (h*w));
					std::sort(Garr, Garr + (h*w));
					std::sort(Barr, Barr + (h*w));
					pixel[y][x].r = (byte)limit(Rarr[mid]);
					pixel[y][x].g = (byte)limit(Garr[mid]);
					pixel[y][x].b = (byte)limit(Barr[mid]);
				}
			}
			delete[] Barr;
			delete[] Garr;
			delete[] Rarr;
		}
		else
			return false;
	}
	else if (depth == CV_16U || depth == CV_16S) {
		if (channels == 1) {
			WORD **pixel = (WORD**)img.GetPixels();
			WORD **tpixel = (WORD**)timg.GetPixels();

			for (y = 0; y < height; y++) {
				for (x = 0; x < width; x++) {
					cnt = 0;
					for (fy = 0; fy < h; fy++) {
						for (fx = 0; fx < w; fx++) {
							arr[cnt] = tpixel[y + fy][x + fx];	//save in array 
							cnt++;								//increase cnt 
						}
					}
					//sort 
					std::sort(arr, arr + (h*w));
					pixel[y][x] = (WORD)limit16bit(arr[mid]);
				}
			}
		}
		else if (channels == 3) {

		}
		else
			return false;
	}
	else
		return false;

	delete[] arr;
	return true;
}

bool IpFilterMed(Mat & img, int h, int w)
{
	if (!img.data)	return false;
	if (h < 3 || h % 2 == 0)	return false;
	if (w < 3 || w % 2 == 0)	return false;
	Size msize(w, h);
	Point h_m = msize / 2;

	int x, y, mid = (w*h) / 2;	//for�� x,y , �߽���ġ
	Point start;
	Mat mask, sort_m, one_row;

	//border�ۿ��� ���α׷� ���� �ʾҴ�. 
	//�ϴ�b border����� ū �̹����� ������!
	Size Newsize(img.size() + Size(h-1, w-1));
	Mat timg = Mat(Newsize, img.type(), Scalar(0));
	Rect roi = Rect(h_m, img.size());
	img.copyTo(timg(roi));


	if (img.depth() == CV_8U) {
		if (img.channels() == 1) {
			for (y = 0; y < timg.rows-msize.height+1; y++) {
				for (x = 0; x < timg.cols-msize.width+1; x++) {
					start = Point(x, y);	//kernel�� �»�ܺ��� �����Ұ��̴�
					Rect roi(start, msize);
					timg(roi).copyTo(mask);		//w*h��ŭ�� ����ũ�� ����Ȱ���!

					one_row = mask.reshape(1, 1);				//1�� ��ķ� ������ 
					cv::sort(one_row, sort_m, SORT_EVERY_ROW);	//�� ���� ���� ����
					img.at<uchar>(y, x) = sort_m.at<uchar>(mid);	//!!!!!!���� ������ ��� �ٲ��. border�̹��� ������ �� ū ����� ����� ���� �ڿ� img�� ��������...
				}
			}
		}
		else
			return false;
	}
	else
		return false;

	return true;
}

bool IpFilterAdpMed(MyIPImage & img, int maxsize)
{
	if (!img.GetPixels())	return false;
	if (maxsize % 2 == 0) 	return false;
	if (maxsize < 3)		return false;
	if (maxsize > 11)		return false;

	int depth = img.GetDepth();
	int channels = img.GetChannels();
	int width = img.GetWidth();
	int height = img.GetHeight();

	int mVal, max, min, size;
	int x, y, fx, fy;

	int *arr;
	arr = new int[maxsize*maxsize];

	MyIPImage tempimg;
	if (!IpFillBorder(img, tempimg, maxsize, maxsize, IOP_BORDER_REPLICATE))	return false;
									
	if (depth == CV_8U || depth == CV_8S) {

		if (channels == 1) {
			byte** pixel = (byte**)img.GetPixels();
			byte** tpixel = (byte**)tempimg.GetPixels();
			for (y = 0; y < height; y++) {
				for (x = 0; x < width; x++) {
					for (size = 3; size <= maxsize; size += 2) {
						int sizePOW = size*size;
						int arrcnt = 0;
						for (fy = 0; fy < size; fy++) {
							for (fx = 0; fx < size; fx++) {
								int offset = maxsize / 2 - size / 2;
								arr[arrcnt] = tpixel[offset + y + fy][offset + x + fx];
								arrcnt++;
							}
						}
						std::sort(arr, arr + sizePOW);			//������������ �迭 ����
						mVal = arr[sizePOW / 2];					//2. Median �� ���ϱ� 
						min = arr[0];
						max = arr[sizePOW - 1];
						if (mVal == min || mVal == max) {			//3. mVal�� �ִ�,�ּڰ��� ������ 
							if (size < maxsize)						//4. ����� max����� �ɶ����� 
								continue;
						}

						if (pixel[y][x] == min || pixel[y][x] == max) 
							pixel[y][x] = (byte)limit(mVal);
						break;
					}
				}
			}
		}
		else if (channels == 3) {
			RGBBYTE** pixel = (RGBBYTE**)img.GetPixels();
			RGBBYTE** tpixel = (RGBBYTE**)tempimg.GetPixels();

			int *Rarr = new int[maxsize*maxsize];
			int *Garr = new int[maxsize*maxsize];
			int *Barr = new int[maxsize*maxsize];
			int RmVal, Rmax, Rmin;
			int GmVal, Gmax, Gmin;
			int BmVal, Bmax, Bmin;

			for (y = 0; y < height; y++) {
				for (x = 0; x < width; x++) {
					bool Rfin(false), Gfin(false), Bfin(false);
					for (size = 3; size <= maxsize; size += 2) {
						bool Rcontinue(false), Gcontinue(false), Bcontinue(false);
						int sizePOW = size*size;
						int arrcnt = 0;
						for (fy = 0; fy < size; fy++) {
							for (fx = 0; fx < size; fx++) {
								int offset = maxsize / 2 - size / 2;
								Rarr[arrcnt] = tpixel[offset + y + fy][offset + x + fx].r;
								Garr[arrcnt] = tpixel[offset + y + fy][offset + x + fx].g;
								Barr[arrcnt] = tpixel[offset + y + fy][offset + x + fx].b;
								arrcnt++;
							}
						}
						std::sort(Rarr, Rarr + sizePOW);			//������������ �迭 ����
						std::sort(Garr, Garr + sizePOW);			//������������ �迭 ����
						std::sort(Barr, Barr + sizePOW);			//������������ �迭 ����
						RmVal = Rarr[sizePOW / 2];					//2. Median �� ���ϱ� 
						GmVal = Garr[sizePOW / 2];					//2. Median �� ���ϱ� 
						BmVal = Barr[sizePOW / 2];					//2. Median �� ���ϱ� 
						Rmax = Rarr[sizePOW - 1];
						Gmax = Garr[sizePOW - 1];
						Bmax = Barr[sizePOW - 1];
						Rmin = Rarr[0];
						Gmin = Garr[0];
						Bmin = Barr[0];
						
						if (Rfin == false) {
							if (RmVal == Rmin || RmVal == Rmax) {		//3. mVal�� �ִ�,�ּڰ��� ������ 
								if (size < maxsize)						//4. ����� max����� �ɶ����� 
									Rcontinue = true;
							}
						}
						if (Gfin == false) {
							if (GmVal == Gmin || GmVal == Gmax) {		//3. mVal�� �ִ�,�ּڰ��� ������ 
								if (size < maxsize)						//4. ����� max����� �ɶ����� 
									Gcontinue = true;
							}
						}
						if (Bfin == false) {
							if (BmVal == Bmin || BmVal == Bmax) {		//3. mVal�� �ִ�,�ּڰ��� ������ 
								if (size < maxsize)						//4. ����� max����� �ɶ����� 
									Bcontinue = true;
							}
						}

						//kernel size�� max size���� Ŀ���� �Ʒ� ���ǹ��� ���� �ȼ��� ���� 
						if (Rfin == false && Rcontinue == false) {					//R���� �������� �ʾҰ�, R���� kernel size�����Ͽ� ����� �ʿ������ 
							if (pixel[y][x].r == Rmin || pixel[y][x].r == Rmax)		//�ȼ����� kernel���� �ִ� or �ּڰ��̶� ������
								pixel[y][x].r = (byte)limit(RmVal);					//median���� �ȼ����� ���� 
																					//else ����) �� �ܴ� �ȼ��� ���� 
							Rfin = true;											//R�� ������ٰ� Flag -> true 
						}
						if (Gfin == false && Gcontinue == false) {					//R���� �������� �ʾҰ�, R���� kernel size�����Ͽ� ����� �ʿ������ 
							if (pixel[y][x].g == Gmin || pixel[y][x].g == Gmax)		//�ȼ����� kernel���� �ִ� or �ּڰ��̶� ������
								pixel[y][x].g = (byte)limit(GmVal);					//median���� �ȼ����� ���� 
																					//else ����) �� �ܴ� �ȼ��� ���� 
							Gfin = true;											//R�� ������ٰ� Flag -> true 
						}
						if (Bfin == false && Bcontinue == false) {					//R���� �������� �ʾҰ�, R���� kernel size�����Ͽ� ����� �ʿ������ 
							if (pixel[y][x].b == Bmin || pixel[y][x].b == Bmax)		//�ȼ����� kernel���� �ִ� or �ּڰ��̶� ������
								pixel[y][x].b = (byte)limit(BmVal);					//median���� �ȼ����� ���� 
																					//else ����) �� �ܴ� �ȼ��� ���� 
							Bfin = true;											//R�� ������ٰ� Flag -> true 
						}

						if (Rfin && Gfin && Bfin)									//��� ���� ������ ��ٸ�
							break;													//break
						else														//�ϳ��� ������ ���� ������ 
							continue;												//kernel size�����Ͽ� ��� 
					}
				}
			}
		}
		else
			return false;
	}
	else if (depth == CV_16U || depth == CV_16S) {
		if (channels == 1) {
			WORD** pixel = (WORD**)img.GetPixels();
			WORD** tpixel = (WORD**)tempimg.GetPixels();
			for (y = 0; y < height; y++) {
				for (x = 0; x < width; x++) {
					for (size = 3; size <= maxsize; size += 2) {
						int sizePOW = size*size;
						int arrcnt = 0;
						for (fy = 0; fy < size; fy++) {
							for (fx = 0; fx < size; fx++) {
								int offset = maxsize / 2 - size / 2;
								arr[arrcnt] = tpixel[offset + y + fy][offset + x + fx];
								arrcnt++;
							}
						}
						std::sort(arr, arr + sizePOW);			//������������ �迭 ����
						mVal = arr[sizePOW / 2];					//2. Median �� ���ϱ� 
						min = arr[0];
						max = arr[sizePOW - 1];
						if (mVal == min || mVal == max) {			//3. mVal�� �ִ�,�ּڰ��� ������ 
							if (size < maxsize)						//4. ����� max����� �ɶ����� 
								continue;
						}

						if (pixel[y][x] == min || pixel[y][x] == max)
							pixel[y][x] = (WORD)limit16bit(mVal);
						break;
					}
				}
			}
		}
		else if (channels == 3) {
			return false;
		}
		else
			return false;
	}
	else return false;

	delete[] arr;

	return true;
}

bool IpFilterAdpMed(Mat & img, int maxsize)
{
	if (!img.data)	return false;
	if (maxsize % 2 == 0) 	return false;
	if (maxsize < 3)		return false;
	if (maxsize > 11)		return false;


	int depth = img.depth();
	int channels = img.channels();
	int width = img.cols;
	int height = img.rows;

	int x, y, size;	//for�� x,y , �߽���ġ
	Point start;
	Mat mask, sort_m, one_row;
	int mVal, min, max;

	//border�ۿ��� ���α׷� ���� �ʾҴ�. 
	//�ϴ�b border����� ū �̹����� ������!
	Mat timg;
	copyMakeBorder(img, timg, maxsize / 2, maxsize / 2, maxsize / 2, maxsize / 2, BORDER_REFLECT);

	if (depth == CV_8U) {
		if (channels == 1) {
			for (y = 0; y < height; y++) {
				for (x = 0; x < width; x++) {
					for (size = 3; size <= maxsize; size += 2) {
						int pixel = img.at<uchar>(y, x);
						int offset = maxsize / 2 - size / 2;
						start = Point(offset + x, offset + y);		//kernel�� �»�ܺ��� �����Ұ��̴�
						Rect roi(start, Size(size,size));
						timg(roi).copyTo(mask);						//w*h��ŭ�� ����ũ�� ����Ȱ���!

						one_row = mask.reshape(1, 1);				//1�� ��ķ� ������ 
						cv::sort(one_row, sort_m, SORT_EVERY_ROW);	//�� ���� ���� ����
						mVal = sort_m.at<uchar>((size*size) / 2);	//!!!!!!���� ������ ��� �ٲ��. border�̹��� ������ �� ū ����� ����� ���� �ڿ� img�� ��������...
						min = sort_m.at<uchar>(0);
						max = sort_m.at<uchar>((size*size) - 1);

						if (mVal == min || mVal == max) {			//3. mVal�� �ִ�,�ּڰ��� ������ 
							if (size < maxsize)						//4. ����� max����� �ɶ����� 
								continue;
						}

						if (pixel == min || pixel == max)
							img.at<uchar>(y, x) = limit<uchar>(mVal);
						break;
					}
				}
			}
		}
		else if (channels == 3) {
			int RmVal, Rmin, Rmax;
			int GmVal, Gmin, Gmax;
			int BmVal, Bmin, Bmax;
			RGBBYTE pixel;
			for (y = 0; y < height; y++) {
				for (x = 0; x < width; x++) {
					bool Rfin(false), Gfin(false), Bfin(false);
					for (size = 3; size <= maxsize; size += 2) {
						bool Rcontinue(false), Gcontinue(false), Bcontinue(false);
						pixel.r = img.at<Vec3b>(y, x)[2];
						pixel.g = img.at<Vec3b>(y, x)[1];
						pixel.b = img.at<Vec3b>(y, x)[0];

						int offset = maxsize / 2 - size / 2;
						start = Point(offset + x, offset + y);		//kernel�� �»�ܺ��� �����Ұ��̴�
						Rect roi(start, Size(size, size));
						timg(roi).copyTo(mask);						//w*h��ŭ�� ����ũ�� ����Ȱ���!
						Mat bgr[3], sortbgr[3];
						split(mask, bgr);

						sortbgr[0] = bgr[0].reshape(1, 1);				//1�� ��ķ� ������ 
						sortbgr[1] = bgr[1].reshape(1, 1);				//1�� ��ķ� ������ 
						sortbgr[2] = bgr[2].reshape(1, 1);				//1�� ��ķ� ������ 

						cv::sort(sortbgr[0], sortbgr[0], SORT_EVERY_ROW);	//�� ���� ���� ����
						cv::sort(sortbgr[1], sortbgr[1], SORT_EVERY_ROW);	//�� ���� ���� ����
						cv::sort(sortbgr[2], sortbgr[2], SORT_EVERY_ROW);	//�� ���� ���� ����

						RmVal = sortbgr[2].at<uchar>((size*size) / 2);	//!!!!!!���� ������ ��� �ٲ��. border�̹��� ������ �� ū ����� ����� ���� �ڿ� img�� ��������...
						GmVal = sortbgr[1].at<uchar>((size*size) / 2);
						BmVal = sortbgr[0].at<uchar>((size*size) / 2);
						Rmax = sortbgr[2].at<uchar>((size*size) - 1);
						Gmax = sortbgr[1].at<uchar>((size*size) - 1);
						Bmax = sortbgr[0].at<uchar>((size*size) - 1);
						Rmin = sortbgr[2].at<uchar>(0);
						Gmin = sortbgr[1].at<uchar>(0);
						Bmin = sortbgr[0].at<uchar>(0);

						if (Rfin == false) {
							if (RmVal == Rmin || RmVal == Rmax) {		//3. mVal�� �ִ�,�ּڰ��� ������ 
								if (size < maxsize)						//4. ����� max����� �ɶ����� 
									Rcontinue = true;
							}
						}
						if (Gfin == false) {
							if (GmVal == Gmin || GmVal == Gmax) {		//3. mVal�� �ִ�,�ּڰ��� ������ 
								if (size < maxsize)						//4. ����� max����� �ɶ����� 
									Gcontinue = true;
							}
						}
						if (Bfin == false) {
							if (BmVal == Bmin || BmVal == Bmax) {		//3. mVal�� �ִ�,�ּڰ��� ������ 
								if (size < maxsize)						//4. ����� max����� �ɶ����� 
									Bcontinue = true;
							}
						}

						//kernel size�� max size���� Ŀ���� �Ʒ� ���ǹ��� ���� �ȼ��� ���� 
						if (Rfin == false && Rcontinue == false) {										//R���� �������� �ʾҰ�, R���� kernel size�����Ͽ� ����� �ʿ������ 
							if (pixel.r == Rmin || pixel.r == Rmax)										//�ȼ����� kernel���� �ִ� or �ּڰ��̶� ������
								img.at<Vec3b>(y, x)[2] = (byte)limit(RmVal);							//median���� �ȼ����� ���� 
																										//else ����) �� �ܴ� �ȼ��� ���� 
							Rfin = true;																//R�� ������ٰ� Flag -> true 
						}
						if (Gfin == false && Gcontinue == false) {										//R���� �������� �ʾҰ�, R���� kernel size�����Ͽ� ����� �ʿ������ 
							if (pixel.g == Gmin || pixel.g == Gmax)										//�ȼ����� kernel���� �ִ� or �ּڰ��̶� ������
								img.at<Vec3b>(y, x)[1] = (byte)limit(GmVal);							//median���� �ȼ����� ���� 
																										//else ����) �� �ܴ� �ȼ��� ���� 
							Gfin = true;																//R�� ������ٰ� Flag -> true 
						}
						if (Bfin == false && Bcontinue == false) {										//R���� �������� �ʾҰ�, R���� kernel size�����Ͽ� ����� �ʿ������ 
							if (pixel.b == Bmin || pixel.b == Bmax)		//�ȼ����� kernel���� �ִ� or �ּڰ��̶� ������
								img.at<Vec3b>(y, x)[0] = (byte)limit(BmVal);							//median���� �ȼ����� ���� 
																									//else ����) �� �ܴ� �ȼ��� ���� 
							Bfin = true;																//R�� ������ٰ� Flag -> true 
						}

						if (Rfin && Gfin && Bfin)									//��� ���� ������ ��ٸ�
							break;													//break
						else														//�ϳ��� ������ ���� ������ 
							continue;												//kernel size�����Ͽ� ��� 
					}
				}
			}
		}
	}
	else if (depth == CV_16U) {
		if (channels == 1) {
			for (y = 0; y < height; y++) {
				for (x = 0; x < width; x++) {
					for (size = 3; size <= maxsize; size += 2) {
						int pixel = img.at<unsigned short>(y, x);
						int offset = maxsize / 2 - size / 2;
						start = Point(offset + x, offset + y);		//kernel�� �»�ܺ��� �����Ұ��̴�
						Rect roi(start, Size(size, size));
						timg(roi).copyTo(mask);						//w*h��ŭ�� ����ũ�� ����Ȱ���!

						one_row = mask.reshape(1, 1);				//1�� ��ķ� ������ 
						cv::sort(one_row, sort_m, SORT_EVERY_ROW);	//�� ���� ���� ����
						mVal = sort_m.at<unsigned short>((size*size) / 2);	//!!!!!!���� ������ ��� �ٲ��. border�̹��� ������ �� ū ����� ����� ���� �ڿ� img�� ��������...
						min = sort_m.at<unsigned short>(0);
						max = sort_m.at<unsigned short>((size*size) - 1);

						if (mVal == min || mVal == max) {			//3. mVal�� �ִ�,�ּڰ��� ������ 
							if (size < maxsize)						//4. ����� max����� �ɶ����� 
								continue;
						}

						if (pixel == min || pixel == max)
							img.at<unsigned short>(y, x) = limit<unsigned short>(mVal);
						break;
					}
				}
			}
		}
		else if (channels == 3) {

		}
		else
			return false;
	}
	else
		return false;

	return true;
}


bool IpFillBorder(MyIPImage & img, MyIPImage & oimg, int h, int w, int borderType, int value)
{
	if (!img.GetPixels())				return false;
	if ((h % 2 == 0) || (w % 2 == 0))	return false;

	int depth = img.GetDepth();
	int channels = img.GetChannels();
	int width = img.GetWidth();
	int height = img.GetHeight();

	int x, y;
	int kh2 = h / 2;					//kernel height / 2 (��, �Ʒ��� ���� �þ�� �ȼ� ��)
	int kw2 = w / 2;					//kernel  width / 2 (���ʿ����� ���� �þ�� �ȼ� ��)
	int newWidth = width + kw2 * 2;		//���� �����Ǵ� �̹����� Width
	int newHeight = height + kh2 * 2;	//���� �����Ǵ� �̹����� Height

	//kernel size�� �ݸ��ƾ� ��, �Ʒ�, ����, ���������� ū �ӽ� ������ �����.
	//borderType�� ���� ���� ü�� ���´�.

	if (!oimg.CreateImage(newHeight, newWidth, depth, channels)) return false;

	if (depth == CV_8U || depth == CV_8S) {
		if (channels == 1) {
			byte **pixel = (byte**)img.GetPixels();
			byte **opixel = (byte**)oimg.GetPixels();
			// *16��Ʈ�̱⿡ memset, memcpy ������ size�κп� 2�� �����־���. -> �� ���� ����� ������?
			//-> memset ���� �ȼ� ���� �������� ����
			switch (borderType)
			{
			case IOP_BORDER_CONSTANT:
				for (y = 0; y < newHeight; y++) {
					for (x = 0; x < newWidth; x++) {
						if (y >= kh2 && y < kh2 + height && x >= kw2 && x < kw2 + width)
							opixel[y][x] = pixel[y - kh2][x - kw2];
						else
							opixel[y][x] = value;
					}
				}
				break;

			case IOP_BORDER_REPLICATE:	//�����ڸ� ������ ���� 

										//TOP ~ kh2 
				for (y = 0; y < kh2; y++) {
					for (x = 0; x < kw2; x++)
						opixel[y][x] = pixel[0][0];					//�»�� 
					for (; x < kw2 + width; x++)
						opixel[y][x] = pixel[0][x - kw2];			//�߻�� 
					for (; x < newWidth; x++)
						opixel[y][x] = pixel[0][width - 1];			//���� 
				}

				//kh2 ~ kh2+height
				for (; y < height + kh2; y++) {
					int py = y - kh2;

					for (x = 0; x < kw2; x++)
						opixel[y][x] = pixel[py][0];					//���ߴ� 
					for (; x < kw2 + width; x++)
						opixel[y][x] = pixel[py][x - kw2];				//���ߴ� (�̹���)
					for (; x < newWidth; x++)
						opixel[y][x] = pixel[py][width - 1];			//���ߴ� 
				}

				//kh2+height ~ BOTTOM
				for (; y < newHeight; y++) {

					for (x = 0; x < kw2; x++)
						opixel[y][x] = pixel[height - 1][0];					//���ϴ� 
					for (; x < kw2 + width; x++)
						opixel[y][x] = pixel[height - 1][x - kw2];				//���ϴ� (�̹���)
					for (; x < newWidth; x++)
						opixel[y][x] = pixel[height - 1][width - 1];			//���ϴ� 
				}
				break;

			case IOP_BORDER_REFLECT:

				//TOP ~ kh2 
				for (y = 0; y < kh2; y++) {
					int py = kh2 - 1 - y;							//y�� ���� ����Ī ��ȭ�� 

					for (x = 0; x < newWidth; x++) {

						//�»�� 
						if (x < kw2) {
							int px = kw2 - 1 - x;					//x�� ���� ����Ī ��ȭ�� 
							opixel[y][x] = pixel[py][px];
						}

						//�߻�� 
						else if (x < kw2 + width) {
							int x0 = x - kw2;						//0���� �����ϴ� x
							opixel[y][x] = pixel[py][x0];
						}

						//����
						else {
							int x0 = x - kw2 - width;				//0���� �����ϴ� x
							int px = kw2 - 1 - x0;					//x�� ���� ����Ī ��ȭ�� 
							opixel[y][x] = pixel[py][width - kw2 + px];
						}
					}
				}

				//kh2 ~ kh2+height
				for (; y < height + kh2; y++) {
					int y0 = y - kh2;								//0���� �����ϴ� y
					int py = kh2 - 1 - y0;							//y�� ���� ����Ī ��ȭ�� 

					for (x = 0; x < newWidth; x++) {

						//���ߴ� 
						if (x < kw2) {
							int px = kw2 - 1 - x;					//x�� ���� ����Ī ��ȭ�� 
							opixel[y][x] = pixel[y0][px];
						}

						//���ߴ� 
						else if (x < kw2 + width) {
							int x0 = x - kw2;						//0���� �����ϴ� x
							opixel[y][x] = pixel[y0][x0];			//�̹���
						}

						//���ߴ�
						else {
							int x0 = x - kw2 - width;				//0���� �����ϴ� x
							int px = kw2 - 1 - x0;					//x�� ���� ����Ī ��ȭ�� 
							opixel[y][x] = pixel[y0][width - kw2 + px];
						}
					}
				}

				//kh2+height ~ BOTTOM
				for (; y < newHeight; y++) {
					int y0 = y - kh2 - height;						//0���� �����ϴ� y
					int py = kh2 - 1 - y0;							//y�� ���� ����Ī ��ȭ�� 

					for (x = 0; x < newWidth; x++) {

						//���ϴ� 
						if (x < kw2) {
							int px = kw2 - 1 - x;					//x�� ���� ����Ī ��ȭ�� 
							opixel[y][x] = pixel[height - kh2 + py][px];
						}

						//���ϴ� 
						else if (x < kw2 + width) {
							int x0 = x - kw2;						//0���� �����ϴ� x
							opixel[y][x] = pixel[height - kh2 + py][x0];
						}

						//���ϴ�
						else {
							int x0 = x - kw2 - width;				//0���� �����ϴ� x
							int px = kw2 - 1 - x0;					//x�� ���� ����Ī ��ȭ�� 
							opixel[y][x] = pixel[height - kh2 + py][width - kw2 + px];
						}

					}
				}
				break;

			case IOP_BORDER_WRAP:

				//TOP ~ kh2 
				for (y = 0; y < kh2; y++) {
					for (x = 0; x < kw2; x++)
						opixel[y][x] = pixel[height - kh2 + y][width - kw2 + x];		//�»�� 
					for (; x < kw2 + width; x++)
						opixel[y][x] = pixel[height - kh2 + y][x - kw2];				//�߻�� 
					for (; x < newWidth; x++)
						opixel[y][x] = pixel[height - kh2 + y][x - kw2 - width];		//���� 
				}

				//kh2 ~ kh2+height
				for (; y < height + kh2; y++) {
					int y0 = y - kh2;									//0���� �����ϴ� y -> y0

					for (x = 0; x < kw2; x++)
						opixel[y][x] = pixel[y0][width - kw2 + x];		//�»�� 
					for (; x < kw2 + width; x++)
						opixel[y][x] = pixel[y0][x - kw2];				//�߻�� 
					for (; x < newWidth; x++)
						opixel[y][x] = pixel[y0][x - kw2 - width];		//���� 
				}

				//kh2+height ~ BOTTOM
				for (; y < newHeight; y++) {
					int y0 = y - kh2 - height;							//0���� �����ϴ� y -> y0

					for (x = 0; x < kw2; x++)
						opixel[y][x] = pixel[y0][width - kw2 + x];		//�»�� 
					for (; x < kw2 + width; x++)
						opixel[y][x] = pixel[y0][x - kw2];				//�߻�� 
					for (; x < newWidth; x++)
						opixel[y][x] = pixel[y0][x - kw2 - width];		//���� 
				}

				break;

			default:
				return false;
			}
		}
// 		if (channels == 1) {
// 			byte **pixel = (byte**)img.GetPixels();
// 			byte **opixel = (byte**)oimg.GetPixels();
// 
// 			switch (borderType)
// 			{
// 			case IOP_BORDER_CONSTANT:
// 				for (y = 0; y < newHeight; y++)
// 					memset(opixel[y], value, newWidth);
// 				for (y = kh2; y < height + kh2; y++)
// 					memcpy(opixel[y] + kw2, pixel[y - kh2], width);
// 				break;
// 
// 			case IOP_BORDER_REPLICATE:	//�����ڸ� ������ ���� 
// 				
// 				//TOP ~ kh2 
// 				for (y = 0; y < kh2; y++) {		
// 					memset(opixel[y], pixel[0][0], kw2);								//�»�� 
// 					memcpy(opixel[y] + kw2, pixel[0], width);							//�߻��
// 					memset(opixel[y] + kw2 + width, pixel[0][width - 1], kw2);			//����
// 				}
// 
// 				//kh2 ~ kh2+height
// 				for (; y < height + kh2; y++) {
// 					int py = y - kh2;
// 					memset(opixel[y], pixel[py][0], kw2);								//���ߴ�
// 					memcpy(opixel[y] + kw2, pixel[py], width);							//�߾�(�̹���)
// 					memset(opixel[y] + kw2 + width, pixel[py][width - 1], kw2);			//���ߴ� 
// 				}
// 
// 				//kh2+height ~ BOTTOM
// 				for (; y < newHeight; y++) {
// 					memset(opixel[y], pixel[height - 1][0], kw2);						//���ϴ� 
// 					memcpy(opixel[y] + kw2, pixel[height - 1], width);					//���ϴ�
// 					memset(opixel[y] + kw2 + width, pixel[height - 1][width - 1], kw2);	//���ϴ�
// 				}
// 
// 				/*//TOP ~ kh2 
// 				for (y = 0; y < kh2; y++) {		
// 					memset(opixel[y], pixel[0][0], kw2);								//�»�� 
// 					memcpy(opixel[y] + kw2, pixel[0], width);							//�߻��
// 					memset(opixel[y] + kw2 + width, pixel[0][width - 1], kw2);			//����
// 				}
// 
// 				//kh2 ~ kh2+height
// 				for (; y < height + kh2; y++) {
// 					int py = y - kh2;
// 					memset(opixel[y], pixel[py][0], kw2);								//���ߴ�
// 					memcpy(opixel[y] + kw2, pixel[py], width);							//�߾�(�̹���)
// 					memset(opixel[y] + kw2 + width, pixel[py][width - 1], kw2);			//���ߴ� 
// 				}
// 
// 				//kh2+height ~ BOTTOM
// 				for (; y < newHeight; y++) {
// 					memset(opixel[y], pixel[height - 1][0], kw2);						//���ϴ� 
// 					memcpy(opixel[y] + kw2, pixel[height - 1], width);					//���ϴ�
// 					memset(opixel[y] + kw2 + width, pixel[height - 1][width - 1], kw2);	//���ϴ�
// 				}*/
// 				
// 				/*
// 				//����
// 				for (y = 0; y < kh2; y++) {	
// 					for (x = 0; x < kw2; x++)
// 						opixel[y][x] = pixel[0][0];
// 					for (; x < width + kw2; x++)
// 						opixel[y][x] = pixel[0][x - kw2];
// 					for (; x < newWidth; x++)
// 						opixel[y][x] = pixel[0][width - 1];
// 				}
// 				//�߰�
// 				for (; y < height + kh2; y++) {
// 					for (x = 0; x < kw2; x++)
// 						opixel[y][x] = pixel[y-kh2][0];
// 					for (; x < width + kw2; x++)
// 						opixel[y][x] = pixel[y-kh2][x - kw2];
// 					for (; x < newWidth; x++)
// 						opixel[y][x] = pixel[y-kh2][width - 1];
// 				}
// 				//����
// 				for (; y < newHeight; y++) {
// 					for (x = 0; x < kw2; x++)
// 						opixel[y][x] = pixel[height-1][0];
// 					for (; x < width + kw2; x++)
// 						opixel[y][x] = pixel[height-1][x - kw2];
// 					for (; x < newWidth; x++)
// 						opixel[y][x] = pixel[height-1][width - 1];
// 				}
// 				*/
// 				break;
// 
// 			case IOP_BORDER_REFLECT:
// 				
// 				//TOP ~ kh2 
// 				for (y = 0; y < kh2; y++) {
// 					int py = kw2 - 1 - y;									//��Ī�����ϴ� y
// 					memcpy(opixel[y] + kw2, pixel[kh2 - 1 - y], width);											//�߻��
// 					for (x = 0; x < kw2; x++) {
// 						int px = kh2 - 1 - x;								//��Ī�����ϴ� x
// 						memset(opixel[y] + x, pixel[py][px], 1);												//�»�� 
// 						memset(opixel[y] + x + kw2 + width, pixel[py][width - kw2 + px], 1);					//����
// 					}
// 				}
// 
// 				//kh2 ~ kh2+height
// 				for (; y < height + kh2; y++) {
// 					int y0 = y - kh2;										//0���� �����ϴ� y -> y0
// 					memcpy(opixel[y] + kw2, pixel[y0], width);													//�߾�(�̹���)
// 					for (x = 0; x < kw2; x++) {
// 						memset(opixel[y] + x, pixel[y0][kw2 - 1 - x], 1);										//���ߴ�
// 						memset(opixel[y] + x + kw2 + width, pixel[y0][width - 1 - x], 1);						//���ߴ� 
// 					}
// 				}
// 
// 				//kh2+height ~ BOTTOM
// 				for (; y < newHeight; y++) {
// 					int y0 = y - kh2 - height;								//0���� �����ϴ� y -> y0
// 					int py = kw2 - 1 - y0;									//��Ī �����ϴ� x
// 					memcpy(opixel[y] + kw2, pixel[height - 1 - y0], width);										//���ϴ�
// 					for (x = 0; x < kw2; x++) {
// 						int px = kh2 - 1 - x;								//��Ī �����ϴ� y
// 						memset(opixel[y] + x, pixel[height - kh2 + py][px], 1);									//���ϴ� 
// 						memset(opixel[y] + x + kw2 + width, pixel[height - kh2 + py][width - kw2 + px], 1);		//���ϴ�
// 					}
// 				}
// 				break;
// 
// 			case IOP_BORDER_WRAP:
// 
// 				//TOP ~ kh2 
// 				for (y = 0; y < kh2; y++) {
// 					memcpy(opixel[y], pixel[height - kh2 + y] + width - kw2, kw2);		//�»��
// 					memcpy(opixel[y] + kw2, pixel[height - kh2 + y], width);			//�߻��
// 					memcpy(opixel[y] + kw2 + width, pixel[height - kh2 + y], kw2);		//����
// 				}
// 
// 				//kh2 ~ kh2+height
// 				for (; y < height + kh2; y++) {
// 					int y0 = y - kh2;													//0���� �����ϴ� y -> y0
// 					memcpy(opixel[y], pixel[y0] + width - kw2, kw2);					//���ߴ�
// 					memcpy(opixel[y] + kw2, pixel[y0], width);							//�߾�(�̹���)
// 					memcpy(opixel[y] + kw2 + width, pixel[y0], kw2);					//���ߴ�
// 				}
// 
// 				//kh2+height ~ BOTTOM
// 				for (; y < newHeight; y++) {
// 					int y0 = y - kh2 - height;
// 					memcpy(opixel[y], pixel[y0] + width - kw2, kw2);					//���ϴ�
// 					memcpy(opixel[y] + kw2, pixel[y0], width);							//���ϴ�
// 					memcpy(opixel[y] + kw2 + width, pixel[y0], kw2);					//���ϴ�
// 				}
// 
// 				break;
// 
// 			default:
// 				return false;
// 			}
// 
// 		}
		else if (channels == 3) {
			RGBBYTE **pixel = (RGBBYTE**)img.GetPixels();
			RGBBYTE **opixel = (RGBBYTE**)oimg.GetPixels();
			// *16��Ʈ�̱⿡ memset, memcpy ������ size�κп� 2�� �����־���. -> �� ���� ����� ������?
			//-> memset ���� �ȼ� ���� �������� ����
			switch (borderType)
			{
			case IOP_BORDER_CONSTANT:
				for (y = 0; y < newHeight; y++) {
					for (x = 0; x < newWidth; x++) {
						if (y >= kh2 && y < kh2 + height && x >= kw2 && x < kw2 + width) {
							opixel[y][x].r = pixel[y - kh2][x - kw2].r;
							opixel[y][x].g = pixel[y - kh2][x - kw2].g;
							opixel[y][x].b = pixel[y - kh2][x - kw2].b;
						}
						else {
							opixel[y][x].r = value;
							opixel[y][x].g = value;
							opixel[y][x].b = value;
						}
					}
				}
				break;

			case IOP_BORDER_REPLICATE:	//�����ڸ� ������ ���� 

				//TOP ~ kh2 
				for (y = 0; y < kh2; y++) {
					for (x = 0; x < kw2; x++) {
						opixel[y][x].r = pixel[0][0].r;					//�»�� 
						opixel[y][x].g = pixel[0][0].g;					//�»�� 
						opixel[y][x].b = pixel[0][0].b;					//�»�� 
					}
					for (; x < kw2 + width; x++) {
						opixel[y][x].r = pixel[0][x - kw2].r;			//�߻�� 
						opixel[y][x].g = pixel[0][x - kw2].g;			//�߻�� 
						opixel[y][x].b = pixel[0][x - kw2].b;			//�߻�� 
					}
					for (; x < newWidth; x++) {
						opixel[y][x].r = pixel[0][width - 1].r;			//���� 
						opixel[y][x].g = pixel[0][width - 1].g;			//���� 
						opixel[y][x].b = pixel[0][width - 1].b;			//���� 
					}
				}

				//kh2 ~ kh2+height
				for (; y < height + kh2; y++) {
					int py = y - kh2;

					for (x = 0; x < kw2; x++) {
						opixel[y][x].r = pixel[py][0].r;					//���ߴ� 
						opixel[y][x].g = pixel[py][0].g;					//���ߴ� 
						opixel[y][x].b = pixel[py][0].b;					//���ߴ� 
					}
					for (; x < kw2 + width; x++) {
						opixel[y][x].r = pixel[py][x - kw2].r;				//���ߴ� (�̹���)
						opixel[y][x].g = pixel[py][x - kw2].g;				//���ߴ� (�̹���)
						opixel[y][x].b = pixel[py][x - kw2].b;				//���ߴ� (�̹���)
					}
					for (; x < newWidth; x++) {
						opixel[y][x].r = pixel[py][width - 1].r;			//���ߴ� 
						opixel[y][x].g = pixel[py][width - 1].g;			//���ߴ� 
						opixel[y][x].b = pixel[py][width - 1].b;			//���ߴ� 
					}
				}

				//kh2+height ~ BOTTOM
				for (; y < newHeight; y++) {

					for (x = 0; x < kw2; x++) {
						opixel[y][x].r = pixel[height - 1][0].r;					//���ϴ� 
						opixel[y][x].g = pixel[height - 1][0].g;					//���ϴ� 
						opixel[y][x].b = pixel[height - 1][0].b;					//���ϴ� 
					}
					for (; x < kw2 + width; x++) {
						opixel[y][x].r = pixel[height - 1][x - kw2].r;				//���ϴ�
						opixel[y][x].g = pixel[height - 1][x - kw2].g;				//���ϴ�
						opixel[y][x].b = pixel[height - 1][x - kw2].b;				//���ϴ�
					}
					for (; x < newWidth; x++) {
						opixel[y][x].r = pixel[height - 1][width - 1].r;			//���ϴ� 
						opixel[y][x].g = pixel[height - 1][width - 1].g;			//���ϴ� 
						opixel[y][x].b = pixel[height - 1][width - 1].b;			//���ϴ� 
					}
				}
				break;

			case IOP_BORDER_REFLECT:

				//TOP ~ kh2 
				for (y = 0; y < kh2; y++) {
					int py = kh2 - 1 - y;							//y�� ���� ����Ī ��ȭ�� 

					for (x = 0; x < newWidth; x++) {

						//�»�� 
						if (x < kw2) {
							int px = kw2 - 1 - x;					//x�� ���� ����Ī ��ȭ�� 
							opixel[y][x].r = pixel[py][px].r;
							opixel[y][x].g = pixel[py][px].g;
							opixel[y][x].b = pixel[py][px].b;
						}

						//�߻�� 
						else if (x < kw2 + width) {
							int x0 = x - kw2;						//0���� �����ϴ� x
							opixel[y][x].r = pixel[py][x0].r;
							opixel[y][x].g = pixel[py][x0].g;
							opixel[y][x].b = pixel[py][x0].b;
						}

						//����
						else {
							int x0 = x - kw2 - width;				//0���� �����ϴ� x
							int px = kw2 - 1 - x0;					//x�� ���� ����Ī ��ȭ�� 
							opixel[y][x].r = pixel[py][width - kw2 + px].r;
							opixel[y][x].g = pixel[py][width - kw2 + px].g;
							opixel[y][x].b = pixel[py][width - kw2 + px].b;
						}
					}
				}

				//kh2 ~ kh2+height
				for (; y < height + kh2; y++) {
					int y0 = y - kh2;								//0���� �����ϴ� y
					int py = kh2 - 1 - y0;							//y�� ���� ����Ī ��ȭ�� 

					for (x = 0; x < newWidth; x++) {

						//���ߴ� 
						if (x < kw2) {
							int px = kw2 - 1 - x;					//x�� ���� ����Ī ��ȭ�� 
							opixel[y][x].r = pixel[y0][px].r;
							opixel[y][x].g = pixel[y0][px].g;
							opixel[y][x].b = pixel[y0][px].b;
						}

						//���ߴ� 
						else if (x < kw2 + width) {
							int x0 = x - kw2;						//0���� �����ϴ� x
							opixel[y][x].r = pixel[y0][x0].r;
							opixel[y][x].g = pixel[y0][x0].g;
							opixel[y][x].b = pixel[y0][x0].b;
						}

						//���ߴ�
						else {
							int x0 = x - kw2 - width;				//0���� �����ϴ� x
							int px = kw2 - 1 - x0;					//x�� ���� ����Ī ��ȭ�� 
							opixel[y][x].r = pixel[y0][width - kw2 + px].r;
							opixel[y][x].g = pixel[y0][width - kw2 + px].g;
							opixel[y][x].b = pixel[y0][width - kw2 + px].b;
						}
					}
				}

				//kh2+height ~ BOTTOM
				for (; y < newHeight; y++) {
					int y0 = y - kh2 - height;						//0���� �����ϴ� y
					int py = kh2 - 1 - y0;							//y�� ���� ����Ī ��ȭ�� 

					for (x = 0; x < newWidth; x++) {

						//���ϴ� 
						if (x < kw2) {
							int px = kw2 - 1 - x;					//x�� ���� ����Ī ��ȭ�� 
							opixel[y][x].r = pixel[height - kh2 + py][px].r;
							opixel[y][x].g = pixel[height - kh2 + py][px].g;
							opixel[y][x].b = pixel[height - kh2 + py][px].b;
						}

						//���ϴ� 
						else if (x < kw2 + width) {
							int x0 = x - kw2;						//0���� �����ϴ� x
							opixel[y][x].r = pixel[height - kh2 + py][x0].r;
							opixel[y][x].g = pixel[height - kh2 + py][x0].g;
							opixel[y][x].b = pixel[height - kh2 + py][x0].b;
						}

						//���ϴ�
						else {
							int x0 = x - kw2 - width;				//0���� �����ϴ� x
							int px = kw2 - 1 - x0;					//x�� ���� ����Ī ��ȭ�� 
							opixel[y][x].r = pixel[height - kh2 + py][width - kw2 + px].r;
							opixel[y][x].g = pixel[height - kh2 + py][width - kw2 + px].g;
							opixel[y][x].b = pixel[height - kh2 + py][width - kw2 + px].b;
						}

					}
				}
				break;

			case IOP_BORDER_WRAP:

				//TOP ~ kh2 
				for (y = 0; y < kh2; y++) {
					for (x = 0; x < kw2; x++) {
						opixel[y][x].r = pixel[height - kh2 + y][width - kw2 + x].r;		//�»�� 
						opixel[y][x].g = pixel[height - kh2 + y][width - kw2 + x].g;		//�»�� 
						opixel[y][x].b = pixel[height - kh2 + y][width - kw2 + x].b;		//�»�� 
					}
					for (; x < kw2 + width; x++) {
						opixel[y][x].r = pixel[height - kh2 + y][x - kw2].r;				//�߻�� 
						opixel[y][x].g = pixel[height - kh2 + y][x - kw2].g;				//�߻�� 
						opixel[y][x].b = pixel[height - kh2 + y][x - kw2].b;				//�߻�� 
					}
					for (; x < newWidth; x++) {
						opixel[y][x].r = pixel[height - kh2 + y][x - kw2 - width].r;		//���� 
						opixel[y][x].g = pixel[height - kh2 + y][x - kw2 - width].g;		//���� 
						opixel[y][x].b = pixel[height - kh2 + y][x - kw2 - width].b;		//���� 
					}
				}

				//kh2 ~ kh2+height
				for (; y < height + kh2; y++) {
					int y0 = y - kh2;									//0���� �����ϴ� y -> y0

					for (x = 0; x < kw2; x++) {
						opixel[y][x].r = pixel[y0][width - kw2 + x].r;		//�»�� 
						opixel[y][x].g = pixel[y0][width - kw2 + x].g;		//�»�� 
						opixel[y][x].b = pixel[y0][width - kw2 + x].b;		//�»�� 
					}
					for (; x < kw2 + width; x++) {
						opixel[y][x].r = pixel[y0][x - kw2].r;				//�߻�� 
						opixel[y][x].g = pixel[y0][x - kw2].g;				//�߻�� 
						opixel[y][x].b = pixel[y0][x - kw2].b;				//�߻�� 
					}
					for (; x < newWidth; x++) {
						opixel[y][x].r = pixel[y0][x - kw2 - width].r;		//����
						opixel[y][x].g = pixel[y0][x - kw2 - width].g;		//����
						opixel[y][x].b = pixel[y0][x - kw2 - width].b;		//����
					}
				}

				//kh2+height ~ BOTTOM
				for (; y < newHeight; y++) {
					int y0 = y - kh2 - height;							//0���� �����ϴ� y -> y0

					for (x = 0; x < kw2; x++) {
						opixel[y][x].r = pixel[y0][width - kw2 + x].r;		//�»�� 
						opixel[y][x].g = pixel[y0][width - kw2 + x].g;		//�»�� 
						opixel[y][x].b = pixel[y0][width - kw2 + x].b;		//�»�� 
					}
					for (; x < kw2 + width; x++) {
						opixel[y][x].r = pixel[y0][x - kw2].r;				//�߻�� 
						opixel[y][x].g = pixel[y0][x - kw2].g;				//�߻�� 
						opixel[y][x].b = pixel[y0][x - kw2].b;				//�߻�� 
					}
					for (; x < newWidth; x++) {
						opixel[y][x].r = pixel[y0][x - kw2 - width].r;		//���� 
						opixel[y][x].g = pixel[y0][x - kw2 - width].g;		//���� 
						opixel[y][x].b = pixel[y0][x - kw2 - width].b;		//���� 
					}
				}

				break;

			default:
				return false;
			}
		}
		else {
			return false;
		}
	}

	else if (depth == CV_16U || depth == CV_16S) {
		if (channels == 1) {
			WORD **pixel = (WORD**)img.GetPixels();
			WORD **opixel = (WORD**)oimg.GetPixels();
			// *16��Ʈ�̱⿡ memset, memcpy ������ size�κп� 2�� �����־���. -> �� ���� ����� ������?
			//-> memset ���� �ȼ� ���� �������� ����
			switch (borderType)
			{
			case IOP_BORDER_CONSTANT:
				for (y = 0; y < newHeight; y++) {
					for (x = 0; x < newWidth; x++) {
						if (y >= kh2 && y < kh2 + height && x >= kw2 && x < kw2 + width)
							opixel[y][x] = pixel[y - kh2][x - kw2];
						else
							opixel[y][x] = value;
					}
				}
				break;

			case IOP_BORDER_REPLICATE:	//�����ڸ� ������ ���� 

				//TOP ~ kh2 
				for (y = 0; y < kh2; y++) {
					for (x = 0; x < kw2; x++)
						opixel[y][x] = pixel[0][0];					//�»�� 
					for (; x < kw2 + width; x++)
						opixel[y][x] = pixel[0][x - kw2];			//�߻�� 
					for (; x < newWidth; x++)
						opixel[y][x] = pixel[0][width - 1];			//���� 
				}

				//kh2 ~ kh2+height
				for (; y < height + kh2; y++) {
					int py = y - kh2;

					for (x = 0; x < kw2; x++)
						opixel[y][x] = pixel[py][0];					//���ߴ� 
					for (; x < kw2 + width; x++)
						opixel[y][x] = pixel[py][x - kw2];				//���ߴ� (�̹���)
					for (; x < newWidth; x++)
						opixel[y][x] = pixel[py][width - 1];			//���ߴ� 
				}

				//kh2+height ~ BOTTOM
				for (; y < newHeight; y++) {

					for (x = 0; x < kw2; x++)
						opixel[y][x] = pixel[height - 1][0];					//���ϴ� 
					for (; x < kw2 + width; x++)
						opixel[y][x] = pixel[height - 1][x - kw2];				//���ϴ� (�̹���)
					for (; x < newWidth; x++)
						opixel[y][x] = pixel[height - 1][width - 1];			//���ϴ� 
				}
				break;

			case IOP_BORDER_REFLECT:

				//TOP ~ kh2 
				for (y = 0; y < kh2; y++) {
					int py = kh2 - 1 - y;							//y�� ���� ����Ī ��ȭ�� 

					for (x = 0; x < newWidth; x++) {

						//�»�� 
						if (x < kw2) {
							int px = kw2 - 1 - x;					//x�� ���� ����Ī ��ȭ�� 
							opixel[y][x] = pixel[py][px];
						}

						//�߻�� 
						else if (x < kw2 + width) {
							int x0 = x - kw2;						//0���� �����ϴ� x
							opixel[y][x] = pixel[py][x0];
						}

						//����
						else {
							int x0 = x - kw2 - width;				//0���� �����ϴ� x
							int px = kw2 - 1 - x0;					//x�� ���� ����Ī ��ȭ�� 
							opixel[y][x] = pixel[py][width - kw2 + px];
						}
					}
				}

				//kh2 ~ kh2+height
				for (; y < height + kh2; y++) {
					int y0 = y - kh2;								//0���� �����ϴ� y
					int py = kh2 - 1 - y0;							//y�� ���� ����Ī ��ȭ�� 

					for (x = 0; x < newWidth; x++) {

						//���ߴ� 
						if (x < kw2) {
							int px = kw2 - 1 - x;					//x�� ���� ����Ī ��ȭ�� 
							opixel[y][x] = pixel[y0][px];
						}

						//���ߴ� 
						else if (x < kw2 + width) {
							int x0 = x - kw2;						//0���� �����ϴ� x
							opixel[y][x] = pixel[y0][x0];			//�̹���
						}

						//���ߴ�
						else {
							int x0 = x - kw2 - width;				//0���� �����ϴ� x
							int px = kw2 - 1 - x0;					//x�� ���� ����Ī ��ȭ�� 
							opixel[y][x] = pixel[y0][width - kw2 + px];
						}
					}
				}

				//kh2+height ~ BOTTOM
				for (; y < newHeight; y++) {
					int y0 = y - kh2 - height;						//0���� �����ϴ� y
					int py = kh2 - 1 - y0;							//y�� ���� ����Ī ��ȭ�� 

					for (x = 0; x < newWidth; x++) {

						//���ϴ� 
						if (x < kw2) {
							int px = kw2 - 1 - x;					//x�� ���� ����Ī ��ȭ�� 
							opixel[y][x] = pixel[height - kh2 + py][px];
						}

						//���ϴ� 
						else if (x < kw2 + width) {
							int x0 = x - kw2;						//0���� �����ϴ� x
							opixel[y][x] = pixel[height - kh2 + py][x0];
						}

						//���ϴ�
						else {
							int x0 = x - kw2 - width;				//0���� �����ϴ� x
							int px = kw2 - 1 - x0;					//x�� ���� ����Ī ��ȭ�� 
							opixel[y][x] = pixel[height - kh2 + py][width - kw2 + px];
						}

					}
				}
				break;

			case IOP_BORDER_WRAP:

				//TOP ~ kh2 
				for (y = 0; y < kh2; y++) {
					for (x = 0; x < kw2; x++)
						opixel[y][x] = pixel[height - kh2 + y][width - kw2 + x];		//�»�� 
					for (; x < kw2 + width; x++)
						opixel[y][x] = pixel[height - kh2 + y][x - kw2];				//�߻�� 
					for (; x < newWidth; x++)
						opixel[y][x] = pixel[height - kh2 + y][x - kw2 - width];		//���� 
				}

				//kh2 ~ kh2+height
				for (; y < height + kh2; y++) {
					int y0 = y - kh2;									//0���� �����ϴ� y -> y0

					for (x = 0; x < kw2; x++)
						opixel[y][x] = pixel[y0][width - kw2 + x];		//�»�� 
					for (; x < kw2 + width; x++)
						opixel[y][x] = pixel[y0][x - kw2];				//�߻�� 
					for (; x < newWidth; x++)
						opixel[y][x] = pixel[y0][x - kw2 - width];		//���� 
				}

				//kh2+height ~ BOTTOM
				for (; y < newHeight; y++) {
					int y0 = y - kh2 - height;							//0���� �����ϴ� y -> y0

					for (x = 0; x < kw2; x++)
						opixel[y][x] = pixel[y0][width - kw2 + x];		//�»�� 
					for (; x < kw2 + width; x++)
						opixel[y][x] = pixel[y0][x - kw2];				//�߻�� 
					for (; x < newWidth; x++)
						opixel[y][x] = pixel[y0][x - kw2 - width];		//���� 
				}

				break;

			default:
				return false;
			}
		}
		else if (channels == 3) {
			return false;
		}

		else {
			return false;
		}
	}

	else {
		return false;
	}

	return true;
}

bool IpGaussianKernel(double ** kernel, int h, int w, double sigma)
{
	if (!kernel)											return false;
	if (sigma <= 0.)										return false;
	if ((h < 1) || (w < 1) || (h % 2 == 0) || (w % 2 == 0))	return false;

	double scale = 0.;
	double PI = 3.141592;
	double E = 2.71828;

	//����þ� ����
	int cy = h / 2;
	int cx = w / 2;
	double c1 = sqrt(2 * PI) * sigma;
	double c2 = -2. * sigma * sigma;



	for (int y = 0; y < h; y++) {
		double dy = (cy - y)*(cy - y);
		for (int x = 0; x < w; x++) {
			double dx = (cx - x)*(cx - x);
			kernel[y][x] = exp((dy + dx) / c2) * (1 / c1);
			scale += kernel[y][x];
		}
	}

	//�� ��ġ�� 1�ǰ� ���� 
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			kernel[y][x] /= scale;
		}
	}

	return true;
}

bool IpLoGaussianKernel(double ** kernel, int h, int w, double sigma)
{
	if (!kernel)											return false;
	if (sigma <= 0.)										return false;
	if ((h < 1) || (w < 1) || (h % 2 == 0) || (w % 2 == 0))	return false;

	double scale = sigma*sigma;			//ù��° ������ ���(�ñ׸� ����)
	//double scale = 0;					
	double PI = 3.141592;

	//LoG Equation 
	int cy = h / 2;
	int cx = w / 2;
	int dy, dx;
	double c1, c2;



	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			dy = cy - y;
			dx = cx - x;
			c1 = (double)((dx*dx) + (dy*dy) - (2.f*sigma*sigma)) / (sigma*sigma*sigma*sigma);
			c2 = (double)(dx*dx + dy*dy) / (-2.f*sigma*sigma);
			kernel[y][x] = c1 * exp(c2);
			//scale += kernel[y][x];	//�ι�° ������ ���(kernel ���а�)
		}
	}
	//scale = kernel[cy][cx] * 10;		//����° ������ ���(�߽ɰ� * 10) -> kernel �߰��� ���ϴ°žƴ�
	//scale = cy * 10;


 	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			kernel[y][x] *= scale;
		}
	}

	return true;
}

bool IpAddGaussianNoise(MyIPImage & img, double sigma)
{
	if (!img.GetPixels())	return false;
	if (sigma <= 0.)		return false;

	double noise;

	int width = img.GetWidth();
	int height = img.GetHeight();
	int depth = img.GetDepth();
	int channels = img.GetChannels();

	if (depth == CV_8U || depth == CV_8S) {
		if (channels == 1) {
			byte** pixel = (byte**)img.GetPixels();

			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					noise = (double)pixel[y][x] + GaussianRand(0., sigma);	//����� 0�̰� �ñ׸��� sigma��
					pixel[y][x] = (byte)limit(noise);
				}
			}
		}
		else if (channels == 3) {
			RGBBYTE** pixel = (RGBBYTE**)img.GetPixels();

			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					noise = (double)pixel[y][x].r + GaussianRand(0., sigma);	//����� 0�̰� �ñ׸��� sigma��
					pixel[y][x].r = (byte)limit(noise);
					noise = (double)pixel[y][x].g + GaussianRand(0., sigma);	//����� 0�̰� �ñ׸��� sigma��
					pixel[y][x].g = (byte)limit(noise);
					noise = (double)pixel[y][x].b + GaussianRand(0., sigma);	//����� 0�̰� �ñ׸��� sigma��
					pixel[y][x].b = (byte)limit(noise);
				}
			}
		}
		else 
			return false;
	}
	else if (depth == CV_16U || depth == CV_16S) {
		if (channels == 1) {
			double noise16bit;
			WORD** pixel = (WORD**)img.GetPixels();

			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					noise16bit = GaussianRand(0., sigma) * 256;
					noise = (double)pixel[y][x] + noise16bit;	//����� 0�̰� �ñ׸��� sigma��
					pixel[y][x] = (WORD)limit16bit(noise);
				}
			}
		}
		else if (channels == 3) {
			return false;
		}
		else
			return false;
	}

	else
		return false;

	return true;
}

bool IpAddSaltPepperNoise(MyIPImage & img, double ps, double pp, BOOL rgb)
{
	if (!img.GetPixels())	return false;
	if (ps < 0.)		return false;
	if (pp < 0.)		return false;

	int ns = (int)((double)RAND_MAX*ps + 0.5);	//0~32767�߿� ppȮ����(��üȮ��1) ���� �� �ִ� ����
	int np = (int)((double)RAND_MAX*(ps + pp) + 0.5);	//ps

	int width = img.GetWidth();
	int height = img.GetHeight();
	int depth = img.GetDepth();
	int channels = img.GetChannels();

	if (depth == CV_8U || depth == CV_8S) {
		if (channels == 1) {
			byte** pixel = (byte**)img.GetPixels();

			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					int r = rand();
					if (r < ns)pixel[y][x] = (byte)255;
					else if (r < np)pixel[y][x] = (byte)0;
				}
			}
		}
		else if (channels == 3) {
			RGBBYTE** pixel = (RGBBYTE**)img.GetPixels();

			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					int r;
					if (rgb == false) {
						r = rand();
						if (r < ns) {
							pixel[y][x].r = (byte)255;
							pixel[y][x].g = (byte)255;
							pixel[y][x].b = (byte)255;
						}
						else if (r < np) {
							pixel[y][x].r = (byte)0;
							pixel[y][x].g = (byte)0;
							pixel[y][x].b = (byte)0;
						}
					}
					else if (rgb == TRUE) {		//���尡��...��rgb���� ���� 
						r = rand();
						if (r < ns)
							pixel[y][x].r = (byte)255;
						else if (r < np)
							pixel[y][x].r = (byte)0;
						r = rand();
						if (r < ns)
							pixel[y][x].g = (byte)255;
						else if (r < np)
							pixel[y][x].g = (byte)0;
						r = rand();
						if (r < ns)
							pixel[y][x].b = (byte)255;
						else if (r < np)
							pixel[y][x].b = (byte)0;
					}
				}
			}
		}
		else
			return false;
	}
	else if (depth == CV_16U || depth == CV_16S) {
		if (channels == 1) {
			WORD** pixel = (WORD**)img.GetPixels();

			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					int r = rand();
					if (r < ns)pixel[y][x] = (WORD)65535;
					else if (r < np)pixel[y][x] = (WORD)0;
				}
			}
		}
		else if (channels == 3) {
		}
		else
			return false;
	}

	else
		return false;

	return true;
}

double GaussianRand(double mean, double sigma)
{
	double x1, x2, radius, factor, y1;
	static double y2;
	static int use_last = 0;

	if (use_last) {
		y1 = y2;
		use_last = 0;
	}
	else {
		do {
			x1 = 2. * rand() / RAND_MAX - 1.;		//-1 ~ 1 ������ �������� �ްڴ�. 
			x2 = 2. * rand() / RAND_MAX - 1.;
			radius = x1*x1 + x2*x2;					
		} while (radius < 0.00000001 || radius >= 1.);

		factor = sqrt((-2.*log(radius)) / radius);

		y1 = x1 * factor;
		y2 = x2*factor;
		use_last = 1;
	}
	return (mean + y1*sigma);
}

//sharpening 
bool IpFilterHighboost(MyIPImage & img, double sigma, double k)
{
	if (!img.GetPixels())	return false;
	if (sigma <= 0)			return false;
	if (k < 1 || k>10)		return false;



	return true;
}
