//#version 150

/*in vec4 pos;
in vec3 normal;

uniform mat4 pvm;
uniform mat3 normalMat;
//uniform vec3 lightDir;

//out float intensity;
out vec3 normalV;

void main() {
	gl_Position = pvm * position;

	normalV = normalize(normalMat * normal);
	//vec3 aux = normalize(normalMat * normal);
	//intensity = dot(aux, lightdir);
}*/
varying vec3 lightDir,normal;

//out vec3 lightDirV, normalV;

void main()
{
	lightDir = normalize(vec3(gl_LightSource[0].position));
	normal = gl_NormalMatrix * gl_Normal;

	gl_Position = ftransform();
} 