#include "ShapeElement.h"
#include "stdafx.h"
#include "ShapeElement.h"


CShapeElement::CShapeElement()
{
	m_nElement = 0;
	m_points = (CPoint*)NULL;
}


CShapeElement::~CShapeElement()
{
	if (m_points) {
		delete[] m_points;
		m_points = (CPoint *)NULL;
	}
}

bool CShapeElement::disk(int radius)
{
	if (radius < 1)	return false;

	int diameter = (radius * 2) + 1;
	m_nElement = 0;
	m_points = new CPoint[diameter*diameter];
	if (!m_points) return false;

	m_Center = CPoint(radius, radius);
	int d;
	float dLimit = ((float)radius + 0.5f)*((float)radius + 0.5f);
	int dy, dx;
	for (int y = 0; y < diameter; y++) {
		for (int x = 0; x < diameter; x++) {
			dx = x - radius;
			dy = y - radius;
			d = (dx*dx) + (dy*dy);
			if (d < dLimit)
				m_points[m_nElement++] = CPoint(dx, dy);
		}
	}
	return true;
}

bool CShapeElement::ring(int radius)
{
	if (radius < 1)	return false;

	int diameter = (radius * 2) + 1;
	m_nElement = 0;
	m_points = new CPoint[diameter*diameter];
	if (!m_points) return false;

	m_Center = CPoint(radius, radius);
	double d;
	int dy, dx;
	for (int y = 0; y < diameter; y++) {
		for (int x = 0; x < diameter; x++) {
			dx = x - radius;
			dy = y - radius;
			d = sqrt((dx*dx) + (dy*dy));
			if (((double)radius <= d) && (d < (double)(radius + 1)))
				m_points[m_nElement++] = CPoint(dx, dy);
		}
	}
	return true;
}

bool CShapeElement::box(int height, int width)
{
	if (height < 1 || width < 1)	return false;

	m_nElement = height*width;
	m_points = new CPoint[m_nElement];
	if (!m_points) return false;

	int cx = (width - 1) / 2;
	int cy = (height - 1) / 2;
	m_Center = CPoint(cx, cy);

	int k = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			m_points[k++] = CPoint(x - cx, y - cy);
		}
	}
	return true;
}

bool CShapeElement::frame(int height, int width)
{
	if (height < 1 || width < 1)	return false;

	m_nElement = 2 * (height + width - 2);
	m_points = new CPoint[m_nElement];
	if (!m_points) return false;

	int cx = (width - 1) / 2;
	int cy = (height - 1) / 2;
	m_Center = CPoint(cx, cy);

	int k = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (y == 0 || y == height - 1 || x == 0 || x == width - 1)
				m_points[k++] = CPoint(x - cx, y - cy);
		}
	}
	return true;
}

bool CShapeElement::cross(int height, int width)
{
	if (height < 1 || width < 1)	return false;

	m_nElement = 2 * (height + width - 2);
	m_points = new CPoint[m_nElement];
	if (!m_points) return false;

	int cx = (width - 1) / 2;
	int cy = (height - 1) / 2;
	m_Center = CPoint(cx, cy);

	int k = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (y == cy || x == cx)
				m_points[k++] = CPoint(x - cx, y - cy);
		}
	}
	return true;
}

bool CShapeElement::diamond(int height, int width)
{
	return false;
}
