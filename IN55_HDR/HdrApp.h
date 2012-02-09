/**
 * Classe principal da app
 */

#ifndef __HDR_APP_
#define __HDR_APP_

#include "OpenGLApp.h"

class HdrApp : public OpenGLApp
{
public:
	HdrApp();
	~HdrApp();

private:
	// Overloaded methods from OpenGLApp
	void init(int w, int h);
	void loadResources();
	void loadOpenGLResources();
	void update(float dt);
	void render();
	void renderLoadingScreen();
	void resize(int w, int h);
	void release();

	// String to display while loading resources
	std::string loadingString;

	// Screen size
	int screenW, screenH;

	// Control the limit between HDR rendering and normal rendering range [0;1]
	float mid;

	// Camera to control view and projection matrix
	Camera *cam;

	// Reset default parameters for effects
	void resetEffect();

	// Render HUD
	void renderText();

	// Render the current mesh with selected effect
	void renderMesh(const OpenGLTexture2D &texture, const OpenGLTextureCube &cubeMap);

	// Render the scene using HDR
	void renderHdr();

	// Render the scene using classic rendering
	void renderRgb();

	// Increase or decrease the value with the key by the offset and clamp it
	void keyIncControlDec(int key, float &val, float offset, float min=0.0f, float max=100.0f);

	// Same as above but the value loop from 0 to max
	bool keyIncControlDecLoop(int key, int &val, int max);

	// Current mesh to draw
	TriMesh *mesh;
	int currentMesh;

	// Render Texture
	OpenGLFBO *rtHdr, *rtRgb, *rtBloom;

	// Cube map in HDR and LDR
	OpenGLTextureCube cubeMapHdr, cubeMapRgb;
	int currentCubeMap;

	// Texture in HDR and LDR
	OpenGLTexture2D textureHdr, textureRgb;

	// Shaders for HDR effect
	OpenGLShader shaderTone;
	OpenGLShader shaderBlur;
	OpenGLShader shaderBloom;

	// Effect shaders for the mesh
	std::vector<std::string> fileShaderEffect;
	std::vector<OpenGLShader> shaderEffect;
	int currentEffect;

	// Effect parameters
	float fresnelBias, fresnelScale, fresnelPower;
	float reflectionFactor, refractionFactor;
	float exposure, exposureControl;
	float bloomFactor;
	float brightThreshold;
	Color matColor;
	Vector3 etaRatio;

	// Down sampler
	DownSampler *downSampler;

	// Font to render text
	OpenGLFont font;

	// Store cube maps and meshs files
	std::vector<std::string> fileCubeMap;
	std::vector<std::string> fileMesh;
};

#endif //__HDR_APP_
