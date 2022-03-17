#include "stdafx.h"
#include "MyIPMorphology.h"
#include "MyIPSpetialFilter.h"
#include "MyCSVFile.h"		//190501 CSV TEST


bool IpDilation(MyIPImage & img, CShapeElement &se)
{
	if (!img.GetPixels())	return false;
	if (!se.GetPoint())		return false;

	int depth = img.GetDepth();
	int channels = img.GetChannels();
	int height = img.GetHeight();
	int width = img.GetWidth();

	if (depth != CV_8U || channels != 1)	return false;

	CPoint* pt = se.GetPoint();									//array for CPoint Class Variable
	int nse = se.GetSize();										//element size
	int kh = 0, kw = 0, kh2, kw2;								//height, width of structure element
	for (int k = 0; k < nse; k++) {								//
		if (abs(pt[k].x) > kw)	kw = (abs(pt[k].x));			//get largest x
		if (abs(pt[k].y) > kh)	kh = (abs(pt[k].y));			// "	"	  y
	}
	kh = 2 * kh + 1;											//element height
	kw = 2 * kw + 1;											//element width 
	kh2 = kh / 2;
	kw2 = kw / 2;


	MyIPImage timg;	
	timg.Copy(img);

	MyIPImage borderimg;										//border image, constant zero
	MyIPImage borderimg2;										//border image, constant 128
	if (!IpFillBorder(img, borderimg, kh, kw, BORDER_REPLICATE))	return false;
	if (!IpFillBorder(img, borderimg2, kh, kw, BORDER_CONSTANT,128))	return false;

	byte **pixel = (byte **)img.GetPixels();					//array for output pixel 
	byte **tpixel = (byte **)timg.GetPixels();					//array for input pixel without border
	byte **bpixel = (byte **)borderimg.GetPixels();				//array for input pixel with border
	byte **bpixel2 = (byte **)borderimg2.GetPixels();			//array for input pixel with border 128

	int newH = height + 2 * kh2;								//border image height			
	int newW = width + 2 * kw2;									//border image width
	int x, y, k, nx, ny;										//variable for loop(x,y,k) & new xy

	int SizeA = 0;
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (pixel[y][x])
				SizeA++;
		}
	}
// 	FILE *fp = fopen("test.csv", "a+");
// 	fprintf(fp, "%d, %d, ", SizeA, nse);
// 	double t1, t2, t3, t4;
	
/*-------------------------------Dilation Method 1. Border-------------------------------- */
	for (y = 0; y < height; y++)
		memset(pixel[y], 0, width * sizeof(byte));
	TIMECHECK_START
	for (y = kh2; y < newH - kh2; y++) {						//for loop(image height)
		ny = y - kh2;
		for (x = kw2; x < newW - kw2; x++) {					//for loop(image width)
			nx = x - kw2;
			for (k = 0; k < nse; k++) {							//for loop(structure element)
				if (bpixel[y + pt[k].y][x + pt[k].x]) {			//if any element in S.E is 255
					pixel[ny][nx] = (byte)255;					//set all pixel to 255
					break;										//then break
				}
			}
		}
	}
	TIMECHECK_END
//	AfxNewImage(_T("1(2)-border"), img);
//	t1 = (double)(endtime - s) / CLOCKS_PER_SEC;

	/*-------------------------------Dilation Method 1. Without Border------------------------ */
// 	for (y = 0; y < height; y++)
// 		memset(pixel[y], 0, width * sizeof(byte));
// 	TIMECHECK_START_AGAIN
// 	//2번	{z | B^z ∩A≠0}	//B(hat) z	ALWAYS BREAK
// 	for (y = 0; y < height; y++) {
// 		for (x = 0; x < width; x++) {							// z ∈ Image(y,x)
// 			for (k = 0; k < nse; k++) {
// 				ny = y - pt[k].y;
// 				if ((ny >= 0) && (ny < height)) {
// 					nx = x - pt[k].x;
// 					if ((nx >= 0) && (nx < width)) {
// 						if (tpixel[ny][nx]) {					// hat(B)z ∩ A ≠ O
// 							pixel[y][x] = (byte)255;
// 							break;
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}
// 	TIMECHECK_END
// 	//AfxNewImage(_T("1(2)- no border"), img);
// 	t2 = (double)(endtime - s) / CLOCKS_PER_SEC;


	/*-------------------------------Dilation Method 2. Border-------------------------------- */
