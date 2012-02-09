#ifndef __TRI_MESH_H__
#define __TRI_MESH_H__

#include "std.h"
#include "Vector3.h"

class TriMesh
{
public:
	struct Vertex
	{
		Vector3 p;
		Vector3 n;
		float u, v;
	};

public:
	TriMesh();
	~TriMesh();

	// Number of vertices in the mesh
	uint32 getNumVertices() const { return nbVertex; }

	// Access to one vertex
	const Vertex& getVertex(int i) const { return pVertices[i]; }

	// Read the mesh from .ms3d (milkshape) file
	bool loadFromMs3d(const char *filename);
	
private:
	uint32 nbVertex;
	Vertex* pVertices;
};

#endif // __TRI_MESH_H__
