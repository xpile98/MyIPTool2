#include "stdafx.h"
#include "MyIPObjectDetection.h"

bool IpPatternMatching(Mat & img, Mat & pattern, Mat & out, int nObjects, float fNCCThreshold, int match_method)
{
	//1. exception finding 
	if (!img.data || !pattern.data)							return false;	//img, pattern이 모두 정상이 아니면 
	if (img.type() != pattern.type())						return false;	//두 객체의 타입이 다르다면
	if (img.rows < pattern.rows || img.cols < pattern.cols)	return false;	//이미지보다 패턴이 크면 
	if (nObjects < 1)										return false;	//찾고자 하는 객체가 0보다 작으면 


	//2. make result image 
	int newH = img.rows + pattern.rows - 1;						//out image Height
	int newW = img.cols + pattern.cols - 1;						//out image Width
	out.create(newH, newW, CV_32FC1);							//create out image (cv_32fc1)


	//3. Template Matching (TM_CCOEFF_NORMED)
	//matchTemplate(img, pattern, out, TM_CCOEFF_NORMED);			//template matching (TM_CCOEFF_NORMED)
	matchTemplate(img, pattern, out, TM_CCORR_NORMED);

	//4. Normalize <SKIP>
	//out = out * fNCCThreshold;								//알아서 normalized 되어있음 1
	//normalize(out, out, 0, 1, NORM_MINMAX, -1, Mat());		//알아서 normalized 되어있음 2

	//5. Finding Max, Min value 
	Point *arr = new Point[nObjects];
	Mat out_tmp; out.copyTo(out_tmp);
	Mat mat_roi;
	Size ignore_size(pattern.cols, pattern.rows);
	Rect roi;
	

	double minVal; double maxVal; Point minLoc; Point maxLoc; Point matchLoc;
	for (int n = 0; n < nObjects; n++) {
		minMaxLoc(out_tmp, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
		if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED) 	//square difference = minimum value  
			matchLoc = minLoc;
		else 
			matchLoc = maxLoc;
		
		roi = Rect(matchLoc - Point(ignore_size.width / 2, ignore_size.height / 2), ignore_size);
		if (roi.x < 0) {								//영상 오른쪽으로 오버 
			roi.width += roi.x;
			roi.x = 0;
		}
		if (roi.y < 0) {								//영상 위쪽으로 오버 
			roi.height += roi.y;
			roi.y = 0;
		}
		if (roi.x + roi.width > out_tmp.cols - 1) {		//영상 왼쪽으로 오버 
			int over_x = (roi.x + roi.width + 1) - out_tmp.cols-1;
			roi.width -= over_x;
		}
		if (roi.y + roi.height > out_tmp.rows - 1) {	//영상 오른쪽으로 오버 
			int over_y = (roi.y + roi.height + 1) - out_tmp.rows-1;
			roi.height -= over_y;
		}

		mat_roi = out_tmp(roi);

		if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED) 	//square difference = minimum value  
			mat_roi.setTo(255.f);
		else
			mat_roi.setTo(0.f);

		arr[n] = matchLoc;
	}

	//6. Print Info
	for (int n = 0; n < nObjects; n++) {
		AfxPrintInfo(_T("%d: (%d, %d): %f"), n, arr[n].x, arr[n].y, out.at<float>(arr[n].y, arr[n].x));	//cv32f 이라서 double Template 사용시 오류 ... 황당하군
		printf("%d\t: (%d, %d)\t : %.3f\n", n, arr[n].x, arr[n].y, out.at<float>(arr[n].y, arr[n].x));
	}

	//7. Display Image 
	if (img.channels() == 1) {
		cvtColor(img, img, COLOR_GRAY2BGR);
		cvtColor(out, out, COLOR_GRAY2BGR);
		//cvtColor(out_tmp, out_tmp, COLOR_GRAY2BGR);
	}
	for (int n = 0; n < nObjects; n++) {
		roi.x = arr[n].x;
		roi.y = arr[n].y;
		roi.width = pattern.cols;
		roi.height = pattern.rows;
// 		Mat roi_save = img(roi);
// 		cvtColor(roi_save, roi_save, COLOR_BGR2GRAY);
// 		CString strFileName;
// 		strFileName.Format(_T("%d_(%d, %d)"), n, roi.x, roi.y);
// 		AfxNewImage(strFileName, roi_save);
		//rectangle(img, arr[n], Point(arr[n].x + pattern.cols, arr[n].y + pattern.rows), Scalar(255, 255, 0), 30);
		rectangle(img, roi, Scalar(255, 255, 0), 1);
		circle(out, arr[n], 10, Scalar(0, 0, 255), 2);
	}
	out.convertTo(out, CV_8U, 255); // [0..1] -> [0..255] range
