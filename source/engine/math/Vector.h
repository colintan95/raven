#ifndef VECTOR_H_
#define VECTOR_H_

//--------------------------------------------------
//
// Vector.h
//
// Defines the Vec2, Vec3 and Vec4 vector classes
//
//--------------------------------------------------

#include "base_include.h"

#include <cmath>


class Mat2;
class Vec3;

//--------------------------------------------------
//
// Vec2
//
// 2D-vector class
//
//--------------------------------------------------
class Vec2 {

public:
	Vec2();
	Vec2(double x, double y);
	Vec2(const Vec3& vec);
	~Vec2();

    double GetLength() const;
    double GetSquaredLength() const;

	double GetX() const { return m_X; }
	double GetY() const { return m_Y; }

public:
	static Vec2 Normalize(const Vec2& vec);
	static double Dot(const Vec2& vec1, const Vec2& vec2);
	static double Cross(const Vec2& vec1, const Vec2& vec2);

	// Returns a vector orthogonal to vec
	static Vec2 Orthogonal(const Vec2& vec);

public:
	Vec2& operator=(const Vec2& vec);

    Vec2& operator+=(const Vec2& vec);
    Vec2& operator-=(const Vec2& vec);
    Vec2& operator*=(double n);
    Vec2& operator/=(double n);

    friend const bool operator==(const Vec2& vec1, const Vec2& vec2);
    friend const bool operator!=(const Vec2& vec1, const Vec2& vec2);

	friend const Vec2 operator+(const Vec2& vec1, const Vec2& vec2);
    friend const Vec2 operator-(const Vec2& vec1, const Vec2& vec2);
    friend const Vec2 operator*(const Vec2& vec, double n);
    friend const Vec2 operator/(const Vec2& vec, double n);

    // Unary minus operator
    // Negates the vector
    friend const Vec2 operator-(const Vec2& vec);

    // Matrix multiplication implementation in matrix.h
    friend const Vec2 operator*(const Mat2& mat, const Vec2& vec);

private:
	double m_X;
	double m_Y;
};


//--------------------------------------------------
//
// Vec3
//
// 3D-vector class
//
//--------------------------------------------------
class Vec3 {

public:
	Vec3();
	Vec3(double x, double y, double z);
	Vec3(const Vec2& vec, double z);
	~Vec3();

    double GetLength() const;
    double GetSquaredLength() const;

	double GetX() const { return m_X; }
	double GetY() const { return m_Y; }
	double GetZ() const { return m_Z; }

public:
	static Vec3 Normalize(const Vec3& vec);
	static double Dot(const Vec3& vec1, const Vec3& vec2);
	static Vec3 Cross(const Vec3& vec1, const Vec3& vec2);

public:
	Vec3& operator=(const Vec3& vec);

    Vec3& operator+=(const Vec3& vec);
    Vec3& operator-=(const Vec3& vec);
    Vec3& operator*=(double n);
    Vec3& operator/=(double n);

    friend const bool operator==(const Vec3& vec1, const Vec3& vec2);
    friend const bool operator!=(const Vec3& vec1, const Vec3& vec2);

	friend const Vec3 operator+(const Vec3& vec1, const Vec3& vec2);
    friend const Vec3 operator-(const Vec3& vec1, const Vec3& vec2);
    friend const Vec3 operator*(const Vec3& vec, double n);
    friend const Vec3 operator/(const Vec3& vec, double n);

    // Unary minus operator
    // Negate the vector
    friend const Vec3 operator-(const Vec3& vec);

    // Matrix multiplication implementation in matrix.h
    //friend const Vec2 operator*(const Mat2& mat, const Vec2& vec);

private:
	double m_X;
	double m_Y;
	double m_Z;
};


//--------------------------------------------------
//
// Vec4
//
// 4D-vector class
//
//--------------------------------------------------
class Vec4 {

public:
	Vec4();
	Vec4(double x, double y, double z, double w);
	~Vec4();

    double GetLength() const;
    double GetSquaredLength() const;

