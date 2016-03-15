#include "Shader.h"

#include <cstring>

Shader::Shader() {
	m_PositionLoc = -1;
	m_TexcoordLoc = -1;
	m_ColorLoc = -1;

	m_Program = glCreateProgram();

	glGenBuffers(1, &m_PositionBuffer);
	glGenBuffers(1, &m_TexcoordBuffer);
	glGenBuffers(1, &m_ColorBuffer);

	glGenVertexArrays(1, &m_Vao);
}

Shader::~Shader() {
	glDeleteVertexArrays(1, &m_Vao);

	glDeleteBuffers(1, &m_ColorBuffer);
	glDeleteBuffers(1, &m_TexcoordBuffer);
	glDeleteBuffers(1, &m_PositionBuffer);

	glDeleteProgram(m_Program);
}

void Shader::BindShader() {
	glUseProgram(m_Program);

	glBindVertexArray(m_Vao);
}

void Shader::UnbindShader() {
	glBindVertexArray(0);

	glUseProgram(0);
}

void Shader::Execute(int vertCount) {
	glDrawArrays(GL_TRIANGLES, 0, vertCount);
}

void Shader::InitWithSource(const char* vertSource, const char* fragSource) {
	GLint logLength;
	GLint compileStatus;

	// Compile the vertex shader

	m_VertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(m_VertexShader, 1, &vertSource, NULL);
	glCompileShader(m_VertexShader);

	glGetShaderiv(m_VertexShader, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		GLchar* log = (GLchar*)malloc(sizeof(GLchar) * logLength);
		glGetShaderInfoLog(m_VertexShader, logLength, &logLength, log);
		LOG_PRINT(log);
		free(log);
	}

	glGetShaderiv(m_VertexShader, GL_COMPILE_STATUS, &compileStatus);
	ASSERT(compileStatus);


	// Compile the fragment shader

	m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(m_FragmentShader, 1, &fragSource, NULL);
	glCompileShader(m_FragmentShader);

	glGetShaderiv(m_FragmentShader, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0) {
		GLchar* log = (GLchar*)malloc(sizeof(GLchar) * logLength);
		glGetShaderInfoLog(m_FragmentShader, logLength, &logLength, log);
		LOG_PRINT(log);
		free(log);
	}

	glGetShaderiv(m_FragmentShader, GL_COMPILE_STATUS, &compileStatus);
	ASSERT(compileStatus);


	// Attach the shaders to the program

	glAttachShader(m_Program, m_VertexShader);
	glAttachShader(m_Program, m_FragmentShader);

	glLinkProgram(m_Program);
	glUseProgram(m_Program);


	// Finds the attribute locations that the shader uses

	m_PositionLoc = glGetAttribLocation(m_Program, kShaderAttribPositionName);
	m_TexcoordLoc = glGetAttribLocation(m_Program, kShaderAttribTexcoordName);
	m_ColorLoc = glGetAttribLocation(m_Program, kShaderAttribColorName);


	// Enable the vertex attribute arrays

	glBindVertexArray(m_Vao);

	if (m_PositionLoc != -1) glEnableVertexAttribArray(m_PositionLoc);
	if (m_TexcoordLoc != -1) glEnableVertexAttribArray(m_TexcoordLoc);
	if (m_ColorLoc != -1) glEnableVertexAttribArray(m_ColorLoc);

	glBindVertexArray(0);


	glUseProgram(0);
}

