// render texture and bloom map
uniform sampler2D tex, bloom;

// Control exposure with this value
uniform float exposure;

// How much bloom to add
uniform float bloomFactor;

// Max bright
uniform float brightThreshold;

void main()
{
	vec2 st = gl_TexCoord[0].st;
	vec4 color = texture2D(tex, st);
	vec4 colorBloom = texture2D(bloom, st);
	
	// Add bloom to the image
	color += colorBloom * bloomFactor;

	// Perform tone-mapping
	float Y = dot(vec4(0.30, 0.59, 0.11, 0.0), color);
	float YD = exposure * (exposure/brightThreshold + 1.0) / (exposure + 1.0);
	color *= YD;
	
	gl_FragColor = color;
}
