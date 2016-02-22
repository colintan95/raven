#ifndef MATRIX_H_
#define MATRIX_H_

//--------------------------------------------------
//
// Matrix.h
//
// Defines the Mat2 and Mat3 matrix classes
//
//--------------------------------------------------

#include "base_include.h"

#include <cmath>

#include "Vector.h"

//--------------------------------------------------
//
// Mat2
//
// Row major 2D matrix
//
// Internal values are named with the row first, then column 
// E.g. double m_M12 - value in the 1st row, 2nd column
//
//--------------------------------------------------
class Mat2 {

public:
	Mat2();
	Mat2(double m11, double m12, double m21, double m22);
	~Mat2();

	const Vec2 GetColumn1() const;
	const Vec2 GetColumn2() const;
	const Vec2 GetRow1() const;
	const Vec2 GetRow2() const;

public:
	// Rotation is in degrees
	static const Mat2 CreateRotateMat(double rotation);

public:
	Mat2& operator=(const Mat2& mat);

	Mat2& operator*=(const Mat2& mat);

	friend const bool operator==(const Mat2& mat1, const Mat2& mat2);
	friend const bool operator!=(const Mat2& mat1, const Mat2& mat2);

	// Mat2-Mat2 multiplication
	friend const Mat2 operator*(const Mat2& mat1, const Mat2& mat2);

	// Mat2-Vec2 multiplication
	friend const Vec2 operator*(const Mat2& mat, const Vec2& vec);

private:
	double m_M11, m_M12, m_M21, m_M22;
};

//--------------------------------------------------
//
// Mat3
//
// Row major 3D matrix
//
// Internal values are named with the row first, then column 
// E.g. double m_M12 - value in the 1st row, 2nd column
//
//--------------------------------------------------
class Mat3 {

public:
	Mat3();
	Mat3(double m11, double m12, double m13,
		double m21, double m22, double m23,
		double m31, double m32, double m33);
	~Mat3();

	const Vec3 GetColumn1() const;
	const Vec3 GetColumn2() const;
	const Vec3 GetColumn3() const;
	const Vec3 GetRow1() const;
	const Vec3 GetRow2() const;
	const Vec3 GetRow3() const;

public:
	static const Mat3 CreateIdentityMat();
	static const Mat3 CreateTranslateMat(double dx, double dy);

	// Rotation is in degrees
	static const Mat3 CreateRotateMat(double rotation);

	static const Mat3 CreateScaleMat(double sx, double sy);

public:
	Mat3& operator=(const Mat3& mat);

	Mat3& operator*=(const Mat3& mat);

	friend const bool operator==(const Mat3& mat1, const Mat3& mat2);
	friend const bool operator!=(const Mat3& mat1, const Mat3& mat2);

	// Mat3-Mat3 multiplication
	friend const Mat3 operator*(const Mat3& mat1, const Mat3& mat2);

	// Mat3-Vec3 multiplication
	friend const Vec3 operator*(const Mat3& mat, const Vec3& vec);

private:
	double m_M11, m_M12, m_M13, m_M21, m_M22, m_M23, m_M31, m_M32, m_M33;
};

//--------------------------------------------------
//
// Mat2
//
//--------------------------------------------------
inline Mat2::Mat2() {
	m_M11 = m_M12 = m_M21 = m_M22 = 0.0;
}

inline Mat2::Mat2(double m11, double m12, double m21, double m22) {
	m_M11 = m11; m_M12 = m12; m_M21 = m21; m_M22 = m22;
}

inline Mat2::~Mat2() {
	m_M11 = m_M12 = m_M21 = m_M22 = 0.0;
}

inline const Vec2 Mat2::GetColumn1() const {
	return Vec2(m_M11, m_M21);
}

inline const Vec2 Mat2::GetColumn2() const {
	return Vec2(m_M12, m_M22);
}

inline const Vec2 Mat2::GetRow1() const {
	return Vec2(m_M11, m_M12);
}

inline const Vec2 Mat2::GetRow2() const {
	return Vec2(m_M21, m_M22);
}

