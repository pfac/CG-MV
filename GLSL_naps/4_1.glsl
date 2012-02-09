
[Vertex Shader]
// Shader OpenGL ID 2
#version 130

in vec4 position;
in vec3 normal;

uniform mat4 pvm;
uniform mat4 vm;
uniform mat3 normalMat;
uniform vec3 lightPos;

out vec3 lightDirV;
out vec4 posV;
out vec3 normalV;
out vec3 eyeV;

void main() {
	normalV		= normalize(normalMat * normal);
	eyeV		= normalize(vec3(- vm * position));
	lightDirV	= normalize(vec3(lightPos + eyeV));
	
	gl_Position = pvm * position;
}
[Fragment Shader]
// Shader OpenGL ID 3
#version 130

in vec3 lightDirV;
in vec3 normalV;
in vec3 eyeV;

uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;
uniform vec3 lightDir;
uniform float lightRadius;

out vec3 color;

void main() {
	vec3 n		= normalize(normalV);
	vec3 eye	= normalize(eyeV);
	vec3 h		= normalize(eye + lightDirV);
	
	float i = 0, is = 0;

	if (lightRadius == lightRadius || dot(normalize(lightDir), - lightDirV) < cos(lightRadius)) {
		i	= max(0.0, dot(n, lightDirV));
		is	= max(0.0, dot(n, h));
	}

	color = (i + 0.25) * diffuse + pow(is, shininess) * specular;
  color.r = 0.5;
}