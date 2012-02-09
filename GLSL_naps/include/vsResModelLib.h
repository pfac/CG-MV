/** ----------------------------------------------------------
 * \class VSResModelLib
 *
 * Lighthouse3D
 *
 * VSResModelLib - Very Simple Resource Model Library
 *
 * \version 0.1.0
 * Initial Release
 *
 * This lib provides an interface for Assimp to load and render 3D models
 *  and performs simple resource managment 
 *
 * This lib requires the following classes from VSL:
 * (http://www.lighthouse3d.com/very-simple-libs)
 *
 * VSResourceLib
 * VSMathLib 
 * VSLogLib
 * VSShaderLib
 *
 * and the following third party libs:
 *
 * GLEW (http://glew.sourceforge.net/),
 * Assimp (http://assimp.sourceforge.net/)
 *
 * Full documentation at 
 * http://www.lighthouse3d.com/very-simple-libs
 *
 ---------------------------------------------------------------*/

 
#ifndef __VSResModelLib__
#define __VSResModelLib__

#ifdef _WIN32
#pragma comment(lib,"assimp.lib")
#endif


#include <string>
#include <vector>
#include <map>
#include <fstream>

#include <GL/glew.h>

// Assimp include files. These three are usually needed.
#include <assimp/assimp.hpp>	
#include <assimp/aiPostProcess.h>
#include <assimp/aiScene.h>

#include "vsResourceLib.h"


class VSResModelLib : public VSResourceLib{

public:

	VSResModelLib();
	~VSResModelLib();

	/** implementation of the superclass abstract method
	  * \param filename the model's filename
	*/
	virtual bool load(std::string filename);
	/// implementation of the superclass abstract method
	virtual void render();

virtual void addTexture(unsigned int unit, std::string filename);
protected:

	// A model can be made of many meshes. Each is stored
	// in the following structure
	struct MyMesh{
		GLuint vao;
		GLuint texUnits[MAX_TEXTURES];
		GLuint uniformBlockIndex;
		float transform[16];
		int numIndices;
		unsigned int type;
		struct Material mat;
	};

	struct HalfEdge {
		GLuint vertex;
		struct HalfEdge *next;
		struct HalfEdge *twin;
	};

	/// the mesh collection
	std::vector<struct MyMesh> mMyMeshes;
	
private:
	/// aux pre processed mesh collection
	std::vector<struct MyMesh> pMyMeshesAux;

	// the global Assimp scene object
	const aiScene* pScene;

	bool pUseAdjacency;

	// images / texture
	// map image filenames to textureIds
	// pointer to texture Array
	std::map<std::string, GLuint> pTextureIdMap;	

	// AUX FUNCTIONS
	bool loadTextures(const aiScene *scene, std::string prefix);
	void genVAOsAndUniformBuffer(const struct aiScene *sc);
	void recursive_walk_for_matrices(const struct aiScene *sc, 
						const struct aiNode* nd);

	void get_bounding_box_for_node (const struct aiNode* nd, 
						struct aiVector3D* min, 
						struct aiVector3D* max);

	void color4_to_float4(const struct aiColor4D *c, float f[4]);
	void set_float4(float f[4], float a, float b, float c, float d);

};

#endif
