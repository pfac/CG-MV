#version 130

in vec4 position;
in vec3 normal;

out vec3 normalV;

uniform mat4 pvm;
uniform mat3 normalMat;
uniform vec3 lightDir;

void main() {
	// normalize both vectors
	vec3 objNormal		= normalize(normalMat * normal);
	vec3 lightNormal	= normalize(normalMat * lightDir);

	normalV = normalize(normalMat * normal);

	gl_Position = pvm * position;
}