// 	for (y = 0; y < height; y++)
// 		memset(pixel[y], 0, width * sizeof(byte));
// 	TIMECHECK_START_AGAIN
// 	for (y = kh2; y < newH - kh2; y++) {						//for loop(image height)
// 		for (x = kw2; x < newW - kw2; x++) {					//for loop(image width)
// 			if (bpixel2[y][x] == 255) {							//if a pixel is 255
// 				for (k = 0; k < nse; k++) {
// 					ny = y - kh2 + pt[k].y;
// 					nx = x - kw2 + pt[k].x;
// 					if (bpixel2[y + pt[k].y][x + pt[k].x] != 128) {	//when its not border pixels
// 						if (pixel[ny][nx] == 255)				//make S.E surrounding pixels to 255
// 							continue;
// 						else
// 							pixel[ny][nx] = 255;
// 					}
// 				}
// 			}
// 		}
// 	}
// 	TIMECHECK_END
// 	//AfxNewImage(_T("2(3)-border"), img);
// 	t3 = (double)(endtime - s) / CLOCKS_PER_SEC;


	/*-------------------------------Dilation Method 2. Without Border------------------------ */
// 	for (y = 0; y < height; y++)
// 		memset(pixel[y], 0, width * sizeof(byte));
// 	TIMECHECK_START_AGAIN
// 	//3번	 ∪	Ba							BREAK
// 	//		a∈A
// 	for (y = 0; y < height; y++) {
// 		for (x = 0; x < width; x++) {
// 			if (tpixel[y][x]) {									//a∈A
// 				for (k = 0; k < nse; k++) {						//∪Ba
// 					ny = y + pt[k].y;
// 					if (ny >= 0 && ny < height) {
// 						nx = x + pt[k].x;
// 						if (nx >= 0 && nx < width) {
// 							if (pixel[ny][nx] == 255)			//with break
// 								continue;
// 							else
// 								pixel[ny][nx] = (byte)255;		
// 
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}
// 	TIMECHECK_END
// 	//AfxNewImage(_T("2(3)- no border"), img);
// 	t4 = (double)(endtime - s) / CLOCKS_PER_SEC;


/*------------------------------------	other method	---------------------------------- */

	//	-------------1번   {z | z=a+b, for a∈A, b∈B}		BREAK-------------------
// 	for (y = 0; y < height; y++)
// 		memset(pixel[y], 0, width * sizeof(byte));
// 	for (k = 0; k < nse; k++) {								//b∈B
// 		for (y = 0; y < height; y++) {						//a∈A
// 			for (x = 0; x < width; x++) {
// 				if (tpixel[y][x]) {							//z=a+b
// 					ny = y + pt[k].y;
// 					if (ny >= 0 && ny < height) {
// 						nx = x + pt[k].x;
// 						if (nx >= 0 && nx < width) {
// 							if (pixel[ny][nx] == 255)		//with break
// 								continue;
// 							else
// 								pixel[ny][nx] = 255;
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}


	//	-----------1번   {z | z=a+b, for a∈A, b∈B}		NO BREAK----------------
// 	for (y = 0; y < height; y++)
// 		memset(pixel[y], 0, width * sizeof(byte));
	
