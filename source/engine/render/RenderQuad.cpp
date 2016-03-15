#include "RenderQuad.h"

#include <cstring>

RenderQuad::RenderQuad() {
	ClearPositionData();
	ClearTexcoordData();
	ClearColorData();
}

RenderQuad::~RenderQuad() {

}

void RenderQuad::SetPosition(const Vec2& topLeft, const Vec2& topRight, const Vec2& bottomRight, const Vec2& bottomLeft) {
	ClearPositionData();

	m_PositionData[0] = topLeft.GetX();
	m_PositionData[1] = topLeft.GetY();
	m_PositionData[2] = topRight.GetX();
	m_PositionData[3] = topRight.GetY();
	m_PositionData[4] = bottomRight.GetX();
	m_PositionData[5] = bottomRight.GetY();
	m_PositionData[6] = bottomLeft.GetX();
	m_PositionData[7] = bottomLeft.GetY();

	m_UsePosition = true;
}

void RenderQuad::SetTexcoord(const Vec2& topLeft, const Vec2& topRight, const Vec2& bottomRight, const Vec2& bottomLeft) {
	ClearTexcoordData();

	m_TexcoordData[0] = topLeft.GetX();
	m_TexcoordData[1] = topLeft.GetY();
	m_TexcoordData[2] = topRight.GetX();
	m_TexcoordData[3] = topRight.GetY();
	m_TexcoordData[4] = bottomRight.GetX();
	m_TexcoordData[5] = bottomRight.GetY();
	m_TexcoordData[6] = bottomLeft.GetX();
	m_TexcoordData[7] = bottomLeft.GetY();

	m_UseTexcoord = true;
}

void RenderQuad::SetColor(const Vec4& topLeft, const Vec4& topRight, const Vec4& bottomRight, const Vec4& bottomLeft) {
	ClearColorData();

	m_ColorData[0] = topLeft.GetX();
	m_ColorData[1] = topLeft.GetY();
	m_ColorData[2] = topLeft.GetZ();
	m_ColorData[3] = topLeft.GetW();
	m_ColorData[4] = topRight.GetX();
	m_ColorData[5] = topRight.GetY();
	m_ColorData[6] = topRight.GetZ();
	m_ColorData[7] = topRight.GetW();
	m_ColorData[8] = bottomRight.GetX();
	m_ColorData[9] = bottomRight.GetY();
	m_ColorData[10] = bottomRight.GetZ();
	m_ColorData[11] = bottomRight.GetW();
	m_ColorData[12] = bottomLeft.GetX();
	m_ColorData[13] = bottomLeft.GetY();
	m_ColorData[14] = bottomLeft.GetZ();
	m_ColorData[15] = bottomLeft.GetW();

	m_UseColor = true;
}

void RenderQuad::ClearPositionData() {
	memset((void*)m_PositionData, 0, 8 * sizeof(float));
	m_UsePosition = false;
}

void RenderQuad::ClearTexcoordData() {
	memset((void*)m_TexcoordData, 0, 8 * sizeof(float));
	m_UseTexcoord = false;
}

void RenderQuad::ClearColorData() {
	memset((void*)m_ColorData, 0, 16 * sizeof(float));
	m_UseColor = false;
}