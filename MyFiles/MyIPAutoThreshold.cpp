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
	//int cnt;			//�ӵ� ���̱�1 cnt, size-cnt�� ���� -> cumulative histo...
	//int mLcnt, mRcnt;	

	/*---------------------------------------------------------------------------------------------------------------------------------------*/
	//�ӵ� ���̱�2 for�� 65536�� �ݺ��������� histo �̿��ؼ� 256���� �ݺ��ϵ��� 
	//�ӵ� ���̱�3 ��հ� �̸� ����صα�
	//�ӵ� ���̱�4 �����̿� cumulative histogram�ؼ� mLcnt, mRcnt������ chisto[t0]���� ��� 
	//�ӵ� ���̱�5 x*histo[x]�� �̸� cumulative histogram���� ���س���
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
			chisto[x] = chisto[x - 1] + histo[x];						//����...���ϱ� cumulative histogram
			cxhisto[x] = cxhisto[x - 1] + (x*histo[x]);					//x*histo[x]�̸� ���صα� 
			//mT += (double)(x*histo[x]);
		}
		mT = (double)cxhisto[255];										//mT ��ü x* histo �� 
		t0 = (int)(mT / (double)size + 0.5);							//t0 ���� T�� ������� 


		while (1)
		{
			//		mL = mR = mLcnt = mRcnt = 0;							//mR, mL�� ���ϱ� ���� ������ 0���� �ʱ�ȭ 				//�ӵ� ���̱�
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


			//1. �ʱ� Threshold ���ϱ�
			//2. ���ο� Threshold�� ����Ѵ� T1
			// 		mL = mL / (double)(size - cnt);
			// 		mR = mR / (double)cnt;
			// 		if (t0 == -1) 
			// 			t0 = mR;											//�ʱ� Threshold to�� -1���� ū �ȼ����� ����� ���� mR�� ����! 
			// 		else 
			// 			t1 = (mL + mR) / 2 + 0.5;							//�ݿø� ���ֱ�!!!     +0.5


			/*---------------------------------------------------------------------------------------------------------------------------------------*/
			//3. | t0-t1 | <= 1�̸� stop, T1���� return 
			// 		if (t1 != 0) {											//���� t0�� ���� �ް��� �ι�° while������ t1�� ���� �ް��� üũ�ϵ��� 	
			// 			int val = t0 - t1;									//t0-t1�� �� ���ϰ� 
			// 			val = (val) >= 0 ? val : -val;						//���밪 �����ְ�~
			// 				
			// 			if (val <= 1) {										//���̰� 1���� �۰ų� ������
			// 				*pnThreshold = t1;								//Threshold���� t1������ ��ȯ���ְ� 
			// 				break;											//while���� ���������ش�
			// 			}
			// 			else {												//���̰� 1���� ũ�ٸ� 
			// 				t0 = t1;										//threshold���� t1������ �ٲ��ְ� �ٽ� while�ݺ� �����Ѵ�.
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
			chisto[x] = chisto[x - 1] + histo[x];						//����...���ϱ� cumulative histogram
			cxhisto[x] = cxhisto[x - 1] + (x*histo[x]);					//x*histo[x]�̸� ���صα� 
																		//mT += (double)(x*histo[x]);
		}
		mT = (double)cxhisto[65535];										//mT ��ü x* histo �� 
		t0 = (int)(mT / (double)size + 0.5);							//t0 ���� T�� ������� 


		while (1)
		{
			mL = cxhisto[t0];
			mR = mT - mL;

			if (chisto[t0] > 0)	mL /= (double)chisto[t0];
			else mL = 0;
			if (chisto[t0] != size)	mR /= (double)size - chisto[t0];
			else mR = 0;
			t1 = (int)((mL + mR + 0.5) / 2);

			if (abs(t0 - t1) <= 1) break;	//t0-t1�� ���� 1���� �۰ų� ���� ���̱� ������ 8��Ʈ���� ������ ��갡��. 8��Ʈó���Ϸ��� 256���� �ϸ� �� 
			else t0 = t1;
		}
		break;

	default:
		return false;
	}

	*pnThreshold = t1;											//������ ����!
	delete[] cxhisto;											//�Ҵ�� �ݴ��� ������ delete���ִ°� pc���� ���ٰ� ��-�ֱ�����-
	delete[] chisto;											//�Ҵ�� �ݴ��� ������ delete���ִ°� pc���� ���ٰ� ��-�ֱ�����-
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

	int t0 = -1, t0_min = 0, t0_max = 0;										//���� t0��
	int t1, t2;
	double mL, mR, mT=0;
	double p1 = 0.0, p2 = 0.0;									//Probability Ȯ�� 
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
		if (histo[0] != 0 && t0 == -1)	t0 = histo[0];				//OTSU 1. t0 �� histo[]�� 0�� �ƴ� ������ ����
		for (int x = 1; x < 256; x++) {
			chisto[x] = chisto[x - 1] + histo[x];					//chisto ���ϱ� cumulative histogram
			cxhisto[x] = cxhisto[x - 1] + (x*histo[x]);				//cshisto(x*histo[x])�̸� ���صα� 
																	//mT += (double)(x*histo[x]);
			if (histo[x] != 0 && t0 == -1)	t0 = x;					//OTSU 1. t0 �� histo[]�� 0�� �ƴ� ������ ����
		}
		mT = (double)cxhisto[255] / size;							//��ü ���


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

			mL = cxhisto[t0];	//0 ~ t		������ ��
			mR = cxhisto[255] - mL;		//t ~ L-1	������ �� 

			if (chisto[t0] > 0)	mL /= (double)chisto[t0];			//0 ~ t �� ���� �ϳ��� ������ mL��� ���ϱ�
			else mL = 0;											//������ ���� �����̴� 0
			if (chisto[t0] != size)	mR /= (double)size - chisto[t0];//0 ~ t���� ��� �ȼ��� ���� �ʴٸ�. mR��� �����ش�.
			else mR = 0;											//0 ~ t���� ��� �ȼ��� �ִٸ�. 255

			p1 = (double)((double)chisto[t0] / (double)size);		//mL �� ����?Ȯ��? �����ֱ� 
																	//�ӵ� ���̱� 2 p2 -> 1-p1
																	//p2 = 1.0 - p1;											//mR �� ����?Ȯ��? �����ֱ�  p1+p2=1


																	// 		double temp1 = (pow((mL - mT), 2));
																	// 		double temp2 = (pow((mR - mT), 2));

																	//P = p1*(mL-mT)*(mL-mT) + p2*(mR-mT)*(mR-mT);		//�׳� �� 
																	//�ӵ� ���̱� 1 ���� ����ȭ 
			P = p1*(1. - p1)*(mL - mR)*(mL - mR);	//����ȭ �� 

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
		if (histo[0] != 0 && t0 == -1)	t0 = histo[0];				//OTSU 1. t0 �� histo[]�� 0�� �ƴ� ������ ����
		for (int x = 1; x < 65536; x++) {
			chisto[x] = chisto[x - 1] + histo[x];					//chisto ���ϱ� cumulative histogram
			cxhisto[x] = cxhisto[x - 1] + (x*histo[x]);				//cshisto(x*histo[x])�̸� ���صα� 
																	//mT += (double)(x*histo[x]);
			if (histo[x] != 0 && t0 == -1)	t0 = x;					//OTSU 1. t0 �� histo[]�� 0�� �ƴ� ������ ����
		}
		mT = (double)cxhisto[65535] / size;							//��ü ���


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

			mL = cxhisto[t0];	//0 ~ t		������ ��
			mR = cxhisto[65535] - mL;		//t ~ L-1	������ �� 

			if (chisto[t0] > 0)	mL /= (double)chisto[t0];			//0 ~ t �� ���� �ϳ��� ������ mL��� ���ϱ�
			else mL = 0;											//������ ���� �����̴� 0
			if (chisto[t0] != size)	mR /= (double)size - chisto[t0];//0 ~ t���� ��� �ȼ��� ���� �ʴٸ�. mR��� �����ش�.
			else mR = 0;											//0 ~ t���� ��� �ȼ��� �ִٸ�. 255

			p1 = (double)((double)chisto[t0] / (double)size);		//mL �� ����?Ȯ��? �����ֱ� 
																	//�ӵ� ���̱� 2 p2 -> 1-p1
																	//p2 = 1.0 - p1;											//mR �� ����?Ȯ��? �����ֱ�  p1+p2=1


																	// 		double temp1 = (pow((mL - mT), 2));
																	// 		double temp2 = (pow((mR - mT), 2));

																	//P = p1*(mL-mT)*(mL-mT) + p2*(mR-mT)*(mR-mT);		//�׳� �� 
																	//�ӵ� ���̱� 1 ���� ����ȭ 
			P = p1*(1. - p1)*(mL - mR)*(mL - mR);	//����ȭ �� 

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
		*pnThreshold = t0_min;									//������ ����!
	delete[] cxhisto;											//�Ҵ�� �ݴ��� ������ delete���ִ°� pc���� ���ٰ� ��-�ֱ�����-
	delete[] chisto;											//�Ҵ�� �ݴ��� ������ delete���ִ°� pc���� ���ٰ� ��-�ֱ�����-
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

	unsigned int *histo, *chisto;								//cxhisto �ʿ����.
	double *plogp;												//p*log(p)���а� ���صα�

	int t = -1, t_min = 0, t1 = 0, t2 = 0;						//���� t0��
	unsigned int n1 = 0, n2 = 0;
	double H1, H2;												//Probability Ȯ�� 
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

		//chisto ���ϱ� cumulative histogram
		chisto[0] = histo[0];
		for (int x = 1; x < 256; x++) {
			chisto[x] = chisto[x - 1] + histo[x];					
		}

		//>>chisto�� 0�� �ƴѰ��� ã�� 0~���ʹ� t1, 255~�� t2�� ����
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

		//plogp ���� �̸� ���صα�
		plogp[t1] = (double)histo[t1] * log((double)histo[t1]);
		for (t = t1 + 1; t <= t2; t++) {
			if (histo[t] > 0)
				plogp[t] = plogp[t - 1] + ((double)histo[t] * log((double)histo[t]));	//log(0) -> error
				//plogp[t] = ((double)histo[t - 1] * log((double)histo[t - 1])) + ((double)histo[t] * log((double)histo[t]));	���� �̰� �̻��ؿ� t-1�� 0�ϼ������ڳ� 
			else
				plogp[t] = plogp[t - 1];
		}

		for (t = t1; t < t2; t++)	//������ t2���� ���� ����̴ϱ�~~~~
		{
			if (chisto[t] == chisto[255]) break;							//�����ʿ� �ƹ��͵� ����
			n1 = chisto[t];
			n2 = chisto[255] - n1;

			H1 = 0.;
			H1 = log((double)n1) - ((double)plogp[t] / (double)n1);		
// 			for (int i = t1; i <= t; i++) {	//�ñ׸� 
// 				if (histo[i] > 0) {							//log(0)�� ������� ����. histo[i]�� 0�̸� pi�� 0�̴ϱ�...
// 					Pi = (double)histo[i] / (double)n1;
// 					H1 += -Pi*log(Pi);						//log(0)�� -���Ѵ�ϱ�...
// 				}
// 			}

			H2 = 0.;
			H2 = log((double)n2) - (((double)plogp[t2] - (double)plogp[t]) / (double)n2);
// 			for (int i = t+1; i <= t2; i++) {	//�ñ׸� 
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

		//chisto ���ϱ� cumulative histogram
		chisto[0] = histo[0];
		for (int x = 1; x < 65536; x++) {
			chisto[x] = chisto[x - 1] + histo[x];
		}

		//>>chisto�� 0�� �ƴѰ��� ã�� 0~���ʹ� t1, 255~�� t2�� ����
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

		//plogp ���� �̸� ���صα�
		plogp[t1] = (double)histo[t1] * log((double)histo[t1]);
		for (t = t1 + 1; t <= t2; t++) {
			if (histo[t] > 0)
				plogp[t] = plogp[t - 1] + ((double)histo[t] * log((double)histo[t]));	//log(0) -> error
			else
				plogp[t] = plogp[t - 1];
		}

		for (t = t1; t < t2; t++)	//������ t2���� ���� ����̴ϱ�~~~~
		{
			if (chisto[t] == chisto[65535]) break;							//�����ʿ� �ƹ��͵� ����
			n1 = chisto[t];
			n2 = chisto[65535] - n1;

			H1 = 0.;
			H1 = log((double)n1) - ((double)plogp[t] / (double)n1);
			// 			for (int i = t1; i <= t; i++) {	//�ñ׸� 
			// 				if (histo[i] > 0) {							//log(0)�� ������� ����. histo[i]�� 0�̸� pi�� 0�̴ϱ�...
			// 					Pi = (double)histo[i] / (double)n1;
			// 					H1 += -Pi*log(Pi);						//log(0)�� -���Ѵ�ϱ�...
			// 				}
			// 			}

			H2 = 0.;
			H2 = log((double)n2) - (((double)plogp[t2] - (double)plogp[t]) / (double)n2);
			// 			for (int i = t+1; i <= t2; i++) {	//�ñ׸� 
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


	*pnThreshold = t_min;									//������ ����!
	delete[] plogp;
	delete[] chisto;										//�Ҵ�� �ݴ��� ������ delete���ִ°� pc���� ���ٰ� ��-�ֱ�����-
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