#ifndef SHADER_SCREENTEXTURED_H_
#define SHADER_SCREENTEXTURED_H_

#include "base_include.h"

const char* const kShaderScreenTexturedVS = STRINGIFY(

attribute vec2 v_Position;
attribute vec2 v_Texcoord;

varying vec2 vf_Texcoord;

void main() {
	gl_Position = vec4(v_Position, 0.0, 1.0);

	vf_Texcoord = v_Texcoord;
}

);

const char* const kShaderScreenTexturedFS = STRINGIFY(

varying vec2 vf_Texcoord;

uniform sampler2D u_Texture2D;

void main() {
	gl_FragColor = texture2D(u_Texture2D, vf_Texcoord);
}

);

#endif