// 	out_tmp.convertTo(out_tmp, CV_8U, 255); // [0..1] -> [0..255] range
// 	AfxNewImage(out_tmp);




	//https://docs.opencv.org/4.0.0/d8/ded/samples_2cpp_2tutorial_code_2Histograms_Matching_2MatchTemplate_Demo_8cpp-example.html#a16
	return true;
}


bool IpPatternMatching_Thresold(Mat & img, Mat & pattern, Mat & out, float fNCCThreshold, int match_method)
{
	//1. exception finding 
	if (!img.data || !pattern.data)							return false;	//img, pattern이 모두 정상이 아니면 
	if (img.type() != pattern.type())						return false;	//두 객체의 타입이 다르다면
	if (img.rows < pattern.rows || img.cols < pattern.cols)	return false;	//이미지보다 패턴이 크면


	//2. make result image 
	int newH = img.rows + pattern.rows - 1;						//out image Height
	int newW = img.cols + pattern.cols - 1;						//out image Width
	out.create(newH, newW, CV_32FC1);							//create out image (cv_32fc1)


																//3. Template Matching (TM_CCOEFF_NORMED)
	matchTemplate(img, pattern, out, TM_CCOEFF_NORMED);			//template matching (TM_CCOEFF_NORMED)


	//4. Normalize <SKIP>
	//out = out * fNCCThreshold;								//알아서 normalized 되어있음 1
	//normalize(out, out, 0, 1, NORM_MINMAX, -1, Mat());		//알아서 normalized 되어있음 2


	//5. Finding Max, Min value 
	vector<Point> arr;
	//Point *arr = new Point[nObjects];
	Mat out_tmp; out.copyTo(out_tmp);
	Mat mat_roi;
	Size ignore_size(pattern.cols, pattern.rows);
	Rect roi;
	int object_number = 0;


	double minVal; double maxVal; Point minLoc; Point maxLoc; Point matchLoc;
	while(1)
	{
		minMaxLoc(out_tmp, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
		if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED) 	//square difference = minimum value  
			matchLoc = minLoc;
		else
			matchLoc = maxLoc;

		roi = Rect(matchLoc - Point(ignore_size.width / 2, ignore_size.height / 2), ignore_size);
		if (roi.x < 0) {								//영상 오른쪽으로 오버 
			roi.width += roi.x;
			roi.x = 0;
		}
		if (roi.y < 0) {								//영상 위쪽으로 오버 
			roi.height += roi.y;
			roi.y = 0;
		}
		if (roi.x + roi.width > out_tmp.cols - 1) {		//영상 왼쪽으로 오버 
			int over_x = (roi.x + roi.width + 1) - out_tmp.cols - 1;
			roi.width -= over_x;
		}
		if (roi.y + roi.height > out_tmp.rows - 1) {	//영상 오른쪽으로 오버 
			int over_y = (roi.y + roi.height + 1) - out_tmp.rows - 1;
			roi.height -= over_y;
		}

		mat_roi = out_tmp(roi);

		if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED) 	//square difference = minimum value  
			mat_roi.setTo(255.f);
		else
			mat_roi.setTo(0.f);

		arr.push_back(matchLoc);
		if (out.at<float>(matchLoc.y, matchLoc.x) > fNCCThreshold)
			object_number++;
		else
			break;
	} 


	//6. Print Info
	string strTmp;
	for (int n = 0; n < object_number; n++) {
		AfxPrintInfo(_T("%d\t: (%d, %d)\t: %.3f"), n, arr[n].x, arr[n].y, out.at<float>(arr[n].y, arr[n].x));	//cv32f 이라서 double Template 사용시 오류 ... 황당하군
		printf("%d\t: (%d, %d)\t : %.3f\n", n, arr[n].x, arr[n].y, out.at<float>(arr[n].y, arr[n].x));
	}

	//7. Display Image 
	if (img.channels() == 1) {
		cvtColor(img, img, COLOR_GRAY2BGR);
		cvtColor(out, out, COLOR_GRAY2BGR);
		//cvtColor(out_tmp, out_tmp, COLOR_GRAY2BGR);
	}
	for (int n = 0; n < object_number; n++) {
		rectangle(img, arr[n], Point(arr[n].x + pattern.cols, arr[n].y + pattern.rows), Scalar(255, 255, 0), 1);
		circle(out, arr[n], 10, Scalar(0, 0, 255), 2);
	}
	out.convertTo(out, CV_8U, 255); // [0..1] -> [0..255] range
									// 	out_tmp.convertTo(out_tmp, CV_8U, 255); // [0..1] -> [0..255] range
									// 	AfxNewImage(out_tmp);
	return true;
}


