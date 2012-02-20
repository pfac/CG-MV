// Texture to blur
uniform sampler2D tex;

// Texture size
uniform float w, h;

// 121
// 242
// 121

// 12421
// 24842
// 48X84
// 24842
// 12421

void main()
{
	// calcula o incremento conforme o tamanho da textura
	float dx = 1.0 / w;
	float dy = 1.0 / h;

	// coordenada actual da textura
	vec2 st = gl_TexCoord[0].st;
	
	// Apply 3x3 gaussian filter
	vec4 color	 = 4.0 * texture2D(tex, st);
	color		+= 2.0 * texture2D(tex, st + vec2(+dx, 0.0));
	color		+= 2.0 * texture2D(tex, st + vec2(-dx, 0.0));
	color		+= 2.0 * texture2D(tex, st + vec2(0.0, +dy));
	color		+= 2.0 * texture2D(tex, st + vec2(0.0, -dy));
	color		+= texture2D(tex, st + vec2(+dx, +dy));
	color		+= texture2D(tex, st + vec2(-dx, +dy));
	color		+= texture2D(tex, st + vec2(-dx, -dy));
	color		+= texture2D(tex, st + vec2(+dx, -dy));
	
	// HACK bloom map
	gl_FragColor = color / 16.0;
	//gl_FragColor = texture2D(tex, st);
}