#include "QuadShader.h"

// Number of vertices to draw as triangles
const int kQuadShaderDrawVertCount = 6;

QuadShader::QuadShader(): Shader() {
	glGenBuffers(1, &m_ElementBuffer);

	uint32_t indices[] = {0, 3, 1, 1, 3, 2};

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, kQuadShaderDrawVertCount * sizeof(uint32_t), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

QuadShader::~QuadShader() {
	glDeleteBuffers(1, &m_ElementBuffer);
}

void QuadShader::Execute(int vertCount) {
	ASSERT(vertCount == 4);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBuffer);

	glDrawElements(GL_TRIANGLES, kQuadShaderDrawVertCount, GL_UNSIGNED_INT, (void*)0);
}

void QuadShader::SetRenderQuad(const RenderQuad& quad) {
	if (quad.m_UsePosition) {
		SetAttribute(kShaderAttribPosition, 4, 2, (const float*)quad.m_PositionData);
	}

	if (quad.m_UseTexcoord) {
		SetAttribute(kShaderAttribTexcoord, 4, 2, (const float*)quad.m_TexcoordData);
	}

	if (quad.m_UseColor) {
		SetAttribute(kShaderAttribColor, 4, 4, (const float*)quad.m_ColorData);
	}
}