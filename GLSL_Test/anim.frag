#version 330

//in float intensity;
/*in vec3 normalV;

out vec4 outColor;

uniform vec3 lightDir;

void main() {
	float intensity = dot(normalize(normalV), lightDir);

	vec4 color;
	if (intensity > 0.9)
		color = vec4(0.8);
	else if (intensity > 0.6)
		color = vec4(0.5);
	else if (intensity > 0.3)
		color = vec4(0.2);
	else
		color = vec4(0.1);

	outColor = color;
}*/

in vec3 lightDir,normal;
out vec4 vecColor;

void main()
{

	float intensity;
	vec4 color;
	
	// normalizing the lights position to be on the safe side
	
	vec3 n = normalize(normal);
	
	intensity = dot(lightDir,n);
	
	if (intensity > 0.95)
		color = vec4(1.0,0.5,0.5,1.0);
	else if (intensity > 0.5)
		color = vec4(0.6,0.3,0.3,1.0);
	else if (intensity > 0.25)
		color = vec4(0.4,0.2,0.2,1.0);
	else
		color = vec4(0.2,0.1,0.1,1.0);
	
	vecColor = color;
} 