	double GetX() const { return m_X; }
	double GetY() const { return m_Y; }
	double GetZ() const { return m_Z; }
	double GetW() const { return m_W; }

public:
	static Vec4 Normalize(const Vec4& vec);
	static double Dot(const Vec4& vec1, const Vec4& vec2);

public:
	Vec4& operator=(const Vec4& vec);

    Vec4& operator+=(const Vec4& vec);
    Vec4& operator-=(const Vec4& vec);
    Vec4& operator*=(double n);
    Vec4& operator/=(double n);

    friend const bool operator==(const Vec4& vec1, const Vec4& vec2);
    friend const bool operator!=(const Vec4& vec1, const Vec4& vec2);

	friend const Vec4 operator+(const Vec4& vec1, const Vec4& vec2);
    friend const Vec4 operator-(const Vec4& vec1, const Vec4& vec2);
    friend const Vec4 operator*(const Vec4& vec, double n);
    friend const Vec4 operator/(const Vec4& vec, double n);

    // Unary minus operator
    // Negate the vector
    friend const Vec4 operator-(const Vec4& vec);

private:
	double m_X;
	double m_Y;
	double m_Z;
	double m_W;
};

//--------------------------------------------------
//
// Vec2
//
//--------------------------------------------------
inline Vec2::Vec2() {
	m_X = 0.0; m_Y = 0.0;
}

inline Vec2::Vec2(double x, double y) { 
	m_X = x; m_Y = y;
}

inline Vec2::Vec2(const Vec3& vec) {
	m_X = vec.GetX(); m_Y = vec.GetY();
}

inline Vec2::~Vec2() {
	m_X = 0.0; m_Y = 0.0;
}

inline double Vec2::GetLength() const { 
	return sqrt(m_X * m_X + m_Y * m_Y); 
}

inline double Vec2::GetSquaredLength() const { 
	return (m_X * m_X + m_Y * m_Y);
}

inline Vec2 Vec2::Normalize(const Vec2& vec) {
	double length = vec.GetLength();
	return length != 0.0 ? (vec / length) : Vec2(0.0, 0.0);
}

inline double Vec2::Dot(const Vec2& vec1, const Vec2& vec2) {
	return vec1.m_X * vec2.m_X + vec1.m_Y * vec2.m_Y;
}

inline double Vec2::Cross(const Vec2& vec1, const Vec2& vec2) {
	return vec1.m_X * vec2.m_Y - vec1.m_Y * vec2.m_X;
}

inline Vec2 Vec2::Orthogonal(const Vec2& vec) {
	return Vec2(-vec.m_Y, vec.m_X);
}

inline Vec2& Vec2::operator=(const Vec2& vec) {
	m_X = vec.m_X; m_Y = vec.m_Y;
	return *this;
}

inline Vec2& Vec2::operator+=(const Vec2& vec) {
	m_X += vec.m_X; m_Y += vec.m_Y;
	return *this;
}

inline Vec2& Vec2::operator-=(const Vec2& vec) {
	m_X -= vec.m_X; m_Y -= vec.m_Y;
	return *this;
}

inline Vec2& Vec2::operator*=(double n) {
	m_X *= n; m_Y *= n;
	return *this;
}

inline Vec2& Vec2::operator/=(double n) {
	ASSERT(n != 0);
	m_X /= n; m_Y /= n;
	return *this;
}

inline const bool operator==(const Vec2& vec1, const Vec2& vec2) {
	return (vec1.m_X == vec2.m_X && vec1.m_Y == vec2.m_Y);
}

inline const bool operator!=(const Vec2& vec1, const Vec2& vec2) {
	return !(vec1 == vec2);
}

inline const Vec2 operator+(const Vec2& vec1, const Vec2& vec2) {
	return Vec2(vec1.m_X + vec2.m_X, vec1.m_Y + vec2.m_Y);
}

inline const Vec2 operator-(const Vec2& vec1, const Vec2& vec2) {
	return Vec2(vec1.m_X - vec2.m_X, vec1.m_Y - vec2.m_Y);
}

inline const Vec2 operator*(const Vec2& vec, double n) {
	return Vec2(vec.m_X * n, vec.m_Y * n);
}

