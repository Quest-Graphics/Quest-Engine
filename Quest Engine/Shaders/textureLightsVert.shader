#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 fragPos;
out vec3 N;
out vec2 TexCoords;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
	fragPos = vec3(Model * vec4(aPos, 1.0));
	N = mat3(transpose(inverse(Model))) * aNormal;

	gl_Position = Projection * View * Model * vec4(aPos, 1.0);
	TexCoords = aTexCoords;
}