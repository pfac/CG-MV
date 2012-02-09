/**
 * REFLEX Fragment Shader
 * Dado vector de reflexão calculado no Vertex Shader,
 * mix entre a cor do material do objecto e a cor do CubeMap no ponto apontado pelo vector de Reflexão
 */

// Decal texture
uniform sampler2D tex;

// Cube map
uniform samplerCube env;

// Reflection factor
uniform float reflectionFactor;

// Reflected vector
varying vec3 R;

void main()
{
	// Mix reflection and texture
	vec4 color = mix(texture2D(tex, gl_TexCoord[0].st), textureCube(env, R), reflectionFactor);
	color.a = 1.0;
	gl_FragColor = color;
}