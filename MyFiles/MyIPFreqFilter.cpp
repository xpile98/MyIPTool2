#include "stdafx.h"
#include "MyIPFreqFilter.h"


bool IpFFT2D(Mat & src, Mat & dst)
{
	if (!src.data)	return false;
	if (src.channels() != 1)	return false;

	Mat pad_img = IpZeroPadding(src);
	dft(pad_img, dst, 0);

	return true;
}


bool IpIFFT2D(Mat & src, Mat & ref, bool DoScale)
{
	if (!src.data)	return false;
	//if (src.type() != CV_64FC2)	return false;

	if (!ref.data) return false;
	if ((ref.type() != CV_8UC1) && (ref.type() != CV_16UC1)) return false;

	Mat ifft_img;
	dft(src, ifft_img, DFT_INVERSE + DFT_SCALE);

	Mat tmp = Mat(ref.size(), CV_64F);
	int height = ref.rows;
	int width = ref.cols;

	for (int y = 0; y < height; y++) {
		Complex<double> *cptr = ifft_img.ptr<Complex<double>>(y);
		double *dptr = tmp.ptr<double>(y);
		for (int x = 0; x < width; x++) {
			if ((x + y) % 2) dptr[x] = -cptr[x].re;
			else dptr[x] = cptr[x].re;
		}
	}

	if (DoScale) {
		if (ref.depth() == CV_8U)
			normalize(tmp, tmp, 0, 255, NORM_MINMAX);
		else
			normalize(tmp, tmp, 0, 65535, NORM_MINMAX);
	}
	tmp.convertTo(ref, ref.depth());
	return true;
}


bool IpFFTSpectrum(Mat & src, Mat & dst)
{
	if (!src.data)	return false;
	if (src.type() != CV_64FC2)	return false;

	int height = src.rows;
	int width = src.cols;

	dst = Mat(src.size(), CV_64FC1);

	for (int y = 0; y < dst.rows; y++) {
		double *dptr = dst.ptr<double>(y);
		Complex<double> *cptr = src.ptr<Complex<double>>(y);
		for (int x = 0; x < dst.cols; x++) {
			dptr[x] = log(1. + abs(cptr[x]));
		}
	}

	normalize(dst, dst, 0, 255, NORM_MINMAX);
	dst.convertTo(dst, CV_8U);

	return true;
}


bool IpFreqFilter(Mat & src, Mat & filter, BOOL scale, BOOL average)
{
	if (!src.data) return false;
	if (src.channels() != 1) return false;

	//1. FFT & Zero Padding
	Mat fft_img;
	if (!IpFFT2D(src, fft_img))	return false;						//Make Pad Image

	//2. Multiply by filter coeffcient
	int fheight = fft_img.rows;
	int fwidth = fft_img.cols;

	Complex<double> F00 = fft_img.at<Complex<double>>(fheight / 2, fwidth / 2);
	for (int y = 0; y < fheight; y++) {
		Complex<double> *cptr = fft_img.ptr<Complex<double>>(y);
		double *fval = filter.ptr<double>(y);
		for (int x = 0; x < fwidth; x++)
			cptr[x] *= fval[x];
	}

	//3. Inverse Fourier Transform.
	if (average)
		fft_img.at<Complex<double>>(fheight / 2, fwidth / 2) = F00;
	if (!IpIFFT2D(fft_img, src, (bool)scale)) return false;



	return true;
}

