#include "TriMeshManager.h"
#include "AllocOverLoaderOn.h"

TriMeshManager::TriMeshManager()
{
	setDir("media/");
}


TriMeshManager::~TriMeshManager()
{
	// Destruction des mesh
	for (it=triMesh.begin() ; it!=triMesh.end() ; ++it)
		delete it->second;
	triMesh.clear();
}

void TriMeshManager::setDir(const std::string &path)
{
	dir = path;
}

TriMesh* TriMeshManager::getTriMesh(const std::string &filename)
{
	// Mesh already loaded
	it = triMesh.find(filename);
	if (it != triMesh.end())
		return it->second;

	// Create a new mesh from file
	TriMesh *m = new TriMesh();
	m->loadFromMs3d((dir+filename).c_str());
	triMesh[filename] = m;
	return m;
}
