#pragma once
class CShapeElement
{
protected:
	int m_nElement;
	CPoint *m_points;
	CPoint m_Center;
public:
	CShapeElement();
	~CShapeElement();

	int GetSize() const { return m_nElement; };
	CPoint* GetPoint() const { return m_points; };
	CPoint GetCenter() const { return m_Center; };

	bool disk(int radius);
	bool ring(int radius);
	bool box(int height, int width);
	bool frame(int height, int width);
	bool cross(int height, int width);
	bool diamond(int height, int width);
};

