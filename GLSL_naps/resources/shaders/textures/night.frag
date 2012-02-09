in vec3 normalV;
in vec2 texCoordV;

uniform sampler1D texUnitD;
uniform sampler1D texUnitN;
uniform sampler2D texUnitS;
uniform vec3 lightDir;


void main() {
	vec3 n = normalize(normalV);
	vec3 l = normalize(lightDir);

	corD = texture(texUnitD, texCoordV);
	corN = texture(texUnitN, texCoordV);
	corS = texture(texUnitS, texCoordV).g;

	float dotProd = dot(n, l);
	if (dotProd > 0.1)
		cor = corD;
	else if (dotProd < -0.1)
		cor = corN;
	else
		mix(corD, corN, ...);
}