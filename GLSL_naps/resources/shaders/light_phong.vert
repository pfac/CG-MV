#version 130

in vec4 position;
in vec3 normal;

uniform mat4 pvm;
uniform mat4 vm;
uniform mat3 normalMat;
uniform vec3 lightPos;

out vec3 lightDirV;
out vec3 normalV;
out vec3 eyeV;

void main() {
	normalV		= normalize(normalMat * normal);
	//lightDirV	= normalize(normalMat * lightDir);
	eyeV		= normalize(vec3(- vm * position));
	lightDirV	= normalize(vec3(lightPos + eyeV));
	//lightDirV	= normalize(normalMat * lightPos);
	gl_Position = pvm * position;
}