// 		
// 	for (k = 0; k < nse; k++) {								//b∈B
// 		for (y = 0; y < height; y++) {						//a∈A
// 			for (x = 0; x < width; x++) {					//
// 				if (tpixel[y][x]) {							//	
// 					ny = y + pt[k].y;						//z=a+b			
// 					if (ny >= 0 && ny < height) {
// 						nx = x + pt[k].x;
// 						if (nx >= 0 && nx < width) {
// 							pixel[ny][nx] = 255;			//without break
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}

	// 	----------3번	 ∪	Ba	----------------------	NO BREAK--------------------
// 	a∈A
// 	for (y = 0; y < height; y++)
// 		memset(pixel[y], 0, width * sizeof(byte));
// 	for (y = 0; y < height; y++) {							//a∈A
// 		for (x = 0; x < width; x++) {						//
// 			if (tpixel[y][x]) {								//
// 				for (k = 0; k < nse; k++) {					//∪Ba
// 					ny = y + pt[k].y;						//			
// 					if (ny >= 0 && ny < height) {			//
// 						nx = x + pt[k].x;					//
// 						if (nx >= 0 && nx < width) {		//
// 							pixel[ny][nx] = 255;			//without break
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}

//	fprintf(fp, "%04.3f, %04.3f, %04.3f, %04.3f,\n",t1,t2,t3,t4);
//	fclose(fp);
	return true;
}

bool IpErotion(MyIPImage & img, CShapeElement & se)
{
	if (!img.GetPixels())	return false;						//exception handling
	if (!se.GetPoint())		return false;						//

	int depth = img.GetDepth();									//get image info
	int channels = img.GetChannels();							//
	int height = img.GetHeight();								//
	int width = img.GetWidth();									//

	if (depth != CV_8U || channels != 1)	return false;		//only for 8bit 1ch image 

	CPoint* pt = se.GetPoint();									//array for CPoint Class Variable
	int nse = se.GetSize();										//element size
	int kh = 0, kw = 0, kh2, kw2;								//height, width of structure element
	for (int k = 0; k < nse; k++) {								//
		if (abs(pt[k].x) > kw)	kw = (abs(pt[k].x));			//get largest x
		if (abs(pt[k].y) > kh)	kh = (abs(pt[k].y));			// "	"	  y
	}
	kh = 2 * kh + 1;											//element height
	kw = 2 * kw + 1;											//element width 
	kh2 = kh / 2;
	kw2 = kw / 2;

	MyIPImage timg;												//reference image 
	timg.Copy(img);												//

	MyIPImage borderimg;										//border image, constant zero
	MyIPImage borderimg2;										//border image, constant 128
	if (!IpFillBorder(img, borderimg, kh, kw, BORDER_REPLICATE))	return false;
	if (!IpFillBorder(img, borderimg2, kh, kw, BORDER_CONSTANT, 128))	return false;
	//if (!IpFillBorder(img, borderimg, kh, kw, BORDER_CONSTANT, (int)&morphologyDefaultBorderValue()))	return false;

	byte **pixel = (byte **)img.GetPixels();					//array for output pixel 
	byte **tpixel = (byte **)timg.GetPixels();					//array for input pixel without border
	byte **bpixel = (byte **)borderimg.GetPixels();				//array for input pixel with border
	byte **bpixel2 = (byte **)borderimg2.GetPixels();				//array for input pixel with border

	int newH = height + 2 * kh2;								//border image height			
	int newW = width + 2 * kw2;									//border image width
	int x, y, k, nx, ny;										//variable for loop(x,y,k) & new xy

	int SizeA = 0;
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (pixel[y][x])
				SizeA++;
		}
	}
// 	FILE *fp = fopen("test.csv", "a+");
// 	fprintf(fp, "%d, %d, ", SizeA, nse);
// 	double t1, t2, t3, t4;

