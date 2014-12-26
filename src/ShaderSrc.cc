#include "ShaderSrc.h"

const char* ShaderSrc::cubeVsh = R"shader_src(
#version 330 core

layout(location = 0) in vec3 v_pos;

uniform mat4 _model, _view, _proj;

out vec4 vertColor;

void main() {
	gl_Position = _proj * _view * _model * vec4(v_pos, 1);
	vertColor = vec4(clamp(v_pos, 0, 1), 1.0);
}

)shader_src";


const char* ShaderSrc::cubeFsh = R"shader_src(
#version 330 core
precision mediump float;

in vec4 vertColor;
out vec4 frag_color;

void main() {
	frag_color = vertColor;
}

)shader_src";


const char* ShaderSrc::surfaceVsh = R"shader_src(
#version 330 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 texCoord;

out vec2 uv;

uniform mat4 mvp;

void main() {
	gl_Position = mvp * vec4(pos, 0, 1);
	uv = texCoord;
}
)shader_src";

const char* ShaderSrc::surfaceFsh = R"shader_src(
#version 330 core
precision mediump float;

in vec2 uv;
out vec4 color;

uniform sampler2D tex;
uniform float alpha;

void main() {
	color = vec4(texture(tex, uv).rgb, alpha);
	//color = vec4(uv.y, uv.x, 0.0, alpha);
}
)shader_src";