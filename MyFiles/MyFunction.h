#pragma once

#include <opencv2/opencv.hpp>
#include <afxwin.h>		//CBrush, CPen Ŭ���� ������� 

using namespace cv;

//Color Define RGB
#define RGB_RED			RGB(255,0,0)
#define RGB_GREEN		RGB(0,255,0)
#define RGB_BLUE		RGB(0,0,255)
#define RGB_WHITE		RGB(255,255,255)
#define RGB_BLACK		RGB(0,0,0)

//RANDOM COLOR(SCALAR)	
//>>rand�Լ����� ���� �����ڿ��� �ʱ�ȭ �ع����� �� ���� ���� ������ �־� ������ �ǹ̸� �������� ����
#define SCALAR_RANDOM256	Scalar(rand()%256, rand()%256, rand()%256)	


class MyFunction
{
public:
	MyFunction();
	~MyFunction();

	int cx, cy;	//����
	int r;		//������
	int x, y;	//x,y ��ǥ 
	double dtr;	//��->���� 

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