bool IpHoughLine(Mat &img, Mat &dst, double rho, double theta, int threshold, int mask_height, int mask_width)
{

	//1. Convert to color 
	cvtColor(img, dst, COLOR_GRAY2BGR);

	//2. cv::HoughLines()
	rho *= 1.0;
	theta *= CV_PI / 180;

	vector<Vec3f> lines;	//lines[i][0]->rho, [1]->theta, [2]->votes
	HoughLines(img, lines, rho, theta, threshold);

	//3. Draw Line
	Mat Line_image(1000, 180, CV_8U, Scalar(0));
	Point3_<int> *pRhoThetaVotes = new Point3_<int>[lines.size()];
	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		int votes = lines[i][2];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		//prect[i] = Rect(pt1, pt2);
		//line(dst, pt1, pt2, Scalar(0, 0, 255), 1, LINE_AA);
		Line_image.at<byte>((int)(500 - rho), (int)(theta*180/CV_PI)) = votes;
		pRhoThetaVotes[i] = Point3_<int>((int)(theta * 180 / CV_PI), (int)rho, votes);
	}
	AfxNewImage(Line_image); 


	//4. mask_second try
	vector<int> nglist;
	int Rho, Theta, Votes, comp_rho, comp_theta, comp_votes;
	int h2 = mask_height / 2;
	int w2 = mask_width / 2;
	for (int i = 0; i < lines.size(); i++) {
		Theta = pRhoThetaVotes[i].x;				//기준 변수들 선언
		Rho = pRhoThetaVotes[i].y;
		Votes = pRhoThetaVotes[i].z;
		
		for (int n = 0; n < lines.size(); n++) {
			comp_theta = pRhoThetaVotes[n].x;		//비교할 변수들 선언 
			comp_rho = pRhoThetaVotes[n].y;
			comp_votes = pRhoThetaVotes[n].z;
			if ((Rho - h2 <= comp_rho && comp_rho <= Rho -h2 + mask_height)					//마스크 범위 내에 있으면 
				&& (Theta - w2 <= comp_theta && comp_theta <= Theta -w2 + mask_width))
			{
				if ((Votes > comp_votes) && (comp_votes != 0)) {							//votes 값 비교하고 만약 기준이 크면
					pRhoThetaVotes[n].z = 0;
					nglist.push_back(n);
				}
			}
		}
	}

	sort(nglist.begin(), nglist.end(), greater<int>());
	int ng_cnt;
	for (size_t i = 0; i < lines.size(); i++)
	{
		ng_cnt = nglist.back();
		if (i == ng_cnt) {
			nglist.pop_back();
			continue;
		}

		float rho = lines[i][0], theta = lines[i][1];
		int votes = lines[i][2];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));

		line(dst, pt1, pt2, Scalar(0, 0, 255), 1, LINE_AA);
	}


	//4. mask_first try
	/*
	Point std_pt1, std_pt2, comp_pt1, comp_pt2;
	int mHeight = dlg.m_nMaskHeight;
	int mWidth = dlg.m_nMaskWidth;
	for (int i = 0; i < lines.size(); i++) {
	std_pt1 = prect[i].tl();	std_pt1.x -= mWidth / 2;	std_pt1.y -= mHeight / 2;
	std_pt2 = prect[i].br();	std_pt2.x -= mWidth / 2; 	std_pt2.y -= mHeight / 2;

	for (int n = 0; n < lines.size(); n++) {
	comp_pt1 = prect[n].tl();
	comp_pt2 = prect[n].br();

	if (((std_pt1.x <= comp_pt1.x && comp_pt1.x <= std_pt1.x + mWidth) && (std_pt1.y <= comp_pt1.y && comp_pt1.y <= std_pt1.y + mHeight))
	|| ((std_pt2.x <= comp_pt2.x && comp_pt2.x <= std_pt2.x + mWidth) && (std_pt2.y <= comp_pt2.y && comp_pt2.y <= std_pt2.y + mHeight)))
	{
	if(lines[i][2] >= lines[n][2])
	prect[n] = prect[i];
	else
	prect[i] = prect[n];
	}
	}
	}

	for (size_t i = 0; i < lines.size(); i++)
	line(BGR_canny_img, prect[i].tl(), prect[i].br(), Scalar(0, 0, 255), 1, LINE_AA);
	*/

	//9. Show Image

	return true;
}