inline const Mat2 Mat2::CreateRotateMat(double rotation) {
	// Rotaiton converted to radians
	double cosTheta = (double)cos((double)rotation * M_PI / 180.0);
	double sinTheta = (double)sin((double)rotation * M_PI / 180.0);

	return Mat2(cosTheta, sinTheta, -sinTheta, cosTheta);
}

inline Mat2& Mat2::operator=(const Mat2& mat) {
	m_M11 = mat.GetRow1().GetX(); m_M12 = mat.GetRow1().GetY();
	m_M21 = mat.GetRow2().GetX(); m_M22 = mat.GetRow2().GetY();
	return *this;
}

inline Mat2& Mat2::operator*=(const Mat2& mat) {
	double m11 = Vec2::Dot(this->GetRow1(), mat.GetColumn1());
	double m12 = Vec2::Dot(this->GetRow1(), mat.GetColumn2());
	double m21 = Vec2::Dot(this->GetRow2(), mat.GetColumn1());
	double m22 = Vec2::Dot(this->GetRow2(), mat.GetColumn2());
	m_M11 = m11; m_M12 = m12; m_M21 = m21; m_M22 = m22;
	return *this;
}

inline const bool operator==(const Mat2& mat1, const Mat2& mat2) {
	return (mat1.GetRow1() == mat2.GetRow1() &&
			mat1.GetRow2() == mat2.GetRow2());
}

inline const bool operator!=(const Mat2& mat1, const Mat2& mat2) {
	return !(mat1 == mat2);
}

inline const Mat2 operator*(const Mat2& mat1, const Mat2& mat2) {
	return Mat2(Vec2::Dot(mat1.GetRow1(), mat2.GetColumn1()),
				Vec2::Dot(mat1.GetRow1(), mat2.GetColumn2()),
				Vec2::Dot(mat1.GetRow2(), mat2.GetColumn1()),
				Vec2::Dot(mat1.GetRow2(), mat2.GetColumn2()));
}

inline const Vec2 operator*(const Mat2& mat, const Vec2& vec) {
	return Vec2(Vec2::Dot(mat.GetRow1(), vec),
				Vec2::Dot(mat.GetRow2(), vec));
}


//--------------------------------------------------
//
// Mat3
//
//--------------------------------------------------
inline Mat3::Mat3() {
	m_M11 = m_M12 = m_M13 = 0.0;
	m_M21 = m_M22 = m_M23 = 0.0;
	m_M31 = m_M32 = m_M33 = 0.0;
}

inline Mat3::Mat3(double m11, double m12, double m13,
				double m21, double m22, double m23,
				double m31, double m32, double m33) {
	m_M11 = m11; m_M12 = m12; m_M13 = m13;
	m_M21 = m21; m_M22 = m22; m_M23 = m23;
	m_M31 = m31; m_M32 = m32; m_M33 = m33;
}

inline Mat3::~Mat3() {
	m_M11 = m_M12 = m_M13 = 0.0;
	m_M21 = m_M22 = m_M23 = 0.0;
	m_M31 = m_M32 = m_M33 = 0.0;
}

inline const Vec3 Mat3::GetColumn1() const {
	return Vec3(m_M11, m_M21, m_M31);
}

inline const Vec3 Mat3::GetColumn2() const {
	return Vec3(m_M12, m_M22, m_M32);
}

inline const Vec3 Mat3::GetColumn3() const {
	return Vec3(m_M13, m_M23, m_M33);
}

inline const Vec3 Mat3::GetRow1() const {
	return Vec3(m_M11, m_M12, m_M13);
}

inline const Vec3 Mat3::GetRow2() const {
	return Vec3(m_M21, m_M22, m_M23);
}

inline const Vec3 Mat3::GetRow3() const {
	return Vec3(m_M31, m_M32, m_M33);
}

inline const Mat3 Mat3::CreateIdentityMat() {
	return Mat3(1.f, 0.0, 0.0,
				0.0, 1.f, 0.0,
				0.0, 0.0, 1.f);
}

