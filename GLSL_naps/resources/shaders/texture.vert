#version 130

in vec4 position;
in vec3 normal;
in vec2 texCoord;

uniform mat4 pvm;
uniform mat4 vm;
uniform mat3 normalMat;
uniform vec3 lightPos;

out vec3 lightDirV;
out vec4 posV;
out vec3 normalV;
out vec3 eyeV;
out vec2 texCoordV; 

void main() {
	normalV		= normalize(normalMat * normal);
	eyeV		= normalize(vec3(- vm * position));
	lightDirV	= normalize(vec3(lightPos + eyeV));
	texCoordV	= texCoord;
	
	gl_Position = pvm * position;
}