bool IpContourTracing(Mat & img, Mat & pattern)
{
	if (!img.data)	return false;
	if (img.type() != CV_8UC1)	return false;

	vector<vector<Point>>	contour;

	//findContours(img, contour, RETR_LIST, CHAIN_APPROX_NONE);
	findContours(img, contour, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	cvtColor(img, pattern, COLOR_GRAY2BGR);

	for (int i = 0; i < contour.size(); i++) {
		Scalar xxx(rand() % 256, rand() % 256, rand() % 256);
		drawContours(pattern, contour, i, xxx, FILLED, LINE_8);
		drawContours(pattern, contour, i, Scalar(0, 0, 255), 2, LINE_8);
	}
	return true;
}

bool IpContourTracingOutline(Mat & img, Mat & pattern)
{
// 	if (!img.data)	return false;
// 	if (img.type() != CV_8UC1)	return false;
// 
// 	vector<vector<Point>>	contour, contours;
// 	findContours(img, contour, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
// 	
// 	vector<Point> ct = contour[0];
// 
// 	double epsilon = 0.2 * arcLength(ct, true);
// 	approxPolyDP(ct, img, epsilon, true);
// 
// 
// 	cvtColor(img, pattern, COLOR_GRAY2BGR);
// 
// 	for (int i = 0; i < contour.size(); i++) {
// 		Scalar xxx(rand() % 256, rand() % 256, rand() % 256);
// 		drawContours(pattern, contour, i, xxx, FILLED, LINE_8);
// 		drawContours(pattern, contour, i, Scalar(0, 0, 255), 2, LINE_8);
// 	}

	if (!img.data)	return false;
	if (img.type() != CV_8UC1)	return false;
	Mat img2;
	img.copyTo(img2);

	vector<vector<Point>>	contours;
	vector<Point> pts;
	findContours(img, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); i++)
	{
		for (int j = 0; j < contours[i].size(); j++)
			pts.push_back(contours[i][j]);
	}


	Rect bR;
	Point pCenter;
	bR = boundingRect(pts);
	pCenter = bR.tl() + Point(bR.width / 2, bR.height / 2);
	cvtColor(img, pattern, COLOR_GRAY2BGR);
	rectangle(pattern, bR, Scalar(255, 0, 0), 3);
	circle(pattern, pCenter, 5, Scalar(0, 255, 0), 10);
	AfxPrintInfo(_T("coutour : %d\tcenter : %d,%d"), 0, pCenter.x, pCenter.y);

	return true;
}

struct EuclideanDistanceFunctor
{
	int _dist2;
	EuclideanDistanceFunctor(int dist) : _dist2(dist*dist) {}

	bool operator()(const Point& lhs, const Point& rhs) const
	{
		return ((lhs.x - rhs.x)*(lhs.x - rhs.x) + (lhs.y - rhs.y)*(lhs.y - rhs.y)) < _dist2;
	}
};

bool IpContourTracingEuclidean(Mat & img, Mat & pattern)
{
	if (!img.data)	return false;
	if (img.type() != CV_8UC1)	return false;
	Mat img2;
	img.copyTo(img2);

	vector<vector<Point>>	contours;
	vector<Point> pts;
	findContours(img, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); i++)
	{
		for (int j = 0; j < contours[i].size(); j++)
			pts.push_back(contours[i][j]);
	}
	//>>
	int th_distance = 100; // radius tolerance
	vector<int> labels;

	int th2 = th_distance * th_distance;
	int n_labels = partition(pts, labels, [th2](const Point& lhs, const Point& rhs) {
		return ((lhs.x - rhs.x)*(lhs.x - rhs.x) + (lhs.y - rhs.y)*(lhs.y - rhs.y)) < th2;
	});

	vector<vector<Point>> clusters(n_labels);
	vector<Point> centroids(n_labels, Point(0, 0));

	for (int i = 0; i < pts.size(); ++i)
	{
		clusters[labels[i]].push_back(pts[i]);
		centroids[labels[i]] += pts[i];
	}
	for (int i = 0; i < n_labels; ++i)
	{
		centroids[i].x /= clusters[i].size();
		centroids[i].y /= clusters[i].size();
	}

	vector<Vec3b> colors;
	for (int i = 0; i < n_labels; ++i)
	{
		colors.push_back(Vec3b(rand() & 255, rand() & 255, rand() & 255));
	}

	Mat3b res(img.rows, img.cols, Vec3b(0, 0, 0));
	for (int i = 0; i < pts.size(); ++i)
	{
		res(pts[i]) = colors[labels[i]];
	}

	for (int i = 0; i < n_labels; ++i)
	{
		circle(res, centroids[i], 3, Scalar(colors[i][0], colors[i][1], colors[i][2]), FILLED);
		circle(res, centroids[i], 6, Scalar(255 - colors[i][0], 255 - colors[i][1], 255 - colors[i][2]));
	}

	// Draw ROI
	cvtColor(img, pattern, COLOR_GRAY2BGR);
	for (int i = 0; i < n_labels; ++i)
	{
		Rect bR;
		Point pCenter;
		bR = boundingRect(clusters[i]);
		pCenter = bR.tl() + Point(bR.width / 2, bR.height / 2);
		rectangle(pattern, bR, Scalar(colors[i][0], colors[i][1], colors[i][2]), 3);
		circle(pattern, pCenter, 5, Scalar(255 - colors[i][0], 255 - colors[i][1], 255 - colors[i][2]), 10);
		AfxPrintInfo(_T("coutour : %d\tcenter : %d,%d"), i, pCenter.x, pCenter.y);
		printf("coutour : %d\tcenter : %d,%d\n", i, pCenter.x, pCenter.y);
	}
	return true;
}

