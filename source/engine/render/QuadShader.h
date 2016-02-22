#ifndef QUADSHADER_H_
#define QUADSHADER_H_

#include "base_include.h"

#include "Shader.h"
#include "RenderQuad.h"

//--------------------------------------------------
//
// QuadShader
//
// Shader that draws its vertices as a quad
//
// Vertices should be specified in the anti-clockwise direction, starting from
// the top left vertex
//
// Specify data using a RenderQuad
//
// SetAttribute() should only be called with 4 vertices in the correct order
//
//--------------------------------------------------
class QuadShader: public Shader {

public:
	QuadShader();
	virtual ~QuadShader();

	// vertCount must be 4
	virtual void Execute(int vertCount);

	// Specifies vertex data using a RenderQuad
	void SetRenderQuad(const RenderQuad& quad);

private:
	// Stores indices to render quad by glDrawElements
	GLuint m_ElementBuffer;
};

#endif