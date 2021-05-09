#version 440 core

layout(location = 0) out vec4 color;

uniform vec3 objColor;

void main() {
	color = vec4(objColor, 1.0);
}
