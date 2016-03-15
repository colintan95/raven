#ifndef SHADER_SCREENCOLORED_H_
#define SHADER_SCREENCOLORED_H_

#include "base_include.h"

const char* const kShaderScreenColoredVS = STRINGIFY(

attribute vec2 v_Position;
attribute vec4 v_Color;

varying vec4 vf_Color;

void main() {
	gl_Position = vec4(v_Position, 0.0, 1.0);

	vf_Color = v_Color;
}

);

const char* const kShaderScreenColoredFS = STRINGIFY(

varying vec4 vf_Color;

void main() {
	gl_FragColor = vf_Color;
}

);

#endif