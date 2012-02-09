#include "TriMesh.h"
#include "Matrix4.h"
#include "AllocOverLoaderOn.h"

TriMesh::TriMesh()
{
	nbVertex = 0;
	pVertices = 0;
}

TriMesh::~TriMesh()
{
	if (pVertices)
		delete [] pVertices;
}


bool TriMesh::loadFromMs3d(const char *filename)
{
	uint32 i;
	uint8 tempU8;
	uint16 tempU16;
	float tempF;

	// Try to open the file
	FILE* f = fopen(filename, "rb");
	if (f == 0)
		throw Exception("File missing : " + std::string(filename));

	// Read id and version of the file
	char id[10];
	fread( id, sizeof(char), 10, f);
	int version;
	fread(&version, sizeof(int), 1, f);

	// Read vertex positions
	uint16 n;
	fread(&n, sizeof(uint16), 1, f);
	Vertex *v = new Vertex[n];
	for (i=0 ; i<n ; i++)
	{
		fread(&tempU8, sizeof(tempU8), 1, f);
		fread(&v[i].p, sizeof(float),  3, f);
		fread(&tempU8, sizeof(tempU8), 1, f);
		fread(&tempU8, sizeof(tempU8), 1, f);
	}

	// Read triangles
	uint16 numTris;
	fread(&numTris, sizeof(uint16), 1, f);
	uint32 nbIndex = numTris * 3;
	nbVertex = nbIndex;
	uint16 *pIndices = new uint16[nbIndex];
	pVertices = new Vertex[nbIndex];

	for (i=0 ; i<numTris ; i++)
	{
		// Vertex indices
		fread(&tempU16, sizeof(tempU16), 1, f);
		fread(&pIndices[3*i], sizeof(uint16), 3, f);
		pVertices[3*i+0] = v[pIndices[3*i+0]];
		pVertices[3*i+1] = v[pIndices[3*i+1]];
		pVertices[3*i+2] = v[pIndices[3*i+2]];

		// unused data
		for (int j=0 ; j<9 ; j++)
			fread(&tempF, sizeof(float), 1, f);

		// Texture coordinates
		fread(&pVertices[3*i+0].u, sizeof(float), 1, f);
		fread(&pVertices[3*i+1].u, sizeof(float), 1, f);
		fread(&pVertices[3*i+2].u, sizeof(float), 1, f);
		fread(&pVertices[3*i+0].v, sizeof(float), 1, f);
		fread(&pVertices[3*i+1].v, sizeof(float), 1, f);
		fread(&pVertices[3*i+2].v, sizeof(float), 1, f);

		// unused data
		fread(&tempU8, sizeof(tempU8), 1, f);
		fread(&tempU8, sizeof(tempU8), 1, f);
	}

	// Close the file
	fclose(f);
	
	// correct texture coordinates
	for (i=0 ; i<nbVertex ; i++)
		pVertices[i].v = 1.0f - pVertices[i].v;

	// Zeroing vertex normals
	Vector3 *faceNormal = new Vector3[numTris];
	for (i=0 ; i<n ; i++)
		v[i].n = Vector3::ZERO;

	// Calculate normals for each face
	for (i=0 ; i<numTris ; i++)
	{
		Vector3 A = v[pIndices[3*i+0]].p;
		Vector3 B = v[pIndices[3*i+1]].p;
		Vector3 C = v[pIndices[3*i+2]].p;

		Vector3 AB = B - A;
		Vector3 AC = C - A;
		Vector3 N = vec3Cross(AB, AC);
		N.normalize();

		faceNormal[i] = N;

		// These vertices belong to the triangle
		v[pIndices[3*i+0]].n += N;
		v[pIndices[3*i+1]].n += N;
		v[pIndices[3*i+2]].n += N;
	}

	// Normalizing vertex normals
	for (i=0 ; i<n ; i++)
		v[i].n.normalize();

	// Assign normals
	for(i=0 ; i<nbIndex ; i++)
		pVertices[i].n = v[pIndices[i]].n;

	// Release temp memory
	delete [] faceNormal;
	delete [] pIndices;
	delete [] v;

	return true;
}
