#include "CollisionDetector.h"

#include <cmath>

bool CollisionDetector::PointToAABB(const Vec2& pt, const Rect& rect) {
	return CheckPointIntersectRect(pt.GetX(), pt.GetY(), rect); // Function from Rect.h
}

Vec2 CollisionDetector::LineToAABB(const Vec2& linePt, const Vec2& lineDir, const Rect& rect) {
	// Prevents false collision if either rects has no width and height
	if (rect.GetW() == 0 && rect.GetH() == 0) {
		return Vec2(0.0, 0.0); 
	}

	Vec2 linePt1 = linePt;
	Vec2 linePt2 = linePt + lineDir;

	Vec2 rectPtArray[] = { 	Vec2(rect.GetX(), rect.GetY()),
							Vec2(rect.GetX() + rect.GetW(), rect.GetY()),
							Vec2(rect.GetX() + rect.GetW(), rect.GetY() + rect.GetH()),
							Vec2(rect.GetX(), rect.GetY() + rect.GetH()) };

	double currentDirMag = 1.0f; // Current direction vector magnitude

	// Test intersection for all four line segments of the rectangle
	for (int i = 0; i < 4; ++i) {
		// i is the index of the first point, j the second
		// set j to 0 if i equals 3
		int j = (i < 3) ? (i + 1) : 0;

		Vec2 rectPt1 = rectPtArray[i];
		Vec2 rectPt2 = rectPtArray[j];

		double sNumer = (	(linePt1.GetY() - rectPt1.GetY()) *
							(rectPt2.GetX() - rectPt1.GetX())	) - 
						(	(linePt1.GetX() - rectPt1.GetX()) *
							(rectPt2.GetY() - rectPt1.GetY())	);

		double sDenom =	(	(linePt2.GetX() - linePt1.GetX()) *
							(rectPt2.GetY() - rectPt1.GetY())	) -
						(	(linePt2.GetY() - linePt1.GetY()) *
							(rectPt2.GetX() - rectPt1.GetX())	);

		double tNumer =	(	(linePt1.GetY() - rectPt1.GetY()) *
							(linePt2.GetX() - linePt1.GetX())	) -
						(	(linePt1.GetX() - rectPt1.GetX()) *
							(linePt2.GetY() - linePt1.GetY())	);

		double tDenom = 	(	(linePt2.GetX() - linePt1.GetX()) *
							(rectPt2.GetY() - rectPt1.GetY())	) -
						(	(linePt2.GetY() - linePt1.GetY()) *
							(rectPt2.GetX() - rectPt1.GetX())	);

		double s = sNumer / sDenom;
		double t = tNumer / tDenom;
		
		// Lines intersect if s and t are in range [0,1]
		if ((s >= 0.0 && s <= 1.0) && (t >= 0.0 && t <= 1.0)) {
			currentDirMag = t < currentDirMag ? s : currentDirMag;
		}
	}

	if (currentDirMag == 1.0) {
		return Vec2(0.0, 0.0);
	}
	else {
		return Vec2(linePt + (lineDir * currentDirMag));
	}
}

Vec2 CollisionDetector::AABBToAABB(const Rect& rect1, const Rect& rect2) {
	// Prevents false collision if either rects has no width and height
	if ((rect1.GetW() == 0 && rect1.GetH() == 0) ||
		(rect2.GetW() == 0 && rect2.GetH() == 0)) {
		return Vec2(0.0, 0.0); 
	}

	Vec2 min1(rect1.GetX(), rect1.GetY());
	Vec2 max1(rect1.GetX() + rect1.GetW(), rect1.GetY() + rect1.GetH());
	Vec2 min2(rect2.GetX(), rect2.GetY());
	Vec2 max2(rect2.GetX() + rect2.GetW(), rect2.GetY() + rect2.GetH());

	if ((max1.GetX() < min2.GetX()) || (max1.GetY() < min2.GetY())) {
		return Vec2(0.0, 0.0);
	}

	if ((max2.GetX() < min1.GetX()) || (max2.GetY() < min1.GetY())) {
		return Vec2(0.0, 0.0);
	}

	// Potential resolution offsets
	double negX = max1.GetX() - min2.GetX();
	double posX = max2.GetX() - min1.GetX();
	double negY = max1.GetY() - min2.GetY();
	double posY = max2.GetY() - min1.GetY();

	// Returns the component vector that needs the least offset to resolve the collision
	if (negX <= posX && negX <= negY && negX <= posY) {
		return Vec2(-negX, 0.0);
	}
	else if (posX <= negY && posX <= posY) {
		return Vec2(posX, 0.0);
	}
	else if (negY <= posY) {
		return Vec2(0.0, -negY);
	}
	else {
		return Vec2(0.0, posY);
	}

	return Vec2(0.0, 0.0);
}

Vec2 CollisionDetector::QuadToQuad(const Vec2 quad1[4], const Vec2 quad2[4]) {
	Vec2 normals[8];

	normals[0] = Vec2::Normalize(Vec2::Orthogonal(quad1[1] - quad1[0]));
	normals[1] = Vec2::Normalize(Vec2::Orthogonal(quad1[2] - quad1[1]));
	normals[2] = Vec2::Normalize(Vec2::Orthogonal(quad1[3] - quad1[2]));
	normals[3] = Vec2::Normalize(Vec2::Orthogonal(quad1[0] - quad1[3]));
	normals[4] = Vec2::Normalize(Vec2::Orthogonal(quad2[1] - quad2[0]));
	normals[5] = Vec2::Normalize(Vec2::Orthogonal(quad2[2] - quad2[1]));
	normals[6] = Vec2::Normalize(Vec2::Orthogonal(quad2[3] - quad2[2]));
	normals[7] = Vec2::Normalize(Vec2::Orthogonal(quad2[0] - quad2[3]));

	double minOverlap = 0.0;

	// Index of normal axis where the overlap is smallest
	int minOverlapNorm = 0;

	for (int i = 0; i < 8; ++i) {

		CollisionProjection proj1 = ProjectQuadToAxis(quad1, normals[i]);
		CollisionProjection proj2 = ProjectQuadToAxis(quad2, normals[i]);

		double overlap = TestProjectionOverlap(proj1, proj2);

		// No collision if there is no overlap in at least one of the axes
		if (overlap == 0.0) {
			return Vec2(0.0, 0.0);
		}

		if (minOverlap == 0.0) {
			minOverlap = overlap;
			minOverlapNorm = i;
		}
		else if (overlap < minOverlap) {
			minOverlap = overlap;
			minOverlapNorm = i;
		}
	}

	return normals[minOverlapNorm] * minOverlap;
}

CollisionProjection CollisionDetector::ProjectQuadToAxis(const Vec2 quad[4], const Vec2& axis) {
	double min = 0.0;
	double max = 0.0;

	for (int i = 0; i < 4; ++i) {
		double length = Vec2::Dot(quad[i], axis);
		if (length < min) {
			min = length;
		}
		else if (length > max) {
			max = length;
		}
	}

	CollisionProjection proj;
	proj.min = min;
	proj.max = max;

	return proj;
}

double CollisionDetector::TestProjectionOverlap(const CollisionProjection& proj1, const CollisionProjection& proj2) {
	if (proj1.max < proj2.min || proj2.max < proj1.min) {
		return 0.0;
	}

	if (proj1.max > proj2.min) {
		return proj1.max - proj2.min;
	}
	else {
		return proj2.max - proj1.min;
	}
}