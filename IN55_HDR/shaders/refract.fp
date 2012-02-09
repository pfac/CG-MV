// Cube map
uniform samplerCube env;

// Refracted vector
varying vec3 R;

void main()
{
	vec4 color = textureCube(env, R);
	color.a = 1.0;
	gl_FragColor = color;
}