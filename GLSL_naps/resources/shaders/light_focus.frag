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
}