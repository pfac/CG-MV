/** ----------------------------------------------------------
 * \class VSResourceLib
 *
 * Lighthouse3D
 *
 * VSResourceLib - Very Simple Resource Library
 *
 * \version 0.1.0
 * Initial Release
 *
 * This abstract class defines an interface 
 * for loading and rendering resources (models)
 *
* This lib requires the following classes from VSL:
 * (http://www.lighthouse3d.com/very-simple-libs)
 *
 * VSMathLib 
 * VSLogLib
 * VSShaderLib
 *
 * and the following third party libs:
 *
 * GLEW (http://glew.sourceforge.net/),
 * DevIL (http://openil.sourceforge.net/)
 *
 * Full documentation at 
 * http://www.lighthouse3d.com/very-simple-libs
 *
 ---------------------------------------------------------------*/

#include "vsResourceLib.h"


VSResourceLib::VSResourceLib(): mScaleToUnitCube(1.0)
{
	// get a pointer to VSMathLib singleton
	mVSML = VSMathLib::getInstance();

	/* initialization of DevIL */
	ilInit(); 
}


VSResourceLib::~VSResourceLib() {

}


// get the scale factor used to fit the model in a unit cube
float
VSResourceLib::getScaleForUnitCube() {

	return mScaleToUnitCube;
}


// Set the shader's material block name
void 
VSResourceLib::setMaterialBlockName(std::string name) {

	mMaterialBlockName = name;
}


// useful to set uniforms inside a named block
void
VSResourceLib::setMaterial(Material &aMat) {

	// use named block
	if (mMaterialBlockName != "" && mMatSemanticMap.size() == 0) {
		VSShaderLib::setBlock(mMaterialBlockName, &aMat);
	}
	// use uniforms in named block
	else if (mMaterialBlockName != "" && mMatSemanticMap.size() != 0) {

		std::map<std::string, MaterialSemantics>::iterator iter;
		for (iter = mMatSemanticMap.begin(); iter != mMatSemanticMap.end(); ++iter) {
			void *value;
			switch ((*iter).second) {
				case DIFFUSE: value = (void *)aMat.diffuse;
					break;
				case AMBIENT: value = (void *)aMat.ambient;
					break;
				case SPECULAR: value = (void *)aMat.specular;
					break;
				case EMISSIVE: value = (void *)aMat.emissive;
					break;
				case SHININESS: value = (void *)&aMat.shininess;
					break;
				case TEX_COUNT: value = (void *)&aMat.texCount;
					break;
			}
			VSShaderLib::setBlockUniform(mMaterialBlockName, 
						(*iter).first, value);
		}
	}
}


void 
VSResourceLib::setUniformSemantics(MaterialSemantics field, std::string name) {

	mMatSemanticMap[name] = field;
}


// Get loading errors
std::string 
VSResourceLib::getErrors() {

	return(mLogError.dumpToString());
}


// get model information
std::string
VSResourceLib::getInfo() {

	return(mLogInfo.dumpToString());
}


// helper function for derived classes
// loads an image and defines an 8-bit RGBA texture
unsigned int
VSResourceLib::loadRGBATexture(std::string filename, 
						bool mipmap, bool compress, 
						GLenum aFilter, GLenum aRepMode) {

	ILboolean success;
	unsigned int imageID;
	GLuint textureID = 0;

	// Load Texture Map
	ilGenImages(1, &imageID); 
	
	ilBindImage(imageID); /* Binding of DevIL image name */
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT); 
	success = ilLoadImage((ILstring)filename.c_str());

	if (!success) {
		VSLOG(mLogError, "Couldn't load texture: %s", 
							filename.c_str());
		// The operation was not sucessfull 
		// hence free image and texture 
		ilDeleteImages(1, &imageID); 
		return 0;
	}
		
	/* Convert image to RGBA */
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE); 
	// Set filters
	GLenum minFilter = aFilter;
	if (aFilter == GL_LINEAR && mipmap) {
		minFilter = GL_LINEAR_MIPMAP_LINEAR;
	}
	else if (aFilter == GL_NEAREST && mipmap){
		minFilter = GL_NEAREST_MIPMAP_LINEAR;
	}
	GLenum type;
	if (compress)
		type = GL_RGBA;
	else
		type = GL_COMPRESSED_RGBA;


	/* Create and load textures to OpenGL */
	glGenTextures(1, &textureID); /* Texture name generation */
	glBindTexture(GL_TEXTURE_2D, textureID); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, aFilter); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, aRepMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, aRepMode);
	glTexImage2D(GL_TEXTURE_2D, 0, type, 
					ilGetInteger(IL_IMAGE_WIDTH),
					ilGetInteger(IL_IMAGE_HEIGHT), 
					0, GL_RGBA, GL_UNSIGNED_BYTE,
					ilGetData()); 

	// Mipmapping?
	if (mipmap)
		glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D,0);

	/* Because we have already copied image data into texture data
	we can release memory used by image. */
	ilDeleteImages(1, &imageID); 

	// add information to the log
	VSLOG(mLogInfo, "Texture Loaded: %s", filename.c_str());

	return textureID;
}
