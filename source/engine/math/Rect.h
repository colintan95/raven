#ifndef RECT_H_
#define RECT_H_

#include "base_include.h"

//--------------------------------------------------
//
// Rect
//
// Axis-aligned rectangle
//
//--------------------------------------------------
class Rect {

public:
	Rect();
	Rect(double x, double y, double w, double h);

	double GetX() const { return m_X; }
	double GetY() const { return m_Y; }
	double GetW() const { return m_W; }
	double GetH() const { return m_H; }

private:
	double m_X;
	double m_Y;
	double m_W;
	double m_H;
};

bool CheckPointIntersectRect(double ptX, double ptY, const Rect& rect);

inline Rect::Rect() {
	m_X = m_Y = m_W = m_H = 0.0;
}

inline Rect::Rect(double x, double y, double w, double h) {
	m_X = x; m_Y = y; m_W = w; m_H = h;
}


#endif