inline const Vec2 operator/(const Vec2& vec, double n) {
	ASSERT(n != 0);
	return Vec2(vec.m_X / n, vec.m_Y / n);
}

inline const Vec2 operator-(const Vec2& vec) {
	return Vec2(vec.m_X * -1.0, vec.m_Y * -1.0);
}


//--------------------------------------------------
//
// Vec3
//
//--------------------------------------------------
inline Vec3::Vec3() {
	m_X = 0.0; m_Y = 0.0; m_Z = 0.0;
}

inline Vec3::Vec3(double x, double y, double z) {
	m_X = x; m_Y = y; m_Z = z;
}

inline Vec3::Vec3(const Vec2& vec, double z) {
	m_X = vec.GetX(); m_Y = vec.GetY(); m_Z = z;
}

inline Vec3::~Vec3() {
	m_X = m_Y = m_Z = 0.0;
}

inline double Vec3::GetLength() const {
	return sqrt(m_X * m_X + m_Y * m_Y + m_Z * m_Z);
}

inline double Vec3::GetSquaredLength() const {
	return (m_X * m_X + m_Y * m_Y + m_Z * m_Z);
}

inline Vec3 Vec3::Normalize(const Vec3& vec) {
	double length = vec.GetLength();
	return length != 0.0 ? (vec / length) : Vec3(0.0, 0.0, 0.0);
}

inline double Vec3::Dot(const Vec3& vec1, const Vec3& vec2) {
	return vec1.m_X * vec2.m_X + vec1.m_Y * vec2.m_Y + vec1.m_Z * vec2.m_Z;
}

inline Vec3 Vec3::Cross(const Vec3& vec1, const Vec3& vec2) {
	return Vec3(vec1.GetY() * vec2.GetZ() - vec1.GetZ() * vec2.GetY(),
				vec1.GetZ() * vec2.GetX() - vec1.GetX() * vec2.GetZ(),
				vec1.GetX() * vec2.GetY() - vec1.GetY() * vec2.GetX());
}

inline Vec3& Vec3::operator=(const Vec3& vec) {
	m_X = vec.m_X; m_Y = vec.m_Y; m_Z = vec.m_Z;
	return *this;
}

inline Vec3& Vec3::operator+=(const Vec3& vec) {
	m_X += vec.m_X; m_Y += vec.m_Y; m_Z += vec.m_Z;
	return *this;
}

inline Vec3& Vec3::operator-=(const Vec3& vec) {
	m_X -= vec.m_X; m_Y -= vec.m_Y; m_Z -= vec.m_Z;
	return *this;
}

inline Vec3& Vec3::operator*=(double n) {
	m_X *= n; m_Y *= n; m_Z *= n;
	return *this;
}

inline Vec3& Vec3::operator/=(double n) {
	ASSERT(n != 0);
	m_X /= n; m_Y /= n; m_Z /= n;
	return *this;
}

inline const bool operator==(const Vec3& vec1, const Vec3& vec2) {
	return (vec1.m_X == vec2.m_X && vec1.m_Y == vec2.m_Y && vec1.m_Z == vec2.m_Z);
}

inline const bool operator!=(const Vec3& vec1, const Vec3& vec2) {
	return !(vec1 == vec2);
}

inline const Vec3 operator+(const Vec3& vec1, const Vec3& vec2) {
	return Vec3(vec1.m_X + vec2.m_X, vec1.m_Y + vec2.m_Y, vec1.m_Z + vec2.m_Z);
}

inline const Vec3 operator-(const Vec3& vec1, const Vec3& vec2) {
	return Vec3(vec1.m_X - vec2.m_X, vec1.m_Y - vec2.m_Y, vec1.m_Z - vec2.m_Z);
}

inline const Vec3 operator*(const Vec3& vec, double n) {
	return Vec3(vec.m_X * n, vec.m_Y * n, vec.m_Z * n);
}

inline const Vec3 operator/(const Vec3& vec, double n) {
	ASSERT(n != 0);
	return Vec3(vec.m_X / n, vec.m_Y / n, vec.m_Z / n);
}