bool IpContourTracingEuclideanFast(Mat & img, Mat & pattern, int threshold)
{
	if (!img.data)	return false;
	if (img.type() != CV_8UC1)	return false;
	Mat img2;
	img.copyTo(img2);

	vector<vector<Point>>	contours;
	vector<Point> pts, pts_represent;
	vector<int> contours_size;
	findContours(img, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); i++){
		int nSize = contours[i].size();
		for (int j = 0; j < nSize; j++) {
			pts.push_back(contours[i][j]);
		}
		contours_size.push_back(nSize);
		pts_represent.push_back(contours[i][0]);
	}
	//>>
	int th_distance = threshold; // radius tolerance
	vector<int> labels;

	int th2 = th_distance * th_distance;
	int n_labels = partition(pts_represent, labels, [th2](const Point& lhs, const Point& rhs) {
		return ((lhs.x - rhs.x)*(lhs.x - rhs.x) + (lhs.y - rhs.y)*(lhs.y - rhs.y)) < th2;
	});

	vector<vector<Point>> clusters(n_labels);
	vector<Point> centroids(n_labels, Point(0, 0));

	int index = 0;
	for (int i = 0; i < pts_represent.size(); ++i)
	{
		int contour_size = contours_size[i];
		for (int cs = 0; cs < contour_size; cs++)
		{
			clusters[labels[i]].push_back(pts[index]);
			centroids[labels[i]] += pts[index];
			index++;
		}
	}
	for (int i = 0; i < n_labels; ++i)
	{
		centroids[i].x /= clusters[i].size();
		centroids[i].y /= clusters[i].size();
	}

	vector<Vec3b> colors;
	for (int i = 0; i < n_labels; ++i)
	{
		colors.push_back(Vec3b(rand() & 255, rand() & 255, rand() & 255));
	}

	Mat3b res(img.rows, img.cols, Vec3b(0, 0, 0));
	for (int i = 0; i < pts_represent.size(); ++i)
	{
		res(pts_represent[i]) = colors[labels[i]];
	}


	cvtColor(img, pattern, COLOR_GRAY2BGR);
	for (int i = 0; i < n_labels; ++i)
	{
		Rect bR;
		Point pCenter;
		bR = boundingRect(clusters[i]);
		pCenter = bR.tl() + Point(bR.width / 2, bR.height / 2);
// 		rectangle(pattern, bR, Scalar(colors[i][0], colors[i][1], colors[i][2]), 3);
// 		circle(pattern, pCenter, 5, Scalar(255 - colors[i][0], 255 - colors[i][1], 255 - colors[i][2]), 10);
		rectangle(pattern, bR, Scalar(255,255,255), 3);
		circle(pattern, pCenter, 5, Scalar(255,255,255), 10);
		AfxPrintInfo(_T("contour;%d;center;%d;%d;W;%d;H;%d;area;%d"), i, pCenter.x, pCenter.y,bR.width, bR.height, bR.area());
		printf("contour;%d;center;%d;%d;W;%d;H;%d;area;%d\n", i, pCenter.x, pCenter.y, bR.width, bR.height, bR.area());
	}
	return true;
}

// bool IpContourTracingEuclideanFastwithData(Mat & img, Mat & pattern, int threshold, vector<strContourInfo>& info)
// {
// 	return false;
// }
