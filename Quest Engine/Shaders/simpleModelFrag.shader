#version 440 core

layout(location = 0) out vec4 color;

uniform vec3 objColor;
uniform sampler2D ourTexture;

in vec2 TexCoord;

void main() {
	color = vec4(objColor, 1.0) * texture(ourTexture, TexCoord);
}
