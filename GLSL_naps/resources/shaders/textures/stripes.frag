in vec2 texCoordV;

uniform sampler1D texUnit;

void main() {
	vec2 t = texCoordV * 10;

	float tt = fract(t.t);

	// transi��o bin�ria
	if (tt < 0.5)
		cor = vec4(1, 0, 0, 1);
	else
		cor = vec4(0, 1, 0, 1);

	// mix(cor1, cor2, fract)
	//	combina��o linear das cores
	//	fract * cor1 + (1 - fract) * cor2

	// transi��o suave (YAY anti-aliasing)
	if (scaled_t < 0.025)
		fract1 = 0.5 - 20.0*fract1;
	else if (scaled_t > 0.975)
		fract1 = 0.975 - 20.0(fract1 - 0.975);
	else if (scaled_t > 0.475 && scaled_t < 0.525)
		fract1 = 20.0*(frac1 - 0.475);
	else if (scaled_t > 0.5)
	...
	else
	...

	...
	...


	// alternativamente pode-se usar uma textura 1D, simplificando o processo
	vec4 cor = texture(texUnit, texCoordV);
}