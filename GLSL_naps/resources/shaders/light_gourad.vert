#version 130

in vec4 position;
in vec3 normal;

uniform vec3 lightDir;
uniform vec3 diffuse;
uniform float shininess;
uniform mat4 pvm;
uniform mat4 vm;
uniform mat3 normalMat;

out vec3 colorV;

void main() {
	vec3 n		= normalize(normalMat * normal);
	vec3 l		= normalize(normalMat * lightDir);
	float i		= max(0.0, dot(n, l));
	vec3 eye	= normalize(vec3(- vm * position));
	vec3 h		= normalize(eye + l);
	float is	= max(0.0, dot(n, h));
	
	colorV = (i + 0.25) * diffuse + pow(is, shininess) * specular;
	gl_Position = pvm  * position;
}