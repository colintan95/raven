#ifndef RENDERQUAD_H_
#define RENDERQUAD_H_

#include "base_include.h"

#include "math/Vector.h"

// Forward declarations
class QuadShader;

//--------------------------------------------------
//
// RenderQuad
//
// Quad to be rendered by QuadShader
//
// Contains vertex attribute data
//
// Vertices should be specified in the anti-clockwise direction, starting from
// the top left vertex
//
//--------------------------------------------------
class RenderQuad {
	friend class QuadShader;

public:
	RenderQuad();
	~RenderQuad();

	void SetPosition(const Vec3& topLeft, const Vec3& topRight, const Vec3& bottomRight, const Vec3& bottomLeft);

	void SetTexcoord(const Vec3& topLeft, const Vec3& topRight, const Vec3& bottomRight, const Vec3& bottomLeft);

	void SetColor(const Vec4& topLeft, const Vec4& topRight, const Vec4& bottomRight, const Vec4& bottomLeft);

private:
	void ClearPositionData();
	void ClearTexcoordData();
	void ClearColorData();

private:
	// Vertex attribute data
	float m_PositionData[12];
	float m_TexcoordData[8];
	float m_ColorData[16];

	// True if the type of data has been specified
	bool m_UsePosition;
	bool m_UseTexcoord;
	bool m_UseColor;
};

#endif