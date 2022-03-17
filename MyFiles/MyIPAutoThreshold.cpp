#include "stdafx.h"
#include "MyIPAutoThreshold.h"

bool IP_AutoThreshold_Iterative(MyIPImage & img, int * pnThreshold)
{
	if (!img.GetPixels())	return false;

	int depth = img.GetDepth();
	int channels = img.GetChannels();
	int width = img.GetWidth();
	int height = img.GetHeight();
	int size = width*height;
	
	unsigned int *histo, *chisto, *cxhisto;
	BYTE** Pixels = (BYTE**)img.GetPixels();

	int t0 = -1, t1 = 0;
	double mL, mR, mT;
	//int cnt;			//속도 줄이기1 cnt, size-cnt로 구분 -> cumulative histo...
	//int mLcnt, mRcnt;	

	/*---------------------------------------------------------------------------------------------------------------------------------------*/
	//속도 줄이기2 for문 65536번 반복하지말고 histo 이용해서 256번만 반복하도록 
	//속도 줄이기3 평균값 미리 계산해두기
	//속도 줄이기4 적분이용 cumulative histogram해서 mLcnt, mRcnt없에서 chisto[t0]으로 사용 
	//속도 줄이기5 x*histo[x]를 미리 cumulative histogram으로 구해놓기
	switch (depth)
	{
	case CV_8U:
	case CV_8S:
		histo = new unsigned int[256];
		chisto = new unsigned int[256];
		cxhisto = new unsigned int[256];
		if (!histo)						return false;
		if (!chisto)					return false;
		if (!chisto)					return false;
		if (!IP_Histogram(img, histo))	return false;

		mT = 0;
		chisto[0] = histo[0];
		cxhisto[0] = 0;
		for (int x = 1; x < 256; x++) {
			chisto[x] = chisto[x - 1] + histo[x];						//적분...구하기 cumulative histogram
			cxhisto[x] = cxhisto[x - 1] + (x*histo[x]);					//x*histo[x]미리 구해두기 
			//mT += (double)(x*histo[x]);
		}
		mT = (double)cxhisto[255];										//mT 전체 x* histo 값 
		t0 = (int)(mT / (double)size + 0.5);							//t0 시작 T값 평균으로 


		while (1)
		{
			//		mL = mR = mLcnt = mRcnt = 0;							//mR, mL을 구하기 위한 변수들 0으로 초기화 				//속도 줄이기
			// 		for (int y = 0; y < height; y++) {
			// 			for (int x = 0; x < width; x++) {
			// 				if (Pixels[y][x] <= t0) {
			// 					mL += (double)Pixels[y][x];
			// 				}
			// 				else if (Pixels[y][x] > t0) {
			// 					mR += (double)Pixels[y][x];
			// 					cnt++;
			// 				}
			// 			}
			// 		}

			/*---------------------------------------------------------------------------------------------------------------------------------------*/

			// 		mL = 0;
			// 		for (int x = 1; x < t0; x++) 
			// 			mL += (double)(x*histo[x]);
			//>>
			mL = cxhisto[t0];
			mR = mT - mL;

			if (chisto[t0] > 0)	mL /= (double)chisto[t0];
			else mL = 0;
			if (chisto[t0] != size)	mR /= (double)size - chisto[t0];
			else mR = 0;
			t1 = (int)((mL + mR + 0.5) / 2);


			//1. 초기 Threshold 구하기
			//2. 새로운 Threshold를 계산한다 T1
			// 		mL = mL / (double)(size - cnt);
			// 		mR = mR / (double)cnt;
			// 		if (t0 == -1) 
			// 			t0 = mR;											//초기 Threshold to은 -1보다 큰 픽셀값의 평균을 구한 mR과 같다! 
			// 		else 
			// 			t1 = (mL + mR) / 2 + 0.5;							//반올림 해주기!!!     +0.5


			/*---------------------------------------------------------------------------------------------------------------------------------------*/
			//3. | t0-t1 | <= 1이면 stop, T1값을 return 
			// 		if (t1 != 0) {											//먼저 t0의 값을 받고나서 두번째 while문에서 t1의 값을 받고나서 체크하도록 	
			// 			int val = t0 - t1;									//t0-t1의 값 구하고 
			// 			val = (val) >= 0 ? val : -val;						//절대값 구해주고~
			// 				
			// 			if (val <= 1) {										//차이가 1보다 작거나 같으면
			// 				*pnThreshold = t1;								//Threshold값을 t1값으로 반환해주고 
			// 				break;											//while문을 빠져나가준다
			// 			}
			// 			else {												//차이가 1보다 크다면 
			// 				t0 = t1;										//threshold값을 t1값으로 바꿔주고 다시 while반복 진행한다.
			// 			}
			// 		}
			if (abs(t0 - t1) <= 1) break;
			else t0 = t1;
		}
		break;

	case CV_16U:
	case CV_16S:
		histo = new unsigned int[65536];
		chisto = new unsigned int[65536];
		cxhisto = new unsigned int[65536];
		if (!histo)						return false;
		if (!chisto)					return false;
		if (!chisto)					return false;
		if (!IP_Histogram(img, histo))	return false;

		mT = 0;
		chisto[0] = histo[0];
		cxhisto[0] = 0;
		for (int x = 1; x < 65536; x++) {
			chisto[x] = chisto[x - 1] + histo[x];						//적분...구하기 cumulative histogram
			cxhisto[x] = cxhisto[x - 1] + (x*histo[x]);					//x*histo[x]미리 구해두기 
																		//mT += (double)(x*histo[x]);
		}
		mT = (double)cxhisto[65535];										//mT 전체 x* histo 값 
		t0 = (int)(mT / (double)size + 0.5);							//t0 시작 T값 평균으로 


		while (1)
		{
			mL = cxhisto[t0];
			mR = mT - mL;

			if (chisto[t0] > 0)	mL /= (double)chisto[t0];
			else mL = 0;
			if (chisto[t0] != size)	mR /= (double)size - chisto[t0];
			else mR = 0;
			t1 = (int)((mL + mR + 0.5) / 2);

			if (abs(t0 - t1) <= 1) break;	//t0-t1의 값이 1보다 작거나 같을 때이기 때문에 8비트보다 정밀히 계산가능. 8비트처럼하려면 256으로 하면 됨 
			else t0 = t1;
		}
		break;

	default:
		return false;
	}

	*pnThreshold = t1;											//밖으로 뺐음!
	delete[] cxhisto;											//할당과 반대의 순서로 delete해주는게 pc에게 좋다고 함-주교수님-
	delete[] chisto;											//할당과 반대의 순서로 delete해주는게 pc에게 좋다고 함-주교수님-
	delete[] histo;
	return true;
}

