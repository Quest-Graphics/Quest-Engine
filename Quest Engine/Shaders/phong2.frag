 #version 440 core
 in vec3 N;
 in vec3 L;
 in vec3 E;
 in vec3 H;
 in vec2 texcoordInterpolated;
 in vec3 tangentInterpolated;
 uniform mat4 ModelView;
 uniform vec4 lightDiffuse;
 uniform vec4 lightSpecular;
 uniform vec4 lightAmbient;

 uniform vec4 surfaceDiffuse;
 uniform vec4 surfaceSpecular;
 uniform float shininess;
 uniform vec4 surfaceAmbient;
 uniform vec4  surfaceEmissive;
 uniform sampler2D metaltex;
 uniform sampler2D normaltex;

 
vec3 CalcBumpedNormal()
{
    vec3 Normal = N;
    vec3 Tangent = normalize(tangentInterpolated);
    Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);
    vec3 Bitangent = cross(Tangent, Normal);
    vec3 BumpMapNormal = texture2D(normaltex, texcoordInterpolated).xyz;
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);
    vec3 NewNormal;
    mat3 TBN = mat3(Tangent, Bitangent, Normal);
    //TBN=transpose(TBN); // in general you want to include this but it doesnt work in glsl 1.1, which is the version in the labs
    NewNormal = TBN * BumpMapNormal;
    NewNormal = normalize(NewNormal);
    return NewNormal;
}
void main()
{
	 vec3 Normal;
	 Normal = CalcBumpedNormal();
	 vec3 Light  = normalize(L);
     vec3 Eye    = normalize(E);
     vec3 Half   = normalize(H);
  

    float Kd = max(dot(Normal, Light), 0.0);
    float Ks = pow(max(dot(Half, Normal), 0.0), shininess);
   

    vec4 diffuse  = Kd * lightDiffuse*surfaceDiffuse;
    vec4 specular = Ks * lightSpecular*surfaceSpecular;
    vec4 ambient  = lightAmbient*surfaceAmbient;

    gl_FragColor = (surfaceEmissive + ambient + diffuse + specular)* texture2D(metaltex, texcoordInterpolated);
}