void Shader::SetAttribute(ShaderAttrib_t attrib, int vertCount, int vecDim, const float* data) {
	ASSERT(vertCount > 0 && vecDim > 0);

	GLint loc = -1;
	int vertDim = 0;

	if (attrib == kShaderAttribPosition) {

		// Sets the position attribute
		ASSERT(vecDim == kShaderAttribPositionVecDim);
		glBindBuffer(GL_ARRAY_BUFFER, m_PositionBuffer);
		loc = m_PositionLoc;
		vertDim = kShaderAttribPositionVecDim;
	}
	else if (attrib == kShaderAttribTexcoord) {

		// Sets the texcoord attribute
		ASSERT(vecDim == kShaderAttribTexcoordVecDim);
		glBindBuffer(GL_ARRAY_BUFFER, m_TexcoordBuffer);
		loc = m_TexcoordLoc;
		vertDim = kShaderAttribTexcoordVecDim;
	}
	else if (attrib == kShaderAttribColor) {

		// Sets the color attribute
		ASSERT(vecDim == kShaderAttribColorVecDim);
		glBindBuffer(GL_ARRAY_BUFFER, m_ColorBuffer);
		loc = m_ColorLoc;
		vertDim = kShaderAttribColorVecDim;
	}
	else {
		LOG_ERROR("Shader: no such attribute %d", attrib);

		return;
	}

	glBufferData(GL_ARRAY_BUFFER, vertCount * vecDim * sizeof(float), (const void*)data, GL_STREAM_DRAW);
	glVertexAttribPointer(loc, vertDim, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Shader::SetUniform1f(ShaderUniform_t uniform, float v0) {
	GLint loc = GetUniformLocation(uniform);
	glUniform1f(loc, v0);
}

void Shader::SetUniform2f(ShaderUniform_t uniform, float v0, float v1) {
	GLint loc = GetUniformLocation(uniform);
	glUniform2f(loc, v0, v1);
}

void Shader::SetUniform3f(ShaderUniform_t uniform, float v0, float v1, float v2) {
	GLint loc = GetUniformLocation(uniform);
	glUniform3f(loc, v0, v1, v2);
}

void Shader::SetUniform4f(ShaderUniform_t uniform, float v0, float v1, float v2, float v3) {
	GLint loc = GetUniformLocation(uniform);
	glUniform4f(loc, v0, v1, v2, v3);
}

void Shader::SetUniformVec2f(ShaderUniform_t uniform, const Vec2& vec) {
	GLint loc = GetUniformLocation(uniform);
	ConvertVec2(vec);
	glUniform2fv(loc, 1, m_TmpValBuf);
}

void Shader::SetUniformVec3f(ShaderUniform_t uniform, const Vec3& vec) {
    GLint loc = GetUniformLocation(uniform);
    ConvertVec3(vec);
    glUniform3fv(loc, 1, m_TmpValBuf);
}

void Shader::SetUniformMat2f(ShaderUniform_t uniform, const Mat2& mat) {
	GLint loc = GetUniformLocation(uniform);
	ConvertMat2(mat);
	glUniformMatrix2fv(loc, 1, GL_FALSE, m_TmpValBuf);
}

void Shader::SetUniformMat3f(ShaderUniform_t uniform, const Mat3& mat) {
	GLint loc = GetUniformLocation(uniform);
	ConvertMat3(mat);
	glUniformMatrix3fv(loc, 1, GL_FALSE, m_TmpValBuf);
}

void Shader::SetUniformTexture(ShaderUniform_t uniform, const Texture& tex) {
	GLint loc = GetUniformLocation(uniform);
	glUniform1i(loc, tex.m_TextureUnit);
}

GLint Shader::GetUniformLocation(ShaderUniform_t uniform) {
	const char* name;

	switch(uniform) {
	case kShaderUniformModelViewMat:
		name = kShaderUniformModelViewMatName;
		break;
	case kShaderUniformModelMat:
		name = kShaderUniformModelMatName;
		break;
	case kShaderUniformViewMat:
		name = kShaderUniformViewMatName;
		break;
	case kShaderUniformLightPos:
		name = kShaderUniformLightPosName;
		break;
	case kShaderUniformTexture2D:
		name = kShaderUniformTexture2DName;
		break;
	default:
		name = "";
		break;
	}

	GLint loc = glGetUniformLocation(m_Program, name);

	if (loc == -1) {
		LOG_ERROR("Shader: no such uniform %d", uniform);
	}

	return loc;
}

void Shader::ConvertVec2(const Vec2& vec) {
	ResetTmpValBuf();

	m_TmpValBuf[0] = (float)vec.GetX();
	m_TmpValBuf[1] = (float)vec.GetY();
}

void Shader::ConvertVec3(const Vec3& vec) {
	ResetTmpValBuf();

	m_TmpValBuf[0] = (float)vec.GetX();
	m_TmpValBuf[1] = (float)vec.GetY();
	m_TmpValBuf[2] = (float)vec.GetZ();
}

void Shader::ConvertMat2(const Mat2& mat) {
	ResetTmpValBuf();

	m_TmpValBuf[0] = (float)mat.GetColumn1().GetX();
	m_TmpValBuf[1] = (float)mat.GetColumn1().GetY();
	m_TmpValBuf[2] = (float)mat.GetColumn2().GetX();
	m_TmpValBuf[3] = (float)mat.GetColumn2().GetX();
}

void Shader::ConvertMat3(const Mat3& mat) {
	ResetTmpValBuf();

	m_TmpValBuf[0] = (float)mat.GetColumn1().GetX();
	m_TmpValBuf[1] = (float)mat.GetColumn1().GetY();
	m_TmpValBuf[2] = (float)mat.GetColumn1().GetZ();
	m_TmpValBuf[3] = (float)mat.GetColumn2().GetX();
	m_TmpValBuf[4] = (float)mat.GetColumn2().GetY();
	m_TmpValBuf[5] = (float)mat.GetColumn2().GetZ();
	m_TmpValBuf[6] = (float)mat.GetColumn3().GetX();
	m_TmpValBuf[7] = (float)mat.GetColumn3().GetY();
	m_TmpValBuf[8] = (float)mat.GetColumn3().GetZ();
}

void Shader::ResetTmpValBuf() {
	memset((void*)m_TmpValBuf, 0, sizeof(float) * 16);
}