bool IP_AutoThreshold_Otsu(MyIPImage & img, int * pnThreshold)
{
	if (!img.GetPixels())	return false;

	int depth = img.GetDepth();
	int channels = img.GetChannels();
	int width = img.GetWidth();
	int height = img.GetHeight();
	int size = width*height;

	unsigned int *histo, *chisto, *cxhisto;
	BYTE** Pixels = (BYTE**)img.GetPixels();

	int t0 = -1, t0_min = 0, t0_max = 0;										//최종 t0값
	int t1, t2;
	double mL, mR, mT=0;
	double p1 = 0.0, p2 = 0.0;									//Probability 확률 
	double P = 0.0, Pmax = 0;
	
	switch (depth)
	{
	case CV_8U:
	case CV_8S:
		histo = new unsigned int[256];
		chisto = new unsigned int[256];
		cxhisto = new unsigned int[256];
		if (!histo)						return false;
		if (!chisto)					return false;
		if (!chisto)					return false;
		if (!IP_Histogram(img, histo))	return false;

		chisto[0] = histo[0];
		cxhisto[0] = 0;
		if (histo[0] != 0 && t0 == -1)	t0 = histo[0];				//OTSU 1. t0 는 histo[]가 0이 아닌 값부터 시작
		for (int x = 1; x < 256; x++) {
			chisto[x] = chisto[x - 1] + histo[x];					//chisto 구하기 cumulative histogram
			cxhisto[x] = cxhisto[x - 1] + (x*histo[x]);				//cshisto(x*histo[x])미리 구해두기 
																	//mT += (double)(x*histo[x]);
			if (histo[x] != 0 && t0 == -1)	t0 = x;					//OTSU 1. t0 는 histo[]가 0이 아닌 값부터 시작
		}
		mT = (double)cxhisto[255] / size;							//전체 평균


		for (t0 = 0; t0 < 255; t0++) {
			if (chisto[t0] > 0)
				break;
		}
		t1 = t0;
		for (t0 = 255; t0 > t1; t0--) {
			if (chisto[t0] > 0)
				break;
		}
		t2 = t0;

		for (t0 = t1; t0 < t2; t0++)
		{

			mL = cxhisto[t0];	//0 ~ t		까지의 합
			mR = cxhisto[255] - mL;		//t ~ L-1	까지의 합 

			if (chisto[t0] > 0)	mL /= (double)chisto[t0];			//0 ~ t 에 값이 하나라도 있으면 mL평균 구하기
			else mL = 0;											//없으면 검은 영상이니 0
			if (chisto[t0] != size)	mR /= (double)size - chisto[t0];//0 ~ t까지 모든 픽셀이 있지 않다면. mR평균 구해준다.
			else mR = 0;											//0 ~ t까지 모든 픽셀이 있다면. 255

			p1 = (double)((double)chisto[t0] / (double)size);		//mL 의 분포?확률? 구해주기 
																	//속도 줄이기 2 p2 -> 1-p1
																	//p2 = 1.0 - p1;											//mR 의 분포?확률? 구해주기  p1+p2=1


																	// 		double temp1 = (pow((mL - mT), 2));
																	// 		double temp2 = (pow((mR - mT), 2));

																	//P = p1*(mL-mT)*(mL-mT) + p2*(mR-mT)*(mR-mT);		//그냥 식 
																	//속도 줄이기 1 수식 최적화 
			P = p1*(1. - p1)*(mL - mR)*(mL - mR);	//최적화 식 

			if (P > Pmax) {
				Pmax = P;
				t0_min = t0;
			}
			else if (P == Pmax) {
				t0_max = t0;
			}
		}
		break;

	case CV_16U:
	case CV_16S:
		histo = new unsigned int[65536];
		chisto = new unsigned int[65536];
		cxhisto = new unsigned int[65536];
		if (!histo)						return false;
		if (!chisto)					return false;
		if (!chisto)					return false;
		if (!IP_Histogram(img, histo))	return false;

		chisto[0] = histo[0];
		cxhisto[0] = 0;
		if (histo[0] != 0 && t0 == -1)	t0 = histo[0];				//OTSU 1. t0 는 histo[]가 0이 아닌 값부터 시작
		for (int x = 1; x < 65536; x++) {
			chisto[x] = chisto[x - 1] + histo[x];					//chisto 구하기 cumulative histogram
			cxhisto[x] = cxhisto[x - 1] + (x*histo[x]);				//cshisto(x*histo[x])미리 구해두기 
																	//mT += (double)(x*histo[x]);
			if (histo[x] != 0 && t0 == -1)	t0 = x;					//OTSU 1. t0 는 histo[]가 0이 아닌 값부터 시작
		}
		mT = (double)cxhisto[65535] / size;							//전체 평균


		for (t0 = 0; t0 < 65535; t0++) {
			if (chisto[t0] > 0)
				break;
		}
		t1 = t0;
		for (t0 = 65535; t0 > t1; t0--) {
			if (chisto[t0] > 0)
				break;
		}
		t2 = t0;

		for (t0 = t1; t0 < t2; t0++)
		{

			mL = cxhisto[t0];	//0 ~ t		까지의 합
			mR = cxhisto[65535] - mL;		//t ~ L-1	까지의 합 

			if (chisto[t0] > 0)	mL /= (double)chisto[t0];			//0 ~ t 에 값이 하나라도 있으면 mL평균 구하기
			else mL = 0;											//없으면 검은 영상이니 0
			if (chisto[t0] != size)	mR /= (double)size - chisto[t0];//0 ~ t까지 모든 픽셀이 있지 않다면. mR평균 구해준다.
			else mR = 0;											//0 ~ t까지 모든 픽셀이 있다면. 255

			p1 = (double)((double)chisto[t0] / (double)size);		//mL 의 분포?확률? 구해주기 
																	//속도 줄이기 2 p2 -> 1-p1
																	//p2 = 1.0 - p1;											//mR 의 분포?확률? 구해주기  p1+p2=1


																	// 		double temp1 = (pow((mL - mT), 2));
																	// 		double temp2 = (pow((mR - mT), 2));

																	//P = p1*(mL-mT)*(mL-mT) + p2*(mR-mT)*(mR-mT);		//그냥 식 
																	//속도 줄이기 1 수식 최적화 
			P = p1*(1. - p1)*(mL - mR)*(mL - mR);	//최적화 식 

			if (P > Pmax) {
				Pmax = P;
				t0_min = t0;
			}
			else if (P == Pmax) {
				t0_max = t0;
			}
		}
		break;

	default:
		return false;
	}
	

	if (t0_max > t0_min)
		*pnThreshold = (t0_max + t0_min) / 2;
	else
		*pnThreshold = t0_min;									//밖으로 뺐음!
	delete[] cxhisto;											//할당과 반대의 순서로 delete해주는게 pc에게 좋다고 함-주교수님-
	delete[] chisto;											//할당과 반대의 순서로 delete해주는게 pc에게 좋다고 함-주교수님-
	delete[] histo;
	return true;
}


