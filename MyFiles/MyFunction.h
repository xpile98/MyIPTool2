#pragma once

#include <opencv2/opencv.hpp>
#include <afxwin.h>		//CBrush, CPen 클래스 헤더파일 

using namespace cv;

//Color Define RGB
#define RGB_RED			RGB(255,0,0)
#define RGB_GREEN		RGB(0,255,0)
#define RGB_BLUE		RGB(0,0,255)
#define RGB_WHITE		RGB(255,255,255)
#define RGB_BLACK		RGB(0,0,0)

//RANDOM COLOR(SCALAR)	
//>>rand함수같은 경우는 생성자에서 초기화 해버리면 그 때의 값을 가지고 있어 랜덤의 의미를 구현하지 않음
#define SCALAR_RANDOM256	Scalar(rand()%256, rand()%256, rand()%256)	


class MyFunction
{
public:
	MyFunction();
	~MyFunction();

	int cx, cy;	//중점
	int r;		//반지름
	int x, y;	//x,y 좌표 
	double dtr;	//도->라디안 

	Scalar RED;
	Scalar BLUE;
	Scalar GREEN;
	Scalar YELLOW1;
	Scalar RANDOM_COLOR;

	CBrush BRUSH_BACKGROUND;
	CBrush BRUSH_WHITE;
	CBrush BRUSH_BLACK;
	CBrush BRUSH_BLUE1;

	CPen PEN_DOT;
	CPen PEN_NULL;
	CPen PEN_YELLOW1;
	CPen PEN_DEFAULT;

	CFont FONT_TIMESNEWROMAN;
	CFont FONT_BRADELYHANDITC;

	Mat image;

	void CV_DrawStar(int _cx, int _cy, int _r, Scalar color, int thickness = 1, int fill = 0);
	void BirthdayCard();
	void SetFontSize(CFont &font, int size);

// 	void ShowImagePixel(MyIPImage img);
// 	void ShowImagePixel(Mat mat);

}extern g_MyFunction;																			