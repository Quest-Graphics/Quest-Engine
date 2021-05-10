#version 440 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 texCoords;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 objColor;

void main() {
	gl_Position = Projection * View * Model * vec4(vertexPosition, 1.0);
}