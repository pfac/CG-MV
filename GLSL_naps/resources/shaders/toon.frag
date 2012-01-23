#version 150

in vec3 normalV;

out vec4 outColor;

uniform mat3 normalMat;
uniform vec3 lightDir;

void main() {
	vec3 l = normalize(normalMat * lightDir);

	float intensity = dot(normalize(normalV), l);

	vec4 color;

	if (intensity > 0.9)
		color = vec4(0.9);
	else if (intensity > 0.5)
		color = vec4(0.6);
	else if (intensity > 0.3)
		color = vec4(0.4);
	else
		color = vec4(0.0);

	outColor = color;
}