bool IP_AutoThreshold_Entropy(MyIPImage & img, int * pnThreshold)
{
	if (!img.GetPixels())	return false;

	int depth = img.GetDepth();
	int channels = img.GetChannels();
	int width = img.GetWidth();
	int height = img.GetHeight();
	int size = width*height;
	BYTE** Pixels = (BYTE**)img.GetPixels();

	unsigned int *histo, *chisto;								//cxhisto 필요없음.
	double *plogp;												//p*log(p)적분값 구해두기

	int t = -1, t_min = 0, t1 = 0, t2 = 0;						//최종 t0값
	unsigned int n1 = 0, n2 = 0;
	double H1, H2;												//Probability 확률 
	double V = 0., Vmax = 0.;
	switch (depth)
	{
	case CV_8U:
	case CV_8S:
		histo = new unsigned int[256];
		chisto = new unsigned int[256];
		plogp = new double[256];
		if (!histo)						return false;
		if (!chisto)					return false;
		if (!plogp)						return false;
		if (!IP_Histogram(img, histo))	return false;

		//chisto 구하기 cumulative histogram
		chisto[0] = histo[0];
		for (int x = 1; x < 256; x++) {
			chisto[x] = chisto[x - 1] + histo[x];					
		}

		//>>chisto가 0이 아닌값을 찾고 0~부터는 t1, 255~는 t2에 저장
		for (t = 0; t < 255; t++) {
			if (histo[t] > 0)
				break;
		}
		t1 = t;
		for (t = 255; t > t1; t--) {
			if (histo[t] > 0)
				break;
		}
		t2 = t;

		//plogp 적분 미리 구해두기
		plogp[t1] = (double)histo[t1] * log((double)histo[t1]);
		for (t = t1 + 1; t <= t2; t++) {
			if (histo[t] > 0)
				plogp[t] = plogp[t - 1] + ((double)histo[t] * log((double)histo[t]));	//log(0) -> error
				//plogp[t] = ((double)histo[t - 1] * log((double)histo[t - 1])) + ((double)histo[t] * log((double)histo[t]));	쑈발 이거 이상해염 t-1이 0일수도있자나 
			else
				plogp[t] = plogp[t - 1];
		}

		for (t = t1; t < t2; t++)	//어차피 t2까지 가면 흰색이니까~~~~
		{
			if (chisto[t] == chisto[255]) break;							//오른쪽에 아무것도 없당
			n1 = chisto[t];
			n2 = chisto[255] - n1;

			H1 = 0.;
			H1 = log((double)n1) - ((double)plogp[t] / (double)n1);		
// 			for (int i = t1; i <= t; i++) {	//시그마 
// 				if (histo[i] > 0) {							//log(0)을 계산하지 못함. histo[i]가 0이면 pi가 0이니까...
// 					Pi = (double)histo[i] / (double)n1;
// 					H1 += -Pi*log(Pi);						//log(0)은 -무한대니까...
// 				}
// 			}

			H2 = 0.;
			H2 = log((double)n2) - (((double)plogp[t2] - (double)plogp[t]) / (double)n2);
// 			for (int i = t+1; i <= t2; i++) {	//시그마 
// 				if (histo[i] > 0) {
// 					Pi = (double)histo[i] / (double)n2;
// 					H2 += -Pi*log(Pi);
// 				}
// 			}

			V = H1 + H2;

			if (V > Vmax) {
				Vmax = V;
				t_min = t;
			}
		}
		break;

	case CV_16U:
	case CV_16S:
		histo = new unsigned int[65536];
		chisto = new unsigned int[65536];
		plogp = new double[65536];
		if (!histo)						return false;
		if (!chisto)					return false;
		if (!plogp)						return false;
		if (!IP_Histogram(img, histo))	return false;

		//chisto 구하기 cumulative histogram
		chisto[0] = histo[0];
		for (int x = 1; x < 65536; x++) {
			chisto[x] = chisto[x - 1] + histo[x];
		}

		//>>chisto가 0이 아닌값을 찾고 0~부터는 t1, 255~는 t2에 저장
		for (t = 0; t < 65535; t++) {
			if (histo[t] > 0)
				break;
		}
		t1 = t;
		for (t = 65535; t > t1; t--) {
			if (histo[t] > 0)
				break;
		}
		t2 = t;

		//plogp 적분 미리 구해두기
		plogp[t1] = (double)histo[t1] * log((double)histo[t1]);
		for (t = t1 + 1; t <= t2; t++) {
			if (histo[t] > 0)
				plogp[t] = plogp[t - 1] + ((double)histo[t] * log((double)histo[t]));	//log(0) -> error
			else
				plogp[t] = plogp[t - 1];
		}

		for (t = t1; t < t2; t++)	//어차피 t2까지 가면 흰색이니까~~~~
		{
			if (chisto[t] == chisto[65535]) break;							//오른쪽에 아무것도 없당
			n1 = chisto[t];
			n2 = chisto[65535] - n1;

			H1 = 0.;
			H1 = log((double)n1) - ((double)plogp[t] / (double)n1);
			// 			for (int i = t1; i <= t; i++) {	//시그마 
			// 				if (histo[i] > 0) {							//log(0)을 계산하지 못함. histo[i]가 0이면 pi가 0이니까...
			// 					Pi = (double)histo[i] / (double)n1;
			// 					H1 += -Pi*log(Pi);						//log(0)은 -무한대니까...
			// 				}
			// 			}

			H2 = 0.;
			H2 = log((double)n2) - (((double)plogp[t2] - (double)plogp[t]) / (double)n2);
			// 			for (int i = t+1; i <= t2; i++) {	//시그마 
			// 				if (histo[i] > 0) {
			// 					Pi = (double)histo[i] / (double)n2;
			// 					H2 += -Pi*log(Pi);
			// 				}
			// 			}

			V = H1 + H2;

			if (V > Vmax) {
				Vmax = V;
				t_min = t;
			}
		}
		break;

	default:
		return false;
	}


	*pnThreshold = t_min;									//밖으로 뺐음!
	delete[] plogp;
	delete[] chisto;										//할당과 반대의 순서로 delete해주는게 pc에게 좋다고 함-주교수님-
	delete[] histo;
	return true;
}


bool IP_AutoThreshold_OpenCV(Mat & img, int type, int method)
{
	if (!img.data)	return false;

	int depth = img.depth();
	int channel = img.channels();
	int width = img.cols;
	int height = img.rows;

	if (channel != 1)	return false;
	switch (depth)
	{
	case CV_8U:
	case CV_8S:
		threshold(img, img, 0, 255, type | method);
		break;

	case CV_16U:
	case CV_16S:
		threshold(img, img, 0, 65535, type | method);
		break;

	default:
		break;
	}
	if (!img.data)	return false;

	return true;
}