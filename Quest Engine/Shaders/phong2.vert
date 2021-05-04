#version 440 core
out vec3 N;
out vec3 L;
out vec3 E;
out vec3 H;
out vec2 texcoordInterpolated;
out vec3 tangentInterpolated;
 
layout(location=0) in vec4 vertexPosition;
layout(location=1) in vec3 vertexNormal;
layout(location=2) in vec2 texcoord;
layout(location=3) in vec3 tangent;

uniform vec4 lightPosition;
uniform mat4 Projection;
uniform mat4 ModelView;

void main()
{
    gl_Position = Projection * ModelView * vertexPosition;

    vec4 eyePosition = ModelView * vertexPosition;
    vec4 eyeLightPos = lightPosition;

    N = normalize(ModelView * vec4(vertexNormal,0.0)).xyz;
    tangentInterpolated = (ModelView * vec4(tangent, 0.0)).xyz;
    L = normalize(eyeLightPos.xyz - eyePosition.xyz);
    E = -normalize(eyePosition.xyz);
    H = normalize(L + E);

	texcoordInterpolated = texcoord;

   
}

