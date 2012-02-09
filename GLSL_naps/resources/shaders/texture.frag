#version 130

in vec3 lightDirV;
in vec3 normalV;
in vec3 eyeV;
in vec2 texCoordV;

uniform vec3 diffuse;
uniform float shininess;
uniform vec3 lightDir;
uniform float lightRadius;

uniform sampler2D texDay;
uniform sampler2D texNight;
uniform sampler2D texClouds;
uniform sampler2D texSpec;

out vec4 color;

void main() {
	vec3 n		= normalize(normalV);
	vec3 eye	= normalize(eyeV);
	vec3 h		= normalize(lightDirV);
	
	float i = 0, is = 0;
	float dotProd = dot(n, h);
	if (lightRadius == lightRadius || dot(normalize(lightDir), - lightDirV) < cos(lightRadius)) {
		i	= max(0.0, dot(n, lightDirV));
		is	= max(0.0, dotProd);
	}

	vec4 colorD = texture(texDay, texCoordV);
	vec4 colorN = texture(texNight, texCoordV);
	float cloud	= texture(texClouds, texCoordV).r;
	float spec	= texture(texSpec, texCoordV).r;

	//

	if (dotProd > 0.2)
		color = colorD;
	else if (dotProd < -0.2)
		color = colorN;
	else
		color = mix(colorN, colorD, (dotProd+0.2)*2.5);

	//color = color * cloud;

	color = color + pow(is, 100) * (spec) + vec4(cloud);
	//vec4((i + 0.25) * diffuse + pow(is, shininess) * specular);
}