/*-------------------------------Erosion Method 1. Border---------------------------------------- */
	for (y = 0; y < height; y++)
		memset(pixel[y], 255, width * sizeof(byte));			//set output pixel to 255 (erosion)
	TIMECHECK_START
	//1번			with border
	//{z| Bz ∈ A}		
	for (y = kh2; y < newH - kh2; y++) {						//for loop(image height)
		ny = y - kh2;
		for (x = kw2; x < newW - kw2; x++) {					//for loop(image width)
			nx = x - kw2;
			for (k = 0; k < nse; k++) {							//for loop(structure element)
				if (bpixel[y + pt[k].y][x + pt[k].x] == 0) {	//if all element is 255, set pixel to 255
					pixel[ny][nx] = (byte)0;					//if not, set to zero
					break;										//then break
				}
			}
		}
	}
	TIMECHECK_END
//	AfxNewImage(_T("1 - border"), img);
//	t1 = (double)(endtime - s) / CLOCKS_PER_SEC;


	/*-------------------------------Erosion Method 1. Without Border-------------------------------- */
// 	for (y = 0; y < height; y++)
// 		memset(pixel[y], 255, width * sizeof(byte));			//set output pixel to 255 (erosion)
// 	TIMECHECK_START_AGAIN
// 	//1번			without border
// 	//{z| Bz ∈ A}		
// 	for (y = 0; y < height; y++) {								//for loop(image height)
// 		for (x = 0; x < width; x++) {							//for loop(image width)
// 			for (k = 0; k < nse; k++) {							//for loop(structure element)
// 				ny = y + pt[k].y;								//exception handle for ny,nx range	(can skip)
// 				if (ny >= 0 && ny < height) {					//									(can skip)
// 					nx = x + pt[k].x;							//									(can skip)
// 					if (nx >= 0 && nx < width) {				//									(can skip)
// 						if (tpixel[ny][nx] == 0) {				//if any element in S.E is 0,
// 							pixel[y][x] = (byte)0;				//set elements to 0
// 							break;								//then break
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}
// 	TIMECHECK_END
// 	//AfxNewImage(_T("1 - no border"), img);
// 	t2 = (double)(endtime - s) / CLOCKS_PER_SEC;


	/*-------------------------------Erosion Method 2. Border---------------------------------------- */
// 	for (y = 0; y < height; y++)
// 		memset(pixel[y], 255, width * sizeof(byte));			//set output pixel to 255 (erosion)
// 	TIMECHECK_START_AGAIN
// 	//2번			with border
// 	//A (-) B  = complement{complement(A) (+) hat(B)}
// 	for (y = kh2; y < newH - kh2; y++) {						//for loop(image height)
// 		for (x = kw2; x < newW - kw2; x++) {					//for loop(image width)
// 			if (bpixel2[y][x] == 0) {							//if a pixel is 0
// 				for (k = 0; k < nse; k++) {
// 					ny = y - kh2 + pt[k].y;
// 					nx = x - kw2 + pt[k].x;
// 					if (bpixel2[y + pt[k].y][x + pt[k].x] != 128) {	//when its not border pixels
// 						if (pixel[ny][nx] == 0)					//make S.E surrounding pixels to 0
// 							continue;
// 						else
// 							pixel[ny][nx] = 0;
// 					}
// 				}
// 			}
// 		}
// 	}
// 	TIMECHECK_END
// 	//AfxNewImage(_T("2 - border"), img);
// 	t3 = (double)(endtime - s) / CLOCKS_PER_SEC;


	/*-------------------------------Erosion Method 2. Without Border-------------------------------- */
// 	for (y = 0; y < height; y++)
// 		memset(pixel[y], 255, width * sizeof(byte));			//set output pixel to 255 (erosion)
// 	TIMECHECK_START_AGAIN
// 	//2번			without border
// 	//A (-) B  = complement{complement(A) (+) hat(B)}
// 	for (y = 0; y < height; y++) {								//for loop(image height)
// 		for (x = 0; x < width; x++) {							//for loop(image width)
// 			if (tpixel[y][x] == 0) {							// (1)-> complement(A) + (2) = (3)
// 				for (k = 0; k < nse; k++) {						//for loop(structuring element)
// 					ny = y - pt[k].y;							// (2)-> hat(B)
// 					if (ny >= 0 && ny < height) {				//
// 						nx = x - pt[k].x;						//
// 						if (nx >= 0 && nx < width) {			//
// 							if (pixel[ny][nx] == 0)				// (3)-> complement(
// 								continue;						//	
// 							else								//
// 								pixel[ny][nx] = 0;				//
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}
// 	TIMECHECK_END
// 	//AfxNewImage(_T("2 - no border"), img);
// 	t4 = (double)(endtime - s) / CLOCKS_PER_SEC;

