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
//>> 앞으로 이런 tempimg영상을 많이 만들거 같으니까 함수로 만들어주자.(IpFillBorder)
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
							int pixel = tpixel[y + ky][x + kx];	//이 픽셀값은 매번 바뀜 (0~255) 
							sum += pixel*kernel[ky][kx];		//kernel[ky][kx]는 배열로 저장됨 ... -> for문을 돌면서 LUT 을 만들면 어떨까?
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
							int Rpixel = tpixel[y + ky][x + kx].r;	//이 픽셀값은 매번 바뀜 (0~255) 
							int Gpixel = tpixel[y + ky][x + kx].g;	//이 픽셀값은 매번 바뀜 (0~255) 
							int Bpixel = tpixel[y + ky][x + kx].b;	//이 픽셀값은 매번 바뀜 (0~255) 
							Rsum += Rpixel*kernel[ky][kx];		//kernel[ky][kx]는 배열로 저장됨 ... -> for문을 돌면서 LUT 을 만들면 어떨까?
							Gsum += Gpixel*kernel[ky][kx];		//kernel[ky][kx]는 배열로 저장됨 ... -> for문을 돌면서 LUT 을 만들면 어떨까?
							Bsum += Bpixel*kernel[ky][kx];		//kernel[ky][kx]는 배열로 저장됨 ... -> for문을 돌면서 LUT 을 만들면 어떨까?
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
							int pixel = tpixel[y + ky][x + kx];	//이 픽셀값은 매번 바뀜 (0~255) 
							sum += pixel*kernel[ky][kx];		//kernel[ky][kx]는 배열로 저장됨 ... -> for문을 돌면서 LUT 을 만들면 어떨까?
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
	int kh2 = h / 2;					//kernel height / 2 (위, 아래로 각각 늘어나는 픽셀 수)
	int kw2 = w / 2;					//kernel  width / 2 (왼쪽오른쪽 각각 늘어나는 픽셀 수)
	int newWidth = width + kw2 * 2;		//새로 생성되는 이미지의 Width
	int newHeight = height + kh2 * 2;	//새로 생성되는 이미지의 Height


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
							int pixel = tpixel[y + ky][x + kx];	//이 픽셀값은 매번 바뀜 (0~255) 
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
							pixel.r = tpixel[y + ky][x + kx].r;	//이 픽셀값은 매번 바뀜 (0~255) 
							pixel.g = tpixel[y + ky][x + kx].g;	//이 픽셀값은 매번 바뀜 (0~255) 
							pixel.b = tpixel[y + ky][x + kx].b;	//이 픽셀값은 매번 바뀜 (0~255) 
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
							int pixel = tpixel[y + ky][x + kx];	//이 픽셀값은 매번 바뀜 (0~255) 
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
	int kh2 = h / 2;					//kernel height / 2 (위, 아래로 각각 늘어나는 픽셀 수)
	int kw2 = w / 2;					//kernel  width / 2 (왼쪽오른쪽 각각 늘어나는 픽셀 수)
	int newWidth = width + kw2 * 2;		//새로 생성되는 이미지의 Width
	int newHeight = height + kh2 * 2;	//새로 생성되는 이미지의 Height


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
							int pixel = tpixel[y + ky][x + kx];	//이 픽셀값은 매번 바뀜 (0~255) 
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
							pixel.r = tpixel[y + ky][x + kx].r;	//이 픽셀값은 매번 바뀜 (0~255) 
							pixel.g = tpixel[y + ky][x + kx].g;	//이 픽셀값은 매번 바뀜 (0~255) 
							pixel.b = tpixel[y + ky][x + kx].b;	//이 픽셀값은 매번 바뀜 (0~255) 
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
							int pixel = tpixel[y + ky][x + kx];	//이 픽셀값은 매번 바뀜 (0~255) 
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
	if (!IpFillBorder(img, timg, h, w, IOP_BORDER_REFLECT))	//Median Filter의 Border Type은 Reflect 이다. 
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

	int x, y, mid = (w*h) / 2;	//for문 x,y , 중심위치
	Point start;
	Mat mask, sort_m, one_row;

	//border밖에는 프로그램 하지 않았다. 
	//일단b border계산한 큰 이미지만 만들어둠!
	Size Newsize(img.size() + Size(h-1, w-1));
	Mat timg = Mat(Newsize, img.type(), Scalar(0));
	Rect roi = Rect(h_m, img.size());
	img.copyTo(timg(roi));


	if (img.depth() == CV_8U) {
		if (img.channels() == 1) {
			for (y = 0; y < timg.rows-msize.height+1; y++) {
				for (x = 0; x < timg.cols-msize.width+1; x++) {
					start = Point(x, y);	//kernel의 좌상단부터 시작할것이당
					Rect roi(start, msize);
					timg(roi).copyTo(mask);		//w*h만큼의 마스크가 복사된거임!

					one_row = mask.reshape(1, 1);				//1행 행렬로 만들자 
					cv::sort(one_row, sort_m, SORT_EVERY_ROW);	//행 단위 정렬 수행
					img.at<uchar>(y, x) = sort_m.at<uchar>(mid);	//!!!!!!원본 영상이 계속 바뀐다. border이미지 생성할 때 큰 행렬을 만들고 연산 뒤에 img에 저장하자...
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
						std::sort(arr, arr + sizePOW);			//오름차순으로 배열 정렬
						mVal = arr[sizePOW / 2];					//2. Median 값 구하기 
						min = arr[0];
						max = arr[sizePOW - 1];
						if (mVal == min || mVal == max) {			//3. mVal이 최대,최솟값과 같으면 
							if (size < maxsize)						//4. 사이즈가 max사이즈가 될때까지 
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
						std::sort(Rarr, Rarr + sizePOW);			//오름차순으로 배열 정렬
						std::sort(Garr, Garr + sizePOW);			//오름차순으로 배열 정렬
						std::sort(Barr, Barr + sizePOW);			//오름차순으로 배열 정렬
						RmVal = Rarr[sizePOW / 2];					//2. Median 값 구하기 
						GmVal = Garr[sizePOW / 2];					//2. Median 값 구하기 
						BmVal = Barr[sizePOW / 2];					//2. Median 값 구하기 
						Rmax = Rarr[sizePOW - 1];
						Gmax = Garr[sizePOW - 1];
						Bmax = Barr[sizePOW - 1];
						Rmin = Rarr[0];
						Gmin = Garr[0];
						Bmin = Barr[0];
						
						if (Rfin == false) {
							if (RmVal == Rmin || RmVal == Rmax) {		//3. mVal이 최대,최솟값과 같으면 
								if (size < maxsize)						//4. 사이즈가 max사이즈가 될때까지 
									Rcontinue = true;
							}
						}
						if (Gfin == false) {
							if (GmVal == Gmin || GmVal == Gmax) {		//3. mVal이 최대,최솟값과 같으면 
								if (size < maxsize)						//4. 사이즈가 max사이즈가 될때까지 
									Gcontinue = true;
							}
						}
						if (Bfin == false) {
							if (BmVal == Bmin || BmVal == Bmax) {		//3. mVal이 최대,최솟값과 같으면 
								if (size < maxsize)						//4. 사이즈가 max사이즈가 될때까지 
									Bcontinue = true;
							}
						}

						//kernel size가 max size까지 커지면 아래 조건문에 의해 픽셀값 결정 
						if (Rfin == false && Rcontinue == false) {					//R값이 지정되지 않았고, R값이 kernel size증가하여 계산할 필요없으면 
							if (pixel[y][x].r == Rmin || pixel[y][x].r == Rmax)		//픽셀값이 kernel내의 최대 or 최솟값이랑 같으면
								pixel[y][x].r = (byte)limit(RmVal);					//median값을 픽셀값에 저장 
																					//else 생략) 그 외는 픽셀값 유지 
							Rfin = true;											//R값 지정됬다고 Flag -> true 
						}
						if (Gfin == false && Gcontinue == false) {					//R값이 지정되지 않았고, R값이 kernel size증가하여 계산할 필요없으면 
							if (pixel[y][x].g == Gmin || pixel[y][x].g == Gmax)		//픽셀값이 kernel내의 최대 or 최솟값이랑 같으면
								pixel[y][x].g = (byte)limit(GmVal);					//median값을 픽셀값에 저장 
																					//else 생략) 그 외는 픽셀값 유지 
							Gfin = true;											//R값 지정됬다고 Flag -> true 
						}
						if (Bfin == false && Bcontinue == false) {					//R값이 지정되지 않았고, R값이 kernel size증가하여 계산할 필요없으면 
							if (pixel[y][x].b == Bmin || pixel[y][x].b == Bmax)		//픽셀값이 kernel내의 최대 or 최솟값이랑 같으면
								pixel[y][x].b = (byte)limit(BmVal);					//median값을 픽셀값에 저장 
																					//else 생략) 그 외는 픽셀값 유지 
							Bfin = true;											//R값 지정됬다고 Flag -> true 
						}

						if (Rfin && Gfin && Bfin)									//모든 값이 지정이 됬다면
							break;													//break
						else														//하나라도 지정이 되지 않으면 
							continue;												//kernel size증가하여 계속 
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
						std::sort(arr, arr + sizePOW);			//오름차순으로 배열 정렬
						mVal = arr[sizePOW / 2];					//2. Median 값 구하기 
						min = arr[0];
						max = arr[sizePOW - 1];
						if (mVal == min || mVal == max) {			//3. mVal이 최대,최솟값과 같으면 
							if (size < maxsize)						//4. 사이즈가 max사이즈가 될때까지 
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

	int x, y, size;	//for문 x,y , 중심위치
	Point start;
	Mat mask, sort_m, one_row;
	int mVal, min, max;

	//border밖에는 프로그램 하지 않았다. 
	//일단b border계산한 큰 이미지만 만들어둠!
	Mat timg;
	copyMakeBorder(img, timg, maxsize / 2, maxsize / 2, maxsize / 2, maxsize / 2, BORDER_REFLECT);

	if (depth == CV_8U) {
		if (channels == 1) {
			for (y = 0; y < height; y++) {
				for (x = 0; x < width; x++) {
					for (size = 3; size <= maxsize; size += 2) {
						int pixel = img.at<uchar>(y, x);
						int offset = maxsize / 2 - size / 2;
						start = Point(offset + x, offset + y);		//kernel의 좌상단부터 시작할것이당
						Rect roi(start, Size(size,size));
						timg(roi).copyTo(mask);						//w*h만큼의 마스크가 복사된거임!

						one_row = mask.reshape(1, 1);				//1행 행렬로 만들자 
						cv::sort(one_row, sort_m, SORT_EVERY_ROW);	//행 단위 정렬 수행
						mVal = sort_m.at<uchar>((size*size) / 2);	//!!!!!!원본 영상이 계속 바뀐다. border이미지 생성할 때 큰 행렬을 만들고 연산 뒤에 img에 저장하자...
						min = sort_m.at<uchar>(0);
						max = sort_m.at<uchar>((size*size) - 1);

						if (mVal == min || mVal == max) {			//3. mVal이 최대,최솟값과 같으면 
							if (size < maxsize)						//4. 사이즈가 max사이즈가 될때까지 
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
						start = Point(offset + x, offset + y);		//kernel의 좌상단부터 시작할것이당
						Rect roi(start, Size(size, size));
						timg(roi).copyTo(mask);						//w*h만큼의 마스크가 복사된거임!
						Mat bgr[3], sortbgr[3];
						split(mask, bgr);

						sortbgr[0] = bgr[0].reshape(1, 1);				//1행 행렬로 만들자 
						sortbgr[1] = bgr[1].reshape(1, 1);				//1행 행렬로 만들자 
						sortbgr[2] = bgr[2].reshape(1, 1);				//1행 행렬로 만들자 

						cv::sort(sortbgr[0], sortbgr[0], SORT_EVERY_ROW);	//행 단위 정렬 수행
						cv::sort(sortbgr[1], sortbgr[1], SORT_EVERY_ROW);	//행 단위 정렬 수행
						cv::sort(sortbgr[2], sortbgr[2], SORT_EVERY_ROW);	//행 단위 정렬 수행

						RmVal = sortbgr[2].at<uchar>((size*size) / 2);	//!!!!!!원본 영상이 계속 바뀐다. border이미지 생성할 때 큰 행렬을 만들고 연산 뒤에 img에 저장하자...
						GmVal = sortbgr[1].at<uchar>((size*size) / 2);
						BmVal = sortbgr[0].at<uchar>((size*size) / 2);
						Rmax = sortbgr[2].at<uchar>((size*size) - 1);
						Gmax = sortbgr[1].at<uchar>((size*size) - 1);
						Bmax = sortbgr[0].at<uchar>((size*size) - 1);
						Rmin = sortbgr[2].at<uchar>(0);
						Gmin = sortbgr[1].at<uchar>(0);
						Bmin = sortbgr[0].at<uchar>(0);

						if (Rfin == false) {
							if (RmVal == Rmin || RmVal == Rmax) {		//3. mVal이 최대,최솟값과 같으면 
								if (size < maxsize)						//4. 사이즈가 max사이즈가 될때까지 
									Rcontinue = true;
							}
						}
						if (Gfin == false) {
							if (GmVal == Gmin || GmVal == Gmax) {		//3. mVal이 최대,최솟값과 같으면 
								if (size < maxsize)						//4. 사이즈가 max사이즈가 될때까지 
									Gcontinue = true;
							}
						}
						if (Bfin == false) {
							if (BmVal == Bmin || BmVal == Bmax) {		//3. mVal이 최대,최솟값과 같으면 
								if (size < maxsize)						//4. 사이즈가 max사이즈가 될때까지 
									Bcontinue = true;
							}
						}

						//kernel size가 max size까지 커지면 아래 조건문에 의해 픽셀값 결정 
						if (Rfin == false && Rcontinue == false) {										//R값이 지정되지 않았고, R값이 kernel size증가하여 계산할 필요없으면 
							if (pixel.r == Rmin || pixel.r == Rmax)										//픽셀값이 kernel내의 최대 or 최솟값이랑 같으면
								img.at<Vec3b>(y, x)[2] = (byte)limit(RmVal);							//median값을 픽셀값에 저장 
																										//else 생략) 그 외는 픽셀값 유지 
							Rfin = true;																//R값 지정됬다고 Flag -> true 
						}
						if (Gfin == false && Gcontinue == false) {										//R값이 지정되지 않았고, R값이 kernel size증가하여 계산할 필요없으면 
							if (pixel.g == Gmin || pixel.g == Gmax)										//픽셀값이 kernel내의 최대 or 최솟값이랑 같으면
								img.at<Vec3b>(y, x)[1] = (byte)limit(GmVal);							//median값을 픽셀값에 저장 
																										//else 생략) 그 외는 픽셀값 유지 
							Gfin = true;																//R값 지정됬다고 Flag -> true 
						}
						if (Bfin == false && Bcontinue == false) {										//R값이 지정되지 않았고, R값이 kernel size증가하여 계산할 필요없으면 
							if (pixel.b == Bmin || pixel.b == Bmax)		//픽셀값이 kernel내의 최대 or 최솟값이랑 같으면
								img.at<Vec3b>(y, x)[0] = (byte)limit(BmVal);							//median값을 픽셀값에 저장 
																									//else 생략) 그 외는 픽셀값 유지 
							Bfin = true;																//R값 지정됬다고 Flag -> true 
						}

						if (Rfin && Gfin && Bfin)									//모든 값이 지정이 됬다면
							break;													//break
						else														//하나라도 지정이 되지 않으면 
							continue;												//kernel size증가하여 계속 
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
						start = Point(offset + x, offset + y);		//kernel의 좌상단부터 시작할것이당
						Rect roi(start, Size(size, size));
						timg(roi).copyTo(mask);						//w*h만큼의 마스크가 복사된거임!

						one_row = mask.reshape(1, 1);				//1행 행렬로 만들자 
						cv::sort(one_row, sort_m, SORT_EVERY_ROW);	//행 단위 정렬 수행
						mVal = sort_m.at<unsigned short>((size*size) / 2);	//!!!!!!원본 영상이 계속 바뀐다. border이미지 생성할 때 큰 행렬을 만들고 연산 뒤에 img에 저장하자...
						min = sort_m.at<unsigned short>(0);
						max = sort_m.at<unsigned short>((size*size) - 1);

						if (mVal == min || mVal == max) {			//3. mVal이 최대,최솟값과 같으면 
							if (size < maxsize)						//4. 사이즈가 max사이즈가 될때까지 
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
	int kh2 = h / 2;					//kernel height / 2 (위, 아래로 각각 늘어나는 픽셀 수)
	int kw2 = w / 2;					//kernel  width / 2 (왼쪽오른쪽 각각 늘어나는 픽셀 수)
	int newWidth = width + kw2 * 2;		//새로 생성되는 이미지의 Width
	int newHeight = height + kh2 * 2;	//새로 생성되는 이미지의 Height

	//kernel size의 반마늠씩 위, 아래, 왼쪽, 오른쪽으로 큰 임시 영상을 만들고.
	//borderType에 따라서 값을 체워 놓는다.

	if (!oimg.CreateImage(newHeight, newWidth, depth, channels)) return false;

	if (depth == CV_8U || depth == CV_8S) {
		if (channels == 1) {
			byte **pixel = (byte**)img.GetPixels();
			byte **opixel = (byte**)oimg.GetPixels();
			// *16비트이기에 memset, memcpy 연산의 size부분에 2를 곱해주었다. -> 더 좋은 방법은 없을까?
			//-> memset 말고 픽셀 직접 대입으로 변경
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

			case IOP_BORDER_REPLICATE:	//가장자리 값으로 복사 

										//TOP ~ kh2 
				for (y = 0; y < kh2; y++) {
					for (x = 0; x < kw2; x++)
						opixel[y][x] = pixel[0][0];					//좌상단 
					for (; x < kw2 + width; x++)
						opixel[y][x] = pixel[0][x - kw2];			//중상단 
					for (; x < newWidth; x++)
						opixel[y][x] = pixel[0][width - 1];			//우상단 
				}

				//kh2 ~ kh2+height
				for (; y < height + kh2; y++) {
					int py = y - kh2;

					for (x = 0; x < kw2; x++)
						opixel[y][x] = pixel[py][0];					//좌중단 
					for (; x < kw2 + width; x++)
						opixel[y][x] = pixel[py][x - kw2];				//중중단 (이미지)
					for (; x < newWidth; x++)
						opixel[y][x] = pixel[py][width - 1];			//우중단 
				}

				//kh2+height ~ BOTTOM
				for (; y < newHeight; y++) {

					for (x = 0; x < kw2; x++)
						opixel[y][x] = pixel[height - 1][0];					//좌하단 
					for (; x < kw2 + width; x++)
						opixel[y][x] = pixel[height - 1][x - kw2];				//중하단 (이미지)
					for (; x < newWidth; x++)
						opixel[y][x] = pixel[height - 1][width - 1];			//우하단 
				}
				break;

			case IOP_BORDER_REFLECT:

				//TOP ~ kh2 
				for (y = 0; y < kh2; y++) {
					int py = kh2 - 1 - y;							//y에 대한 점대칭 변화량 

					for (x = 0; x < newWidth; x++) {

						//좌상단 
						if (x < kw2) {
							int px = kw2 - 1 - x;					//x에 대한 점대칭 변화량 
							opixel[y][x] = pixel[py][px];
						}

						//중상단 
						else if (x < kw2 + width) {
							int x0 = x - kw2;						//0부터 시작하는 x
							opixel[y][x] = pixel[py][x0];
						}

						//우상단
						else {
							int x0 = x - kw2 - width;				//0부터 시작하는 x
							int px = kw2 - 1 - x0;					//x에 대한 점대칭 변화량 
							opixel[y][x] = pixel[py][width - kw2 + px];
						}
					}
				}

				//kh2 ~ kh2+height
				for (; y < height + kh2; y++) {
					int y0 = y - kh2;								//0부터 시작하는 y
					int py = kh2 - 1 - y0;							//y에 대한 점대칭 변화량 

					for (x = 0; x < newWidth; x++) {

						//좌중단 
						if (x < kw2) {
							int px = kw2 - 1 - x;					//x에 대한 점대칭 변화량 
							opixel[y][x] = pixel[y0][px];
						}

						//중중단 
						else if (x < kw2 + width) {
							int x0 = x - kw2;						//0부터 시작하는 x
							opixel[y][x] = pixel[y0][x0];			//이미지
						}

						//우중단
						else {
							int x0 = x - kw2 - width;				//0부터 시작하는 x
							int px = kw2 - 1 - x0;					//x에 대한 점대칭 변화량 
							opixel[y][x] = pixel[y0][width - kw2 + px];
						}
					}
				}

				//kh2+height ~ BOTTOM
				for (; y < newHeight; y++) {
					int y0 = y - kh2 - height;						//0부터 시작하는 y
					int py = kh2 - 1 - y0;							//y에 대한 점대칭 변화량 

					for (x = 0; x < newWidth; x++) {

						//좌하단 
						if (x < kw2) {
							int px = kw2 - 1 - x;					//x에 대한 점대칭 변화량 
							opixel[y][x] = pixel[height - kh2 + py][px];
						}

						//중하단 
						else if (x < kw2 + width) {
							int x0 = x - kw2;						//0부터 시작하는 x
							opixel[y][x] = pixel[height - kh2 + py][x0];
						}

						//우하단
						else {
							int x0 = x - kw2 - width;				//0부터 시작하는 x
							int px = kw2 - 1 - x0;					//x에 대한 점대칭 변화량 
							opixel[y][x] = pixel[height - kh2 + py][width - kw2 + px];
						}

					}
				}
				break;

			case IOP_BORDER_WRAP:

				//TOP ~ kh2 
				for (y = 0; y < kh2; y++) {
					for (x = 0; x < kw2; x++)
						opixel[y][x] = pixel[height - kh2 + y][width - kw2 + x];		//좌상단 
					for (; x < kw2 + width; x++)
						opixel[y][x] = pixel[height - kh2 + y][x - kw2];				//중상단 
					for (; x < newWidth; x++)
						opixel[y][x] = pixel[height - kh2 + y][x - kw2 - width];		//우상단 
				}

				//kh2 ~ kh2+height
				for (; y < height + kh2; y++) {
					int y0 = y - kh2;									//0에서 시작하는 y -> y0

					for (x = 0; x < kw2; x++)
						opixel[y][x] = pixel[y0][width - kw2 + x];		//좌상단 
					for (; x < kw2 + width; x++)
						opixel[y][x] = pixel[y0][x - kw2];				//중상단 
					for (; x < newWidth; x++)
						opixel[y][x] = pixel[y0][x - kw2 - width];		//우상단 
				}

				//kh2+height ~ BOTTOM
				for (; y < newHeight; y++) {
					int y0 = y - kh2 - height;							//0에서 시작하는 y -> y0

					for (x = 0; x < kw2; x++)
						opixel[y][x] = pixel[y0][width - kw2 + x];		//좌상단 
					for (; x < kw2 + width; x++)
						opixel[y][x] = pixel[y0][x - kw2];				//중상단 
					for (; x < newWidth; x++)
						opixel[y][x] = pixel[y0][x - kw2 - width];		//우상단 
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
// 			case IOP_BORDER_REPLICATE:	//가장자리 값으로 복사 
// 				
// 				//TOP ~ kh2 
// 				for (y = 0; y < kh2; y++) {		
// 					memset(opixel[y], pixel[0][0], kw2);								//좌상단 
// 					memcpy(opixel[y] + kw2, pixel[0], width);							//중상단
// 					memset(opixel[y] + kw2 + width, pixel[0][width - 1], kw2);			//우상단
// 				}
// 
// 				//kh2 ~ kh2+height
// 				for (; y < height + kh2; y++) {
// 					int py = y - kh2;
// 					memset(opixel[y], pixel[py][0], kw2);								//좌중단
// 					memcpy(opixel[y] + kw2, pixel[py], width);							//중앙(이미지)
// 					memset(opixel[y] + kw2 + width, pixel[py][width - 1], kw2);			//우중단 
// 				}
// 
// 				//kh2+height ~ BOTTOM
// 				for (; y < newHeight; y++) {
// 					memset(opixel[y], pixel[height - 1][0], kw2);						//좌하단 
// 					memcpy(opixel[y] + kw2, pixel[height - 1], width);					//중하단
// 					memset(opixel[y] + kw2 + width, pixel[height - 1][width - 1], kw2);	//우하단
// 				}
// 
// 				/*//TOP ~ kh2 
// 				for (y = 0; y < kh2; y++) {		
// 					memset(opixel[y], pixel[0][0], kw2);								//좌상단 
// 					memcpy(opixel[y] + kw2, pixel[0], width);							//중상단
// 					memset(opixel[y] + kw2 + width, pixel[0][width - 1], kw2);			//우상단
// 				}
// 
// 				//kh2 ~ kh2+height
// 				for (; y < height + kh2; y++) {
// 					int py = y - kh2;
// 					memset(opixel[y], pixel[py][0], kw2);								//좌중단
// 					memcpy(opixel[y] + kw2, pixel[py], width);							//중앙(이미지)
// 					memset(opixel[y] + kw2 + width, pixel[py][width - 1], kw2);			//우중단 
// 				}
// 
// 				//kh2+height ~ BOTTOM
// 				for (; y < newHeight; y++) {
// 					memset(opixel[y], pixel[height - 1][0], kw2);						//좌하단 
// 					memcpy(opixel[y] + kw2, pixel[height - 1], width);					//중하단
// 					memset(opixel[y] + kw2 + width, pixel[height - 1][width - 1], kw2);	//우하단
// 				}*/
// 				
// 				/*
// 				//윗줄
// 				for (y = 0; y < kh2; y++) {	
// 					for (x = 0; x < kw2; x++)
// 						opixel[y][x] = pixel[0][0];
// 					for (; x < width + kw2; x++)
// 						opixel[y][x] = pixel[0][x - kw2];
// 					for (; x < newWidth; x++)
// 						opixel[y][x] = pixel[0][width - 1];
// 				}
// 				//중간
// 				for (; y < height + kh2; y++) {
// 					for (x = 0; x < kw2; x++)
// 						opixel[y][x] = pixel[y-kh2][0];
// 					for (; x < width + kw2; x++)
// 						opixel[y][x] = pixel[y-kh2][x - kw2];
// 					for (; x < newWidth; x++)
// 						opixel[y][x] = pixel[y-kh2][width - 1];
// 				}
// 				//밑줄
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
// 					int py = kw2 - 1 - y;									//대칭증가하는 y
// 					memcpy(opixel[y] + kw2, pixel[kh2 - 1 - y], width);											//중상단
// 					for (x = 0; x < kw2; x++) {
// 						int px = kh2 - 1 - x;								//대칭증가하는 x
// 						memset(opixel[y] + x, pixel[py][px], 1);												//좌상단 
// 						memset(opixel[y] + x + kw2 + width, pixel[py][width - kw2 + px], 1);					//우상단
// 					}
// 				}
// 
// 				//kh2 ~ kh2+height
// 				for (; y < height + kh2; y++) {
// 					int y0 = y - kh2;										//0에서 시작하는 y -> y0
// 					memcpy(opixel[y] + kw2, pixel[y0], width);													//중앙(이미지)
// 					for (x = 0; x < kw2; x++) {
// 						memset(opixel[y] + x, pixel[y0][kw2 - 1 - x], 1);										//좌중단
// 						memset(opixel[y] + x + kw2 + width, pixel[y0][width - 1 - x], 1);						//우중단 
// 					}
// 				}
// 
// 				//kh2+height ~ BOTTOM
// 				for (; y < newHeight; y++) {
// 					int y0 = y - kh2 - height;								//0에서 시작하는 y -> y0
// 					int py = kw2 - 1 - y0;									//대칭 증가하는 x
// 					memcpy(opixel[y] + kw2, pixel[height - 1 - y0], width);										//중하단
// 					for (x = 0; x < kw2; x++) {
// 						int px = kh2 - 1 - x;								//대칭 증가하는 y
// 						memset(opixel[y] + x, pixel[height - kh2 + py][px], 1);									//좌하단 
// 						memset(opixel[y] + x + kw2 + width, pixel[height - kh2 + py][width - kw2 + px], 1);		//우하단
// 					}
// 				}
// 				break;
// 
// 			case IOP_BORDER_WRAP:
// 
// 				//TOP ~ kh2 
// 				for (y = 0; y < kh2; y++) {
// 					memcpy(opixel[y], pixel[height - kh2 + y] + width - kw2, kw2);		//좌상단
// 					memcpy(opixel[y] + kw2, pixel[height - kh2 + y], width);			//중상단
// 					memcpy(opixel[y] + kw2 + width, pixel[height - kh2 + y], kw2);		//우상단
// 				}
// 
// 				//kh2 ~ kh2+height
// 				for (; y < height + kh2; y++) {
// 					int y0 = y - kh2;													//0에서 시작하는 y -> y0
// 					memcpy(opixel[y], pixel[y0] + width - kw2, kw2);					//좌중단
// 					memcpy(opixel[y] + kw2, pixel[y0], width);							//중앙(이미지)
// 					memcpy(opixel[y] + kw2 + width, pixel[y0], kw2);					//우중단
// 				}
// 
// 				//kh2+height ~ BOTTOM
// 				for (; y < newHeight; y++) {
// 					int y0 = y - kh2 - height;
// 					memcpy(opixel[y], pixel[y0] + width - kw2, kw2);					//좌하단
// 					memcpy(opixel[y] + kw2, pixel[y0], width);							//중하단
// 					memcpy(opixel[y] + kw2 + width, pixel[y0], kw2);					//우하단
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
			// *16비트이기에 memset, memcpy 연산의 size부분에 2를 곱해주었다. -> 더 좋은 방법은 없을까?
			//-> memset 말고 픽셀 직접 대입으로 변경
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

			case IOP_BORDER_REPLICATE:	//가장자리 값으로 복사 

				//TOP ~ kh2 
				for (y = 0; y < kh2; y++) {
					for (x = 0; x < kw2; x++) {
						opixel[y][x].r = pixel[0][0].r;					//좌상단 
						opixel[y][x].g = pixel[0][0].g;					//좌상단 
						opixel[y][x].b = pixel[0][0].b;					//좌상단 
					}
					for (; x < kw2 + width; x++) {
						opixel[y][x].r = pixel[0][x - kw2].r;			//중상단 
						opixel[y][x].g = pixel[0][x - kw2].g;			//중상단 
						opixel[y][x].b = pixel[0][x - kw2].b;			//중상단 
					}
					for (; x < newWidth; x++) {
						opixel[y][x].r = pixel[0][width - 1].r;			//우상단 
						opixel[y][x].g = pixel[0][width - 1].g;			//우상단 
						opixel[y][x].b = pixel[0][width - 1].b;			//우상단 
					}
				}

				//kh2 ~ kh2+height
				for (; y < height + kh2; y++) {
					int py = y - kh2;

					for (x = 0; x < kw2; x++) {
						opixel[y][x].r = pixel[py][0].r;					//좌중단 
						opixel[y][x].g = pixel[py][0].g;					//좌중단 
						opixel[y][x].b = pixel[py][0].b;					//좌중단 
					}
					for (; x < kw2 + width; x++) {
						opixel[y][x].r = pixel[py][x - kw2].r;				//중중단 (이미지)
						opixel[y][x].g = pixel[py][x - kw2].g;				//중중단 (이미지)
						opixel[y][x].b = pixel[py][x - kw2].b;				//중중단 (이미지)
					}
					for (; x < newWidth; x++) {
						opixel[y][x].r = pixel[py][width - 1].r;			//우중단 
						opixel[y][x].g = pixel[py][width - 1].g;			//우중단 
						opixel[y][x].b = pixel[py][width - 1].b;			//우중단 
					}
				}

				//kh2+height ~ BOTTOM
				for (; y < newHeight; y++) {

					for (x = 0; x < kw2; x++) {
						opixel[y][x].r = pixel[height - 1][0].r;					//좌하단 
						opixel[y][x].g = pixel[height - 1][0].g;					//좌하단 
						opixel[y][x].b = pixel[height - 1][0].b;					//좌하단 
					}
					for (; x < kw2 + width; x++) {
						opixel[y][x].r = pixel[height - 1][x - kw2].r;				//중하단
						opixel[y][x].g = pixel[height - 1][x - kw2].g;				//중하단
						opixel[y][x].b = pixel[height - 1][x - kw2].b;				//중하단
					}
					for (; x < newWidth; x++) {
						opixel[y][x].r = pixel[height - 1][width - 1].r;			//우하단 
						opixel[y][x].g = pixel[height - 1][width - 1].g;			//우하단 
						opixel[y][x].b = pixel[height - 1][width - 1].b;			//우하단 
					}
				}
				break;

			case IOP_BORDER_REFLECT:

				//TOP ~ kh2 
				for (y = 0; y < kh2; y++) {
					int py = kh2 - 1 - y;							//y에 대한 점대칭 변화량 

					for (x = 0; x < newWidth; x++) {

						//좌상단 
						if (x < kw2) {
							int px = kw2 - 1 - x;					//x에 대한 점대칭 변화량 
							opixel[y][x].r = pixel[py][px].r;
							opixel[y][x].g = pixel[py][px].g;
							opixel[y][x].b = pixel[py][px].b;
						}

						//중상단 
						else if (x < kw2 + width) {
							int x0 = x - kw2;						//0부터 시작하는 x
							opixel[y][x].r = pixel[py][x0].r;
							opixel[y][x].g = pixel[py][x0].g;
							opixel[y][x].b = pixel[py][x0].b;
						}

						//우상단
						else {
							int x0 = x - kw2 - width;				//0부터 시작하는 x
							int px = kw2 - 1 - x0;					//x에 대한 점대칭 변화량 
							opixel[y][x].r = pixel[py][width - kw2 + px].r;
							opixel[y][x].g = pixel[py][width - kw2 + px].g;
							opixel[y][x].b = pixel[py][width - kw2 + px].b;
						}
					}
				}

				//kh2 ~ kh2+height
				for (; y < height + kh2; y++) {
					int y0 = y - kh2;								//0부터 시작하는 y
					int py = kh2 - 1 - y0;							//y에 대한 점대칭 변화량 

					for (x = 0; x < newWidth; x++) {

						//좌중단 
						if (x < kw2) {
							int px = kw2 - 1 - x;					//x에 대한 점대칭 변화량 
							opixel[y][x].r = pixel[y0][px].r;
							opixel[y][x].g = pixel[y0][px].g;
							opixel[y][x].b = pixel[y0][px].b;
						}

						//중중단 
						else if (x < kw2 + width) {
							int x0 = x - kw2;						//0부터 시작하는 x
							opixel[y][x].r = pixel[y0][x0].r;
							opixel[y][x].g = pixel[y0][x0].g;
							opixel[y][x].b = pixel[y0][x0].b;
						}

						//우중단
						else {
							int x0 = x - kw2 - width;				//0부터 시작하는 x
							int px = kw2 - 1 - x0;					//x에 대한 점대칭 변화량 
							opixel[y][x].r = pixel[y0][width - kw2 + px].r;
							opixel[y][x].g = pixel[y0][width - kw2 + px].g;
							opixel[y][x].b = pixel[y0][width - kw2 + px].b;
						}
					}
				}

				//kh2+height ~ BOTTOM
				for (; y < newHeight; y++) {
					int y0 = y - kh2 - height;						//0부터 시작하는 y
					int py = kh2 - 1 - y0;							//y에 대한 점대칭 변화량 

					for (x = 0; x < newWidth; x++) {

						//좌하단 
						if (x < kw2) {
							int px = kw2 - 1 - x;					//x에 대한 점대칭 변화량 
							opixel[y][x].r = pixel[height - kh2 + py][px].r;
							opixel[y][x].g = pixel[height - kh2 + py][px].g;
							opixel[y][x].b = pixel[height - kh2 + py][px].b;
						}

						//중하단 
						else if (x < kw2 + width) {
							int x0 = x - kw2;						//0부터 시작하는 x
							opixel[y][x].r = pixel[height - kh2 + py][x0].r;
							opixel[y][x].g = pixel[height - kh2 + py][x0].g;
							opixel[y][x].b = pixel[height - kh2 + py][x0].b;
						}

						//우하단
						else {
							int x0 = x - kw2 - width;				//0부터 시작하는 x
							int px = kw2 - 1 - x0;					//x에 대한 점대칭 변화량 
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
						opixel[y][x].r = pixel[height - kh2 + y][width - kw2 + x].r;		//좌상단 
						opixel[y][x].g = pixel[height - kh2 + y][width - kw2 + x].g;		//좌상단 
						opixel[y][x].b = pixel[height - kh2 + y][width - kw2 + x].b;		//좌상단 
					}
					for (; x < kw2 + width; x++) {
						opixel[y][x].r = pixel[height - kh2 + y][x - kw2].r;				//중상단 
						opixel[y][x].g = pixel[height - kh2 + y][x - kw2].g;				//중상단 
						opixel[y][x].b = pixel[height - kh2 + y][x - kw2].b;				//중상단 
					}
					for (; x < newWidth; x++) {
						opixel[y][x].r = pixel[height - kh2 + y][x - kw2 - width].r;		//우상단 
						opixel[y][x].g = pixel[height - kh2 + y][x - kw2 - width].g;		//우상단 
						opixel[y][x].b = pixel[height - kh2 + y][x - kw2 - width].b;		//우상단 
					}
				}

				//kh2 ~ kh2+height
				for (; y < height + kh2; y++) {
					int y0 = y - kh2;									//0에서 시작하는 y -> y0

					for (x = 0; x < kw2; x++) {
						opixel[y][x].r = pixel[y0][width - kw2 + x].r;		//좌상단 
						opixel[y][x].g = pixel[y0][width - kw2 + x].g;		//좌상단 
						opixel[y][x].b = pixel[y0][width - kw2 + x].b;		//좌상단 
					}
					for (; x < kw2 + width; x++) {
						opixel[y][x].r = pixel[y0][x - kw2].r;				//중상단 
						opixel[y][x].g = pixel[y0][x - kw2].g;				//중상단 
						opixel[y][x].b = pixel[y0][x - kw2].b;				//중상단 
					}
					for (; x < newWidth; x++) {
						opixel[y][x].r = pixel[y0][x - kw2 - width].r;		//우상단
						opixel[y][x].g = pixel[y0][x - kw2 - width].g;		//우상단
						opixel[y][x].b = pixel[y0][x - kw2 - width].b;		//우상단
					}
				}

				//kh2+height ~ BOTTOM
				for (; y < newHeight; y++) {
					int y0 = y - kh2 - height;							//0에서 시작하는 y -> y0

					for (x = 0; x < kw2; x++) {
						opixel[y][x].r = pixel[y0][width - kw2 + x].r;		//좌상단 
						opixel[y][x].g = pixel[y0][width - kw2 + x].g;		//좌상단 
						opixel[y][x].b = pixel[y0][width - kw2 + x].b;		//좌상단 
					}
					for (; x < kw2 + width; x++) {
						opixel[y][x].r = pixel[y0][x - kw2].r;				//중상단 
						opixel[y][x].g = pixel[y0][x - kw2].g;				//중상단 
						opixel[y][x].b = pixel[y0][x - kw2].b;				//중상단 
					}
					for (; x < newWidth; x++) {
						opixel[y][x].r = pixel[y0][x - kw2 - width].r;		//우상단 
						opixel[y][x].g = pixel[y0][x - kw2 - width].g;		//우상단 
						opixel[y][x].b = pixel[y0][x - kw2 - width].b;		//우상단 
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
			// *16비트이기에 memset, memcpy 연산의 size부분에 2를 곱해주었다. -> 더 좋은 방법은 없을까?
			//-> memset 말고 픽셀 직접 대입으로 변경
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

			case IOP_BORDER_REPLICATE:	//가장자리 값으로 복사 

				//TOP ~ kh2 
				for (y = 0; y < kh2; y++) {
					for (x = 0; x < kw2; x++)
						opixel[y][x] = pixel[0][0];					//좌상단 
					for (; x < kw2 + width; x++)
						opixel[y][x] = pixel[0][x - kw2];			//중상단 
					for (; x < newWidth; x++)
						opixel[y][x] = pixel[0][width - 1];			//우상단 
				}

				//kh2 ~ kh2+height
				for (; y < height + kh2; y++) {
					int py = y - kh2;

					for (x = 0; x < kw2; x++)
						opixel[y][x] = pixel[py][0];					//좌중단 
					for (; x < kw2 + width; x++)
						opixel[y][x] = pixel[py][x - kw2];				//중중단 (이미지)
					for (; x < newWidth; x++)
						opixel[y][x] = pixel[py][width - 1];			//우중단 
				}

				//kh2+height ~ BOTTOM
				for (; y < newHeight; y++) {

					for (x = 0; x < kw2; x++)
						opixel[y][x] = pixel[height - 1][0];					//좌하단 
					for (; x < kw2 + width; x++)
						opixel[y][x] = pixel[height - 1][x - kw2];				//중하단 (이미지)
					for (; x < newWidth; x++)
						opixel[y][x] = pixel[height - 1][width - 1];			//우하단 
				}
				break;

			case IOP_BORDER_REFLECT:

				//TOP ~ kh2 
				for (y = 0; y < kh2; y++) {
					int py = kh2 - 1 - y;							//y에 대한 점대칭 변화량 

					for (x = 0; x < newWidth; x++) {

						//좌상단 
						if (x < kw2) {
							int px = kw2 - 1 - x;					//x에 대한 점대칭 변화량 
							opixel[y][x] = pixel[py][px];
						}

						//중상단 
						else if (x < kw2 + width) {
							int x0 = x - kw2;						//0부터 시작하는 x
							opixel[y][x] = pixel[py][x0];
						}

						//우상단
						else {
							int x0 = x - kw2 - width;				//0부터 시작하는 x
							int px = kw2 - 1 - x0;					//x에 대한 점대칭 변화량 
							opixel[y][x] = pixel[py][width - kw2 + px];
						}
					}
				}

				//kh2 ~ kh2+height
				for (; y < height + kh2; y++) {
					int y0 = y - kh2;								//0부터 시작하는 y
					int py = kh2 - 1 - y0;							//y에 대한 점대칭 변화량 

					for (x = 0; x < newWidth; x++) {

						//좌중단 
						if (x < kw2) {
							int px = kw2 - 1 - x;					//x에 대한 점대칭 변화량 
							opixel[y][x] = pixel[y0][px];
						}

						//중중단 
						else if (x < kw2 + width) {
							int x0 = x - kw2;						//0부터 시작하는 x
							opixel[y][x] = pixel[y0][x0];			//이미지
						}

						//우중단
						else {
							int x0 = x - kw2 - width;				//0부터 시작하는 x
							int px = kw2 - 1 - x0;					//x에 대한 점대칭 변화량 
							opixel[y][x] = pixel[y0][width - kw2 + px];
						}
					}
				}

				//kh2+height ~ BOTTOM
				for (; y < newHeight; y++) {
					int y0 = y - kh2 - height;						//0부터 시작하는 y
					int py = kh2 - 1 - y0;							//y에 대한 점대칭 변화량 

					for (x = 0; x < newWidth; x++) {

						//좌하단 
						if (x < kw2) {
							int px = kw2 - 1 - x;					//x에 대한 점대칭 변화량 
							opixel[y][x] = pixel[height - kh2 + py][px];
						}

						//중하단 
						else if (x < kw2 + width) {
							int x0 = x - kw2;						//0부터 시작하는 x
							opixel[y][x] = pixel[height - kh2 + py][x0];
						}

						//우하단
						else {
							int x0 = x - kw2 - width;				//0부터 시작하는 x
							int px = kw2 - 1 - x0;					//x에 대한 점대칭 변화량 
							opixel[y][x] = pixel[height - kh2 + py][width - kw2 + px];
						}

					}
				}
				break;

			case IOP_BORDER_WRAP:

				//TOP ~ kh2 
				for (y = 0; y < kh2; y++) {
					for (x = 0; x < kw2; x++)
						opixel[y][x] = pixel[height - kh2 + y][width - kw2 + x];		//좌상단 
					for (; x < kw2 + width; x++)
						opixel[y][x] = pixel[height - kh2 + y][x - kw2];				//중상단 
					for (; x < newWidth; x++)
						opixel[y][x] = pixel[height - kh2 + y][x - kw2 - width];		//우상단 
				}

				//kh2 ~ kh2+height
				for (; y < height + kh2; y++) {
					int y0 = y - kh2;									//0에서 시작하는 y -> y0

					for (x = 0; x < kw2; x++)
						opixel[y][x] = pixel[y0][width - kw2 + x];		//좌상단 
					for (; x < kw2 + width; x++)
						opixel[y][x] = pixel[y0][x - kw2];				//중상단 
					for (; x < newWidth; x++)
						opixel[y][x] = pixel[y0][x - kw2 - width];		//우상단 
				}

				//kh2+height ~ BOTTOM
				for (; y < newHeight; y++) {
					int y0 = y - kh2 - height;							//0에서 시작하는 y -> y0

					for (x = 0; x < kw2; x++)
						opixel[y][x] = pixel[y0][width - kw2 + x];		//좌상단 
					for (; x < kw2 + width; x++)
						opixel[y][x] = pixel[y0][x - kw2];				//중상단 
					for (; x < newWidth; x++)
						opixel[y][x] = pixel[y0][x - kw2 - width];		//우상단 
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

	//가우시안 공식
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

	//다 합치면 1되게 나눔 
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

	double scale = sigma*sigma;			//첫번째 스케일 방법(시그마 제곱)
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
			//scale += kernel[y][x];	//두번째 스케일 방법(kernel 적분값)
		}
	}
	//scale = kernel[cy][cx] * 10;		//세번째 스케일 방법(중심값 * 10) -> kernel 중간갑 곱하는거아님
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
					noise = (double)pixel[y][x] + GaussianRand(0., sigma);	//평균이 0이고 시그마가 sigma인
					pixel[y][x] = (byte)limit(noise);
				}
			}
		}
		else if (channels == 3) {
			RGBBYTE** pixel = (RGBBYTE**)img.GetPixels();

			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					noise = (double)pixel[y][x].r + GaussianRand(0., sigma);	//평균이 0이고 시그마가 sigma인
					pixel[y][x].r = (byte)limit(noise);
					noise = (double)pixel[y][x].g + GaussianRand(0., sigma);	//평균이 0이고 시그마가 sigma인
					pixel[y][x].g = (byte)limit(noise);
					noise = (double)pixel[y][x].b + GaussianRand(0., sigma);	//평균이 0이고 시그마가 sigma인
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
					noise = (double)pixel[y][x] + noise16bit;	//평균이 0이고 시그마가 sigma인
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

	int ns = (int)((double)RAND_MAX*ps + 0.5);	//0~32767중에 pp확률로(전체확률1) 나올 수 있는 개수
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
					else if (rgb == TRUE) {		//고춧가루...등rgb각각 랜덤 
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
			x1 = 2. * rand() / RAND_MAX - 1.;		//-1 ~ 1 사이의 랜덤값을 받겠다. 
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
