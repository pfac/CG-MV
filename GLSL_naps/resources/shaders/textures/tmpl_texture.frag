in vec2 texCoord2;

uniform sampler2D texUnit;

void main() {
	// aceder a cor da textura na coordenada dada
	//vec4 color = texture(texUnit, texCoordV);

	// aceder a um vec4:
	// a[1]
	// a.a	{rgba} cor
	// a.r
	// a.g
	// a.b
	// a.x	{xyzw} coordenadas
	// a.t	{stpq} texturas
	// a.s

	vec2 t = texCoordV * 10;
	float ts = fract(t.s);
	float tt = fract(t.t);

	if (ts > 0.1 && tt > 0.1)
		discard; // uma espécie de break. nao processa mais este pixel (eeeeiiissh returns no meio do codigo, mm not like it)


}