//	fprintf(fp, ",,,,%04.3f, %04.3f, %04.3f, %04.3f,\n", t1, t2, t3, t4);
//	fclose(fp);
	return true;
}

bool IpConditional(MyIPImage & img, MyIPImage &ref)
{
	if (!img.GetPixels() || !ref.GetPixels())		return false;		//exception handling
	if (img.GetDepth() != ref.GetDepth())			return false;
	if (img.GetChannels() != ref.GetChannels())		return false;
	if (img.GetHeight() != ref.GetHeight())			return false;
	if (img.GetWidth() != ref.GetWidth())			return false;		

	int depth = img.GetDepth();									//get image info
	int channels = img.GetChannels();							//
	int height = img.GetHeight();								//
	int width = img.GetWidth();									//

	if (depth != CV_8U || channels != 1)	return false;		//only for 8bit 1ch image 

	vector<CPoint> strong_edges;								//Vector of Strong Edge coordinates
	strong_edges.reserve(width*height);

	byte **pixel = (byte**)img.GetPixels();						//Get Image Pixel
	byte **rpixel = (byte**)ref.GetPixels();					//Get Reference Image Pixel
	int x, y;													//variable for loop
	TIMECHECK_START
	//1. Set Strong Edge
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (pixel[y][x] && rpixel[y][x]) {					//if pixel exists (AND연산하고 들어가자~) 
				strong_edges.push_back(CPoint(x, y));			//set pixel to strong edge
			}
			else
				pixel[y][x] = (byte)0;
		}
	}
	TIMECHECK_END

	TIMECHECK_START_AGAIN
	//2. Trace 
	CPoint pt;
	while (!strong_edges.empty()) {
		pt = strong_edges.back();
		strong_edges.pop_back();

		for (y = pt.y - 1; y <= pt.y + 1; y++) {				//strong edge 주위 픽셀에
			for (x = pt.x - 1; x <= pt.x + 1; x++) {
				if (y < 0 || y >= height || x < 0 || x >= width)
					continue;
				if (rpixel[y][x]) {								//reference image에 strong edge와 만나는 week edge가 있으면 
					rpixel[y][x] = (byte)0;						//너 이제 week edge 아니야
					if (!pixel[y][x]) pixel[y][x] = (byte)255;	//너도 Output에 표시될거야
					strong_edges.push_back(CPoint(x, y));		//그리고 너도 Strong Edge가 되렴~
				}
			}
		}
	}

	TIMECHECK_END

	return true;
}