bool IpFilter2D_DFT(Mat &img, Mat &dst, int h, int w, double ** kernel)
{
	if (!img.data)	return false;

	//1. get new image size  (2^n)
	int height = img.rows + h - 1;
	int width = img.cols + w - 1;
	int newHeight = 1 << (int)ceil(log2(height));
	int newWidth = 1 << (int)ceil(log2(width));

	//2. make kernel to Mat
	Mat img_kernel(h, w, CV_64FC1, Scalar(0, 0));
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			img_kernel.at<double>(y, x) = kernel[y][x];
		}
	}

	//3. Make Zeropdded Image 
	Mat tmp_image, tmp_kernel;
	img.convertTo(tmp_image, CV_64FC1);
	img_kernel.convertTo(tmp_kernel, CV_64FC1);

	Mat dst_image(newHeight, newWidth, CV_64FC2, Scalar(0.0));
	Mat dst_kernel(newHeight, newWidth, CV_64FC2, Scalar(0.0));


	for (int y = 0; y < tmp_image.rows; y++) {
		double *sptr_image = tmp_image.ptr<double>(y);
		Complex<double> *cptr_img = dst_image.ptr<Complex<double>>(y);
		for (int x = 0; x < tmp_image.cols; x++) {
			if ((x + y) % 2)	cptr_img[x].re = -sptr_image[x];
			else cptr_img[x].re = sptr_image[x];
//			cptr_img[x].re = sptr_image[x];
		}
	}

	for (int y = 0; y < tmp_kernel.rows; y++) {
		double *sptr_kernel = tmp_kernel.ptr<double>(y);							//input image ptr 
		Complex<double> *cptr_kernel = dst_kernel.ptr<Complex<double>>(y);		//output image ptr 
		for (int x = 0; x < tmp_kernel.cols; x++) {
			if ((x + y) % 2)	cptr_kernel[x].re = -sptr_kernel[x];
			else cptr_kernel[x].re = sptr_kernel[x];
// 			cptr_kernel[x].re = sptr_kernel[x];
		}
	}

	//3.5 Show Zeropadded Image

	//normalize(norm_image, norm_image, 0, 255, NORM_MINMAX);
	//normalize(dst_kernel, norm_kernel, 0, 255, NORM_MINMAX);
	//norm_image.convertTo(norm_image, CV_8U);
	//norm_kernel.convertTo(norm_kernel, CV_8U);
	//AfxNewImage(_T("image"), norm_image);
	//AfxNewImage(_T("kernel"), norm_kernel);



	//4. Fourier Transform
	Mat fft_image, fft_kernel;
	dft(dst_image, fft_image, 0);
	dft(dst_kernel, fft_kernel, 0);

	//4.5 Show Spectrum Image
// 	Mat spc_image, spc_kernel;
// 	IpFFTSpectrum(fft_image, spc_image);
// 	IpFFTSpectrum(fft_kernel, spc_kernel);
// 	AfxNewImage(_T("spectrum image"), spc_image);
// 	AfxNewImage(_T("spectrum kernel"), spc_kernel);


	//5. Multiply Filter 
	for (int y = 0; y < newHeight; y++) {
		Complex<double> *cptr_image = fft_image.ptr<Complex<double>>(y);		//output image ptr 
		Complex<double> *cptr_kernel = fft_kernel.ptr<Complex<double>>(y);		//output image ptr 
		for (int x = 0; x < newWidth; x++) {
			//빠가같은 방법
			//cptr_image[x].re *= cptr_kernel[x].re;
			//cptr_image[x].im *= cptr_kernel[x].im;

			//곱셈공식
			double real = (cptr_image[x].re * cptr_kernel[x].re) - (cptr_image[x].im * cptr_kernel[x].im);
			double imag = (cptr_image[x].re * cptr_kernel[x].im) + (cptr_image[x].im * cptr_kernel[x].re);

			cptr_image[x].re = real;
			cptr_image[x].im = imag;
		}
	}

	//5.5 Show spectrum image
// 	Mat spc_multiple;
// 	IpFFTSpectrum(fft_image, spc_multiple);
// 	AfxNewImage(_T("spectrum image*kernel"), spc_multiple);

	//6. Inverse Fourier Transform
	//Mat ifft_image(img.size(), img.type());

	if (!IpIFFT2D(fft_image, dst))	return false;				//Make Pad Image

	return true;
}


Mat IpZeroPadding(Mat &src)
{

	int newheight = 1 << (int)ceil((double)(log(src.rows)) / (double)(log(2)));
	int newwidth = 1 << (int)ceil((double)(log(src.cols)) / (double)(log(2)));

	//openCV Book 
// 	int m = 1 << (int)ceil(log2(src.rows));
// 	int n = 1 << (int)ceil(log2(src.cols));
	Mat tmp;														//zero padding 할 때 임시 Mat 객체 말고 원본 객체를 사용해서 convert 할 시 오류 발생
	src.convertTo(tmp, CV_64F);
	Mat dst(newheight, newwidth, CV_64FC2, Scalar(0.0));			//make pad image (0)

	for (int y = 0; y < tmp.rows; y++) {
		double *sptr = tmp.ptr<double>(y);							//input image ptr 
		Complex<double> *cptr = dst.ptr<Complex<double>>(y);		//output image ptr 
		for (int x = 0; x < tmp.cols; x++) {
			if ((x + y) % 2)	cptr[x].re = -sptr[x];
			else cptr[x].re = sptr[x];
		}
	}

	return dst;
}

