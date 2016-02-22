#ifndef SHADER_SCREENTEXTURED_H_
#define SHADER_SCREENTEXTURED_H_

#include "base_include.h"

const char* const kShaderScreenTexturedVS = STRINGIFY(

in vec3 v_Position;
in vec2 v_Texcoord;

varying vec2 vf_Texcoord;

void main() {
	gl_Position = v_Position;

	vf_Texcoord = v_Texcoord;
}

);

const char* const kShaderScreenTexturedFS = STRINGIFY(

varying vec2 vf_Texcoord;

uniform sampler2D u_Texture2D;

void main() {
	gl_FragColor = texture(u_Texture2D, vf_Texcoord);
}

);

#endif