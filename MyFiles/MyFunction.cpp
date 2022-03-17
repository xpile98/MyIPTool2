#include "stdafx.h"
#include "MyFunction.h"

using namespace cv;
using namespace std;

MyFunction g_MyFunction;

MyFunction::MyFunction()
{
	dtr = 3.141592 / 180;
	image = Mat(800, 800, CV_8UC3, Scalar(255, 255, 255));

	RED = Scalar(0, 0, 255);
	BLUE = Scalar(0, 255, 0);
	GREEN = Scalar(255, 0, 0);
	YELLOW1 = Scalar(128, 255, 255);
	RANDOM_COLOR = Scalar(rand() % 256, rand() % 256, rand() % 256);


	BRUSH_BACKGROUND.CreateSolidBrush(RGB(255, 128, 128));
	BRUSH_WHITE.CreateSolidBrush(RGB(255, 255, 255));
	BRUSH_BLACK.CreateSolidBrush(RGB(0, 0, 0));
	BRUSH_BLUE1.CreateSolidBrush(RGB(0, 128, 255));

	PEN_DOT.CreatePen(PS_DOT, 1, RGB(0, 0, 0));
	PEN_NULL.CreatePen(PS_NULL, 0, RGB(0, 0, 0));
	PEN_YELLOW1.CreatePen(PS_SOLID, 3, RGB(255, 255, 128));
	PEN_DEFAULT.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	FONT_TIMESNEWROMAN.CreatePointFont(200, _T("Times New Roman"));
	FONT_BRADELYHANDITC.CreatePointFont(150, _T("Bradley Hand ITC"));
}


MyFunction::~MyFunction()
{
}

void MyFunction::CV_DrawStar(int _cx, int _cy, int _r, Scalar color, int thickness, int fill)
{
	//Scalar RED(0, 0, 255);
	//Mat image(800, 800, CV_8UC3, Scalar(255, 255, 255));
	Point_<double> p1, p2;

	for (int i = thickness; i < _r; _r--)
	{
		p1 = Point_<double>(_cx, _cy - _r);
		p2 = Point_<double>(_cx - _r*cos(54 * dtr), _cy + _r*sin(54 * dtr));
		line(image, p1, p2, color, thickness);

		p1 = Point_<double>(_cx + _r*cos(18 * dtr), _cy - _r*sin(18 * dtr));
		line(image, p2, p1, color, thickness);

		p2 = Point_<double>(_cx - _r*cos(18 * dtr), _cy - _r*sin(18 * dtr));
		line(image, p1, p2, color, thickness);

		p1 = Point_<double>(_cx + _r*cos(54 * dtr), _cy + _r*sin(54 * dtr));
		line(image, p2, p1, color, thickness);

		p2 = Point_<double>(_cx, _cy - _r);
		line(image, p1, p2, color, thickness);

		if (fill == 0)	break;
	}
	//imshow("CV_DrawStar()", image);	//-> OnDraw함수에서 호출 
}

void MyFunction::BirthdayCard()
{

}

void MyFunction::SetFontSize(CFont &font, int size)
{
	CFont &tempfont = font;
	font.DeleteObject();

	if (&tempfont.m_hObject == &(g_MyFunction.FONT_TIMESNEWROMAN).m_hObject)
		font.CreatePointFont(size * 10, _T("Times New Roman"));
	else if (&tempfont.m_hObject == &(g_MyFunction.FONT_BRADELYHANDITC).m_hObject)
		font.CreatePointFont(size * 10, _T("Bradley Hand ITC"));

	return;
}

//void MyFunction::ShowImagePixel(MyIPImage img)	//수정필요 
//{
//	int height = img.GetHeight();
//	int width = img.GetWidth();
//	void** pixel = img.GetPixels();
//	double** d_pixel = (double**)pixel;
//
//	/*-----------------------------------------------------*/
//	char command[256] = { '\0', };
//	sprintf_s(command, "mode con: lines=200 cols=1000");
//	system(command);
//	/*-----------------------------------------------------*/
//
//
//	for (int y = 0; y < height; y++)
//	{
//		for (int x = 0; x < width; x++)
//		{
//			//cout <<"\t"<< &d_pixel[y][x];
//			printf("%X", (&d_pixel[y][x]));
//		}
//		cout << endl;
//	}
//}
//
//void MyFunction::ShowImagePixel(Mat mat)
//{
//}