bool IpGrayDilation(MyIPImage & img, CShapeElement & se)
{
	if (!img.GetPixels())	return false;
	if (!se.GetPoint())		return false;

	int depth = img.GetDepth();
	int channels = img.GetChannels();
	int height = img.GetHeight();
	int width = img.GetWidth();

	if (depth != CV_8U && depth != CV_16U)	return false;
	if (channels != 1)						return false;

	CPoint* pt = se.GetPoint();										//array for CPoint Class Variable
	int nse = se.GetSize();											//element size
	int kh = 0, kw = 0, kh2, kw2;									//height, width of structure element
	for (int k = 0; k < nse; k++) {									//
		if (abs(pt[k].x) > kw)	kw = (abs(pt[k].x));				//get largest x
		if (abs(pt[k].y) > kh)	kh = (abs(pt[k].y));				// "	"	  y
	}
	kh = 2 * kh + 1;												//element height
	kw = 2 * kw + 1;												//element width 
	kh2 = kh / 2;
	kw2 = kw / 2;

	MyIPImage borderimg;											//border image, constant zero
	if (!IpFillBorder(img, borderimg, kh, kw, BORDER_REPLICATE))	return false;

	if (depth == CV_8U) {
		byte **pixel = (byte **)img.GetPixels();					//array for output pixel 
		byte **bpixel = (byte **)borderimg.GetPixels();				//array for input pixel with border

		int newH = height + 2 * kh2;								//border image height			
		int newW = width + 2 * kw2;									//border image width
		int x, y, k, nx, ny, px, py;								//variable for loop(x,y,k) & new xy
		int bMax;

		for (y = 0; y < height; y++)
			memset(pixel[y], 0, width * sizeof(byte));
		TIMECHECK_START
			for (y = kh2; y < newH - kh2; y++) {						
				ny = y - kh2;
				for (x = kw2; x < newW - kw2; x++) {					
					nx = x - kw2;
					bMax = 0;
					for (k = 0; k < nse; k++) {		
						py = y - pt[k].y;
						px = x - pt[k].x;
						if (bMax < bpixel[py][px]) {		//max{ f(x-s, y-t) + b(s,t)}	
							bMax = bpixel[py][px];								
						}
					}
					pixel[ny][nx] = (byte)bMax;
				}
			}
		TIMECHECK_END
	}
	else if (depth == CV_16U) {

	}

	return true;
}

bool IpGrayErotion(MyIPImage & img, CShapeElement & se)
{
	if (!img.GetPixels())	return false;
	if (!se.GetPoint())		return false;

	int depth = img.GetDepth();
	int channels = img.GetChannels();
	int height = img.GetHeight();
	int width = img.GetWidth();

	if (depth != CV_8U && depth != CV_16U)	return false;
	if (channels != 1)						return false;

	CPoint* pt = se.GetPoint();										//array for CPoint Class Variable
	int nse = se.GetSize();											//element size
	int kh = 0, kw = 0, kh2, kw2;									//height, width of structure element
	for (int k = 0; k < nse; k++) {									//
		if (abs(pt[k].x) > kw)	kw = (abs(pt[k].x));				//get largest x
		if (abs(pt[k].y) > kh)	kh = (abs(pt[k].y));				// "	"	  y
	}
	kh = 2 * kh + 1;												//element height
	kw = 2 * kw + 1;												//element width 
	kh2 = kh / 2;
	kw2 = kw / 2;

	MyIPImage borderimg;											//border image, constant zero
	if (!IpFillBorder(img, borderimg, kh, kw, BORDER_REPLICATE))	return false;

	if (depth == CV_8U) {
		byte **pixel = (byte **)img.GetPixels();					//array for output pixel 
		byte **bpixel = (byte **)borderimg.GetPixels();				//array for input pixel with border

		int newH = height + 2 * kh2;								//border image height			
		int newW = width + 2 * kw2;									//border image width
		int x, y, k, nx, ny, px, py;								//variable for loop(x,y,k) & new xy
		int bMin;

		for (y = 0; y < height; y++)
			memset(pixel[y], 0, width * sizeof(byte));
		TIMECHECK_START
			for (y = kh2; y < newH - kh2; y++) {
				ny = y - kh2;
				for (x = kw2; x < newW - kw2; x++) {
					nx = x - kw2;
					bMin = 255;
					for (k = 0; k < nse; k++) {
						py = y + pt[k].y;
						px = x + pt[k].x;
						if (bMin > bpixel[py][px]) {		//max{ f(x-s, y-t) + b(s,t)}	b(s,t) 생략..
							bMin = bpixel[py][px];
						}
					}
					pixel[ny][nx] = (byte)bMin;
				}
			}
		TIMECHECK_END
	}
	else if (depth == CV_16U) {

	}

	return true;
}

