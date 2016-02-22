#include "Rect.h"

bool CheckPointIntersectRect(double ptX, double ptY, const Rect& rect) {
	if (rect.GetW() == 0 && rect.GetH() == 0) return false; // Prevents false collision if rect has no width and height

	if (ptX >= rect.GetX() && ptX <= (rect.GetX() + rect.GetW()) ) {
		if (ptY >= rect.GetY() && ptY <= (rect.GetY() + rect.GetH()) ) {
			return true;
		}
	}

    return false;
}