inline const Mat3 Mat3::CreateTranslateMat(double dx, double dy) {
	return Mat3(1.f, 0.0, dx,
				0.0, 1.f, dy,
				0.0, 0.0, 1.f);
}

inline const Mat3 Mat3::CreateRotateMat(double rotation) {
	// Rotaiton converted to radians
	double cosTheta = (double)cos((double)rotation * M_PI / 180.0);
	double sinTheta = (double)sin((double)rotation * M_PI / 180.0);

	return Mat3(cosTheta, sinTheta, 0.0,
				-sinTheta, cosTheta, 0.0,
				0.0, 0.0, 1.f);
}

inline const Mat3 Mat3::CreateScaleMat(double sx, double sy) {
	return Mat3(sx, 0.0, 0.0,
				0.0, sy, 0.0,
				0.0, 0.0, 1.0);
}

inline Mat3& Mat3::operator=(const Mat3& mat) {
	m_M11 = mat.GetRow1().GetX(); m_M12 = mat.GetRow1().GetY(); m_M13 = mat.GetRow1().GetZ();
	m_M21 = mat.GetRow2().GetX(); m_M22 = mat.GetRow2().GetY(); m_M23 = mat.GetRow2().GetZ();
	m_M31 = mat.GetRow3().GetX(); m_M32 = mat.GetRow3().GetY(); m_M33 = mat.GetRow3().GetZ();
	return *this;
}

inline Mat3& Mat3::operator*=(const Mat3& mat) {
	double m11 = Vec3::Dot(this->GetRow1(), mat.GetColumn1());
	double m12 = Vec3::Dot(this->GetRow1(), mat.GetColumn2());
	double m13 = Vec3::Dot(this->GetRow1(), mat.GetColumn3());
	double m21 = Vec3::Dot(this->GetRow2(), mat.GetColumn1());
	double m22 = Vec3::Dot(this->GetRow2(), mat.GetColumn2());
	double m23 = Vec3::Dot(this->GetRow2(), mat.GetColumn3());
	double m31 = Vec3::Dot(this->GetRow3(), mat.GetColumn1());
	double m32 = Vec3::Dot(this->GetRow3(), mat.GetColumn2());
	double m33 = Vec3::Dot(this->GetRow3(), mat.GetColumn3());
	m_M11 = m11; m_M12 = m12; m_M13 = m13;
	m_M21 = m21; m_M22 = m22; m_M23 = m23;
	m_M31 = m31; m_M32 = m32; m_M33 = m33;
	return *this;
}

inline const bool operator==(const Mat3& mat1, const Mat3& mat2) {
	return (mat1.GetRow1() == mat2.GetRow1() &&
			mat1.GetRow2() == mat2.GetRow2() &&
			mat1.GetRow3() == mat2.GetRow3());
}

inline const bool operator!=(const Mat3& mat1, const Mat3& mat2) {
	return !(mat1 == mat2);
}

inline const Mat3 operator*(const Mat3& mat1, const Mat3& mat2) {
	return Mat3(Vec3::Dot(mat1.GetRow1(), mat2.GetColumn1()),
				Vec3::Dot(mat1.GetRow1(), mat2.GetColumn2()),
				Vec3::Dot(mat1.GetRow1(), mat2.GetColumn3()),
				Vec3::Dot(mat1.GetRow2(), mat2.GetColumn1()),
				Vec3::Dot(mat1.GetRow2(), mat2.GetColumn2()),
				Vec3::Dot(mat1.GetRow2(), mat2.GetColumn3()),
				Vec3::Dot(mat1.GetRow3(), mat2.GetColumn1()),
				Vec3::Dot(mat1.GetRow3(), mat2.GetColumn2()),
				Vec3::Dot(mat1.GetRow3(), mat2.GetColumn3()));
}

inline const Vec3 operator*(const Mat3& mat, const Vec3& vec) {
	return Vec3(Vec3::Dot(mat.GetRow1(), vec),
				Vec3::Dot(mat.GetRow2(), vec),
				Vec3::Dot(mat.GetRow3(), vec));
}

#endif