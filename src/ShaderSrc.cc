#include "ShaderSrc.h"

const char* ShaderSrc::basicVsh = R"shader_src(
#version 330 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec3 color;

uniform mat4 mvp;

out vec4 baseColor;

void main() {
	gl_Position = mvp * vec4(pos, 0, 1);
	baseColor = vec4(color, 1.0);
}

)shader_src";


const char* ShaderSrc::basicFsh = R"shader_src(
#version 330 core
precision mediump float;

in vec4 baseColor;
out vec4 color;

void main() {
	color = baseColor;
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