inline const Vec3 operator-(const Vec3& vec) {
	return Vec3(vec.m_X * -1.0, vec.m_Y * -1.0, vec.m_Z * -1.0);
}


//--------------------------------------------------
//
// Vec4
//
//--------------------------------------------------
inline Vec4::Vec4() {
	m_X = 0.0; m_Y = 0.0; m_Z = 0.0; m_W = 0.0;
}

inline Vec4::Vec4(double x, double y, double z, double w) {
	m_X = x; m_Y = y; m_Z = z; m_W = w;
}

inline Vec4::~Vec4() {
	m_X = m_Y = m_Z = m_W = 0.0;
}

inline double Vec4::GetLength() const {
	return sqrt(m_X * m_X + m_Y * m_Y + m_Z * m_Z + m_W * m_W);
}

inline double Vec4::GetSquaredLength() const {
	return (m_X * m_X + m_Y * m_Y + m_Z * m_Z + m_W * m_W);
}

inline Vec4 Vec4::Normalize(const Vec4& vec) {
	double length = vec.GetLength();
	return length != 0.0 ? (vec / length) : Vec4(0.0, 0.0, 0.0, 0.0);
}

inline double Vec4::Dot(const Vec4& vec1, const Vec4& vec2) {
	return (vec1.m_X * vec2.m_X + vec1.m_Y * vec2.m_Y + 
			vec1.m_Z * vec2.m_Z + vec1.m_W * vec2.m_W);
}

inline Vec4& Vec4::operator=(const Vec4& vec) {
	m_X = vec.m_X; m_Y = vec.m_Y; m_Z = vec.m_Z; m_W = vec.m_W;
	return *this;
}

inline Vec4& Vec4::operator+=(const Vec4& vec) {
	m_X += vec.m_X; m_Y += vec.m_Y; m_Z += vec.m_Z; m_W += vec.m_W;
	return *this;
}

inline Vec4& Vec4::operator-=(const Vec4& vec) {
	m_X -= vec.m_X; m_Y -= vec.m_Y; m_Z -= vec.m_Z; m_W -= vec.m_W;
	return *this;
}

inline Vec4& Vec4::operator*=(double n) {
	m_X *= n; m_Y *= n; m_Z *= n; m_W *= n;
	return *this;
}

inline Vec4& Vec4::operator/=(double n) {
	ASSERT(n != 0);
	m_X /= n; m_Y /= n; m_Z /= n; m_W /= n;
	return *this;
}

inline const bool operator==(const Vec4& vec1, const Vec4& vec2) {
	return (vec1.m_X == vec2.m_X && vec1.m_Y == vec2.m_Y && 
			vec1.m_Z == vec2.m_Z && vec1.m_W == vec2.m_W);
}

inline const bool operator!=(const Vec4& vec1, const Vec4& vec2) {
	return !(vec1 == vec2);
}

inline const Vec4 operator+(const Vec4& vec1, const Vec4& vec2) {
	return Vec4(vec1.m_X + vec2.m_X, vec1.m_Y + vec2.m_Y, 
				vec1.m_Z + vec2.m_Z, vec1.m_W + vec2.m_W);
}

inline const Vec4 operator-(const Vec4& vec1, const Vec4& vec2) {
	return Vec4(vec1.m_X - vec2.m_X, vec1.m_Y - vec2.m_Y, 
				vec1.m_Z - vec2.m_Z, vec1.m_W - vec2.m_W);
}

inline const Vec4 operator*(const Vec4& vec, double n) {
	return Vec4(vec.m_X * n, vec.m_Y * n, 
				vec.m_Z * n, vec.m_W * n);
}

inline const Vec4 operator/(const Vec4& vec, double n) {
	ASSERT(n != 0);
	return Vec4(vec.m_X / n, vec.m_Y / n, vec.m_Z / n, vec.m_W / n);
}

inline const Vec4 operator-(const Vec4& vec) {
	return Vec4(vec.m_X * -1.0, vec.m_Y * -1.0, vec.m_Z * -1.0, vec.m_W * -1.0);
}

#endif