Mat IpMakeFilter(Mat & src, int filterType, int filterShape, int cofVal, int bpfVal, int butVal)
{
	if (butVal <= 0)	return Mat();
	
	CString title;

	//1. get filter size
	int height = src.rows;
	int width = src.cols;
	int h2 = height / 2;
	int w2 = width / 2;


	//2. make filter
	Mat filter(height, width, CV_64FC1,Scalar(0.0));

	//3. design filter
	int x, y, y2, x2y2;							//for loop x, y, y^2, x^2 + y^2
	int D2 = cofVal*cofVal;						//Cut Off Value ^2

	int BW_low = (cofVal - (bpfVal / 2))*(cofVal - (bpfVal / 2));	//{cof - (bw/2)}^2
	int BW_high = (cofVal + (bpfVal / 2))*(cofVal + (bpfVal / 2));	//{cof + (bw/2)}^2
	if ((cofVal - (bpfVal / 2)) < 0)	BW_low = 0;

	double dLowVal, dHighVal;					//temp Valriable
	double *ptr;

	switch (filterShape)
	{
	case 0:		//Ideal Filter Shape
		switch (filterType)
		{
		case 0:		//LPF
			for (y = 0; y < height; y++) {
				ptr = filter.ptr<double>(y);
				y2 = (y - h2)*(y - h2);				//y^2
				for (x = 0; x < width; x++) {
					if ((y2 + (x - w2)*(x - w2) <= D2))	//y^2 + x^2)	//작거나 같을 때 
						ptr[x] = 1;
				}
			}
			title = _T("LPF(Ideal Type)");
			break;
		case 1:		//HPF
			for (y = 0; y < height; y++) {
				ptr = filter.ptr<double>(y);
				y2 = (y - h2)*(y - h2);				//y^2
				for (x = 0; x < width; x++) {
					if ((y2 + (x - w2)*(x - w2) > D2))	//y^2 + x^2)	//클 때 
						ptr[x] = 1;
				}
			}
			title = _T("HPF(Ideal Type)");
			break;
		case 2:		//BPF
			for (y = 0; y < height; y++) {
				ptr = filter.ptr<double>(y);
				y2 = (y - h2)*(y - h2);				//y^2
				for (x = 0; x < width; x++) {
					x2y2 = y2 + (x - w2)*(x - w2);
					if (x2y2 >= BW_low && x2y2 <= BW_high)	//D2 - BW_high보다 크고 D2 + BW_high 보다 작거나 같으면 1
						ptr[x] = 1;
				}
			}
			title = _T("BPF(Ideal Type)");
			break;
		case 3:		//BRF
			for (y = 0; y < height; y++) {
				ptr = filter.ptr<double>(y);
				y2 = (y - h2)*(y - h2);				//y^2
				for (x = 0; x < width; x++) {
					x2y2 = y2 + (x - w2)*(x - w2);
					if (x2y2 <= BW_low || x2y2 > BW_high)	//D2 - BW_high 이하 거나 HPF보다 크면 1
						ptr[x] = 1;
				}
			}
			title = _T("BRF(Ideal Type)");
			break;
		}
		break;
	case 1:		//Gaussian Filter Shape
		switch (filterType)
		{
		case 0:		//LPF
			for (y = 0; y < height; y++) {
				ptr = filter.ptr<double>(y);
				y2 = (y - h2)*(y - h2);		
				for (x = 0; x < width; x++) {
					x2y2 = y2 + (x - w2)*(x - w2);
					dLowVal = exp(-x2y2 / (2.0 * D2));
					ptr[x] = dLowVal;
				}
			}
			title = _T("LPF(Gaussian Type)");
			break;
		case 1:		//HPF
			for (y = 0; y < height; y++) {
				ptr = filter.ptr<double>(y);
				y2 = (y - h2)*(y - h2);			
				for (x = 0; x < width; x++) {
					x2y2 = y2 + (x - w2)*(x - w2);
					dHighVal = 1.0 - exp(-x2y2 / (2.0 * D2));
					ptr[x] = dHighVal;
				}
			}
			title = _T("HPF(Gaussian Type)");
			break;
		case 2:		//BPF
			for (y = 0; y < height; y++) {
				ptr = filter.ptr<double>(y);
				y2 = (y - h2)*(y - h2);			
				for (x = 0; x < width; x++) {
					x2y2 = y2 + (x - w2)*(x - w2);
					dLowVal = exp(-x2y2 / (2.0 * BW_high));			//LPF
					dHighVal = 1.0 - exp(-x2y2 / (2.0 * BW_low));	//HPF
					ptr[x] = dLowVal * dHighVal;
				}
			}
			title = _T("BPF(Gaussian Type)");
			break;
		case 3:		//BRF
			for (y = 0; y < height; y++) {
				ptr = filter.ptr<double>(y);
				y2 = (y - h2)*(y - h2);				
				for (x = 0; x < width; x++) {
					x2y2 = y2 + (x - w2)*(x - w2);
					dLowVal = exp(-x2y2 / (2.0 * BW_high));			//LPF
					dHighVal = 1.0 - exp(-x2y2 / (2.0 * BW_low));	//HPF
					ptr[x] = 1 - (dLowVal * dHighVal);
				}
			}
			title = _T("BRF(Gaussian Type)");
			break;
		}
		break;
	case 2:		//Butterworth Filter Shape
		switch (filterType)
		{
		case 0:		//LPF
			for (y = 0; y < height; y++) {
				ptr = filter.ptr<double>(y);
				y2 = (y - h2)*(y - h2);
				for (x = 0; x < width; x++) {
					x2y2 = y2 + (x - w2)*(x - w2);
					ptr[x] = 1. / (1. + pow((double)x2y2 / (double)D2, butVal));
				}
			}
			title = _T("LPF(Butterworth Type)");
			break;
		case 1:		//HPF
			for (y = 0; y < height; y++) {
				ptr = filter.ptr<double>(y);
				y2 = (y - h2)*(y - h2);
				for (x = 0; x < width; x++) {
					x2y2 = y2 + (x - w2)*(x - w2);
					ptr[x] = 1. / (1. + pow((double)D2 / (double)x2y2, butVal));
				}
			}
			title = _T("HPF(Butterworth Type)");
			break;
		case 2:		//BPF
			for (y = 0; y < height; y++) {
				ptr = filter.ptr<double>(y);
				y2 = (y - h2)*(y - h2);
				for (x = 0; x < width; x++) {
					x2y2 = y2 + (x - w2)*(x - w2);
					dLowVal = 1. / (1. + pow((double)x2y2 / (double)BW_high, butVal));
					dHighVal = 1. / (1. + pow((double)BW_low / (double)x2y2, butVal));
					ptr[x] = dLowVal * dHighVal;
				}
			}
			title = _T("BPF(Butterworth Type)");
			break;
		case 3:		//BRF
			for (y = 0; y < height; y++) {
				ptr = filter.ptr<double>(y);
				y2 = (y - h2)*(y - h2);
				for (x = 0; x < width; x++) {
					x2y2 = y2 + (x - w2)*(x - w2);
					dLowVal = 1. / (1. + pow((double)x2y2 / (double)BW_high, butVal));
					dHighVal = 1. / (1. + pow((double)BW_low / (double)x2y2, butVal));
					ptr[x] = 1. - (dLowVal * dHighVal);
				}
			}
			title = _T("BRF(Butterworth Type)");
			break;
		}
		break;
	}


	//9. Display Filter 
	Mat filter_img(height, width, CV_64F, Scalar(0.0));
	normalize(filter, filter_img, 0, 255, NORM_MINMAX);
	filter_img.convertTo(filter_img, CV_8U);
	AfxNewImage(title, filter_img);

	return filter;
}