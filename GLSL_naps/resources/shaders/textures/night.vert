in vec3 normal;
in vec2 texCoord;

out vec2 texCoordV;
out vec3 normalV;

void main() {
	texCoordV = texCoord;
	normalV = normalize(normalMat * normal);
}