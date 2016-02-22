#ifndef SHADER_H_
#define SHADER_H_

#include "base_include.h"

#include "GraphicsPlatform.h"

#include "math/Matrix.h"
#include "math/Vector.h"

#include "Texture.h"


// OS X uses different function signatures for some OpenGL functions
#if defined(_PLATFORM_OSX)

#define glGenVertexArrays glGenVertexArraysAPPLE
#define glDeleteVertexArrays glDeleteVertexArraysAPPLE
#define glBindVertexArray glBindVertexArrayAPPLE

#endif

//--------------------------------------------------
//
// Shader enums and consts
//
//--------------------------------------------------

// Identifies a shader attribute
enum ShaderAttrib_t {
	kShaderAttribPosition,
	kShaderAttribTexcoord,
	kShaderAttribColor
};

// Identifies a shader uniform
enum ShaderUniform_t {
	kShaderUniformModelViewMat,
	kShaderUniformModelMat,
	kShaderUniformViewMat,
	kShaderUniformLightPos,
	kShaderUniformTexture2D
};

const char* const kShaderAttribPositionName = "v_Position";
const char* const kShaderAttribTexcoordName = "v_Texcoord";
const char* const kShaderAttribColorName = "v_Color";

// Dimensions of the vector for each attribute
const int kShaderAttribPositionVecDim = 3;
const int kShaderAttribTexcoordVecDim = 2;
const int kShaderAttribColorVecDim = 4;

const char* const kShaderUniformModelViewMatName = "u_ModelViewMat";
const char* const kShaderUniformModelMatName = "u_ModelMat";
const char* const kShaderUniformViewMatName = "u_ViewMat";
const char* const kShaderUniformLightPosName = "u_LightPos";
const char* const kShaderUniformTexture2DName = "u_Texture2D";


//--------------------------------------------------
//
// Shader
//
// Base implementation for all shaders
//
//--------------------------------------------------
class Shader {

public:
	Shader();
	virtual ~Shader();

	// Call before using any set functions or execute()
	void BindShader();

	// Call after finished exexuting the shader
	void UnbindShader();	

	// Draws the specified number of vertices as triangles
	//
	// Call after specifying all data
	virtual void Execute(int vertCount);

	// Initializes the vertex and fragment shaders with strings containing the
	// source codes
	void InitWithSource(const char* vertSource, const char* fragSource);

	// Sets the data of the specified attribute
	//
	// Specify the number of vertices (vertCount), and the dimension of the 
	// vector (vecDim)
	void SetAttribute(ShaderAttrib_t attrib, int vertCount, int vecDim, const float* data);

	// Sets the shader uniform

	void SetUniform1f(ShaderUniform_t uniform, float v0);
	void SetUniform2f(ShaderUniform_t uniform, float v0, float v1);
	void SetUniform3f(ShaderUniform_t uniform, float v0, float v1, float v2);
	void SetUniform4f(ShaderUniform_t uniform, float v0, float v1, float v2, float v3);

	void SetUniformVec2f(ShaderUniform_t uniform, const Vec2& vec);
	void SetUniformVec3f(ShaderUniform_t uniform, const Vec3& vec);

	void SetUniformMat2f(ShaderUniform_t uniform, const Mat2& mat);
	void SetUniformMat3f(ShaderUniform_t uniform, const Mat3& mat);

	void SetUniformTexture2D(ShaderUniform_t uniform, const Texture2D& tex);

protected:
	// Gets the location of the uniform with
	GLint GetUniformLocation(ShaderUniform_t uniform);


	// Converts the values in the vector or matrix to a float array
	// Converted values are stored in m_TmpValBuf
	void ConvertVec2(const Vec2& vec);
	void ConvertVec3(const Vec3& vec);
	void ConvertMat2(const Mat2& mat);
	void ConvertMat3(const Mat3& mat);

	// Resets the values in the temporary value buffer
	void ResetTmpValBuf();

protected:
	GLuint m_Program;
	GLuint m_VertexShader;
	GLuint m_FragmentShader;

	GLuint m_Vao;

	GLuint m_PositionBuffer;
	GLuint m_TexcoordBuffer;
	GLuint m_ColorBuffer;

	// GL location of the attributes
	// Equals -1 if the shader does not use the attribute
	GLint m_PositionLoc;
	GLint m_TexcoordLoc;
	GLint m_ColorLoc;

	// Buffer to contain temporary values of a uniform
	float m_TmpValBuf[16];
};


#endif