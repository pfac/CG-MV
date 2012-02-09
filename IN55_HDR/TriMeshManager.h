#ifndef __TRI_MESH_MANAGER_H__
#define __TRI_MESH_MANAGER_H__

#include "std.h"
#include "TriMesh.h"
#include "Singleton.h"

class TriMeshManager : public Singleton<TriMeshManager>
{
public:
	// Directory containing the meshs
	void setDir(const std::string &path);

	// Get the mesh from the file
	TriMesh* getTriMesh(const std::string &filename);

private:
	TriMeshManager();
	~TriMeshManager();

	std::map<std::string, TriMesh*> triMesh;
	std::map<std::string, TriMesh*>::iterator it;
	std::string dir;

	friend class Singleton<TriMeshManager>;
};

#endif // __TRI_MESH_MANAGER_H__
