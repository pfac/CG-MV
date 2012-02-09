// Cube map
uniform samplerCube env;

// Reflected and refracted vectors
varying vec3 R, TRed, TGreen, TBlue;

// Reflection factor based on fresnel equation
varying float refFactor;

void main()
{
	// Read cube map for reflected color
	vec4 reflectedColor = textureCube(env, R);
	
	// Read cube map for refracted color (3 times)
	vec4 refractedColor;
	refractedColor.r = textureCube(env, TRed).r;
	refractedColor.g = textureCube(env, TGreen).g;
	refractedColor.b = textureCube(env, TBlue).b;
	refractedColor.a = 1.0;
	
	// Mix reflected and refracted colors
	gl_FragColor = mix(refractedColor, reflectedColor, refFactor);
}