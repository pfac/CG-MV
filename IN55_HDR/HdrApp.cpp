#include "HdrApp.h"
#include "SDLException.h"
#include "AllocOverLoaderOn.h"

HdrApp::HdrApp()
{
	// Path to cube maps
	fileCubeMap.push_back("uffizi_cross.hdr");
	fileCubeMap.push_back("building_cross.hdr");
	fileCubeMap.push_back("stpeters_cross.hdr");
	fileCubeMap.push_back("campus_cross.hdr");
	fileCubeMap.push_back("grace_cross.hdr");
	fileCubeMap.push_back("kitchen_cross.hdr");
	fileCubeMap.push_back("beach_cross.hdr");
	fileCubeMap.push_back("galileo_cross.hdr");
	fileCubeMap.push_back("rnl_cross.hdr");
	currentCubeMap = 0;

	// Path to mesh
	fileMesh.push_back("teapot.ms3d");
	fileMesh.push_back("eye.ms3d");
	fileMesh.push_back("ming.ms3d");
	fileMesh.push_back("glass.ms3d");
	fileMesh.push_back("spoon.ms3d");
	fileMesh.push_back("car1.ms3d");
	fileMesh.push_back("pottery.ms3d");
	fileMesh.push_back("small_glass.ms3d");
	fileMesh.push_back("car2.ms3d");
	currentMesh = 0;
	mesh = TriMeshManager::get()->getTriMesh(fileMesh[currentMesh]);

	// Path to effect shaders
	fileShaderEffect.push_back("reflect");
	fileShaderEffect.push_back("reflectTex");
	fileShaderEffect.push_back("refract");
	fileShaderEffect.push_back("fresnel");
	fileShaderEffect.push_back("chromaticDispersion");
	shaderEffect.resize(fileShaderEffect.size());
	resetEffect();
}

HdrApp::~HdrApp()
{
	delete cam;
}

void HdrApp::init(int w, int h)
{
	// Set title
	setTitle("2006 - UTBM - IN55 - High Dynamic Range in OpenGL - Fabien Houlmann and Stephane Metz");

	// Create the camera
	cam = new Camera(screenW = w, screenH = h, getRenderer());
	cam->setPerspective(45.0f, 0.1f, 1000.0f);

	// Init renderer
	getRenderer()->setClearColor(Color(0.2f, 0.3f, 0.4f));

	// Create the font
	font.init(w, h);

	// Mid point
	cam->setMid(mid = 0.5f);
}

void HdrApp::loadResources()
{
	// Preload all cube maps
	for (size_t i=0 ; i<fileCubeMap.size() ; ++i)
	{
		loadingString = "image : " + fileCubeMap[i];
		ImageManager::get()->extractCubeMap(fileCubeMap[i]);
	}

	// Preload all meshs
	for (size_t i=0 ; i<fileMesh.size() ; ++i)
	{
		loadingString = "image : " + fileMesh[i];
		TriMeshManager::get()->getTriMesh(fileMesh[i]);
	}

	// Load texture images
	ImageManager::get()->getImage("pro.bmp");

	loadingString = "creating OpenGL resources ...";
}

void HdrApp::loadOpenGLResources()
{
	// Screen size
	int w = screenW;
	int h = screenH;

	// Reset clear color to black
	getRenderer()->setClearColor(Color::BLACK);

	// Create the cube map texture
	cubeMapHdr.load(fileCubeMap.at(currentCubeMap), GL_RGBA16F_ARB);
	cubeMapRgb.load(fileCubeMap.at(currentCubeMap), GL_RGBA);

	// Create render texture HDR
	rtHdr = new OpenGLFBO();
	rtHdr->init(w, h, GL_RGBA16F_ARB);

	// Create render texture RGB
	rtRgb = new OpenGLFBO();
	rtRgb->init(w, h, GL_RGBA);

	// Create bloom render texture
	rtBloom = new OpenGLFBO();
	rtBloom->init(w, h, GL_RGBA16F_ARB);

	// Load textures
	textureHdr.load("pro.bmp", GL_RGBA16F_ARB);
	textureRgb.load("pro.bmp", GL_RGBA);

	// Load shaders
	shaderTone.load("tone.vp", "tone.fp");
	shaderBlur.load("blur.vp", "blur.fp");
	shaderBloom.load("extractBloom.vp", "extractBloom.fp");

	// Load effect shaders
	for (size_t i=0 ; i<fileShaderEffect.size() ; ++i)
	{
		std::string base = fileShaderEffect.at(i);
		shaderEffect.at(i).load(base+".vp", base+".fp");
	}

	// Create the down sampler
	downSampler = new DownSampler();
	downSampler->init(w, h, GL_RGBA16F_ARB);
}

void HdrApp::update(float dt)
{
	// Quit application
	if (isKeyDown(SDLK_ESCAPE))
		requestQuit();

	// Camera zoom in and out
	if (isButtonDown(2))
		cam->zoom(getMouseRelY() * dt * 10.0f);

	// Camera rotate
	if (isButtonDown(0))
	{
		cam->rotateX(getMouseRelX()*dt*10.0f);
		cam->rotateY(getMouseRelY()*dt*10.0f);		
	}

	// Change mid point
	if (isKeyDown(SDLK_LEFT))
		cam->setMid(mid = Math::clamp(mid-0.01f, 0.0f, 1.0f));
	if (isKeyDown(SDLK_RIGHT))
		cam->setMid(mid = Math::clamp(mid+0.01f, 0.0f, 1.0f));

	// Reset effect parameters
	if (isKeyJustDown(SDLK_SPACE))
		resetEffect();

	// Exposure control
	keyIncControlDec(SDLK_e, exposureControl, 0.01f, 0.1f, 3.0f);
	exposure = Math::exp(exposureControl*exposureControl) - 1.0f;

	// Bloom factor
	keyIncControlDec(SDLK_o, bloomFactor, 0.01f, 0.0f, 1.0f);

	// Brightness threshold
	keyIncControlDec(SDLK_t, brightThreshold, 0.05f);

	// Reflection factor
	keyIncControlDec(SDLK_r, reflectionFactor, 0.01f, 0.0f, 1.0f);

	// Change mesh
	if (keyIncControlDecLoop(SDLK_m, currentMesh, (int)fileMesh.size()))
	{
		// If textured effect : block change to textured mesh
		if (currentEffect == 1 && currentMesh != 1)
				currentMesh = 1;
		mesh = TriMeshManager::get()->getTriMesh(fileMesh[currentMesh]);
	}

	// Change cube map
	if (keyIncControlDecLoop(SDLK_c, currentCubeMap, (int)fileCubeMap.size()))
	{
		cubeMapHdr.release();
		cubeMapHdr.load(fileCubeMap.at(currentCubeMap), GL_RGBA16F_ARB);

		cubeMapRgb.release();
		cubeMapRgb.load(fileCubeMap.at(currentCubeMap), GL_RGBA);
	}

	// Change shader effect
	if (keyIncControlDecLoop(SDLK_h, currentEffect, (int)shaderEffect.size()))
	{
		// If textured effect : block change to textured mesh (1 & 2)
		if (currentEffect == 1 && currentMesh != 1)
		{
			currentMesh = 1;
			mesh = TriMeshManager::get()->getTriMesh(fileMesh[currentMesh]);
		}
	}

	// Change fresnel parameters
	keyIncControlDec(SDLK_b, fresnelBias, 0.05f);
	keyIncControlDec(SDLK_s, fresnelScale, 0.05f);
	keyIncControlDec(SDLK_p, fresnelPower, 0.05f);

	// Change eta ratio
	keyIncControlDec(SDLK_5, etaRatio.x, 0.01f);
	keyIncControlDec(SDLK_6, etaRatio.y, 0.01f);
	keyIncControlDec(SDLK_7, etaRatio.z, 0.01f);
	keyIncControlDec(SDLK_a, etaRatio.x, 0.01f);
	keyIncControlDec(SDLK_a, etaRatio.y, 0.01f);
	keyIncControlDec(SDLK_a, etaRatio.z, 0.01f);

	// Change material color
	keyIncControlDec(SDLK_1, matColor.r, 0.01f);
	keyIncControlDec(SDLK_2, matColor.g, 0.01f);
	keyIncControlDec(SDLK_3, matColor.b, 0.01f);
}

void HdrApp::renderLoadingScreen()
{
	getRenderer()->clearBuffer();
	font.addText(2, screenH-20, Color::WHITE, "<<< LOADING >>>");
	font.addText(2, screenH-40, Color::WHITE, loadingString.c_str());
	font.render();
}

void HdrApp::renderMesh(const OpenGLTexture2D &texture, const OpenGLTextureCube &cubeMap)
{
	// Shader to use
	const OpenGLShader &shader = shaderEffect.at(currentEffect);

	glActiveTexture(GL_TEXTURE0);
	texture.bind();
	glActiveTexture(GL_TEXTURE1);
	cubeMap.bind();

	shader.begin();
	shader.setUniform("env", 1);
	shader.setUniform("tex", 0);
	shader.setUniform("reflectionFactor", reflectionFactor);
	shader.setUniform("fresnelBias", fresnelBias);
	shader.setUniform("fresnelScale", fresnelScale);
	shader.setUniform("fresnelPower", fresnelPower);
	shader.setUniform("etaRatio", etaRatio.x);
	shader.setUniform("eyePos", cam->getPos());
	shader.setUniform("etaRatioRGB", etaRatio);
	shader.setUniform("matColor", matColor);

	getRenderer()->renderTriMesh(mesh);
	OpenGLShader::end();
	glActiveTexture(GL_TEXTURE0);
}

void HdrApp::render()
{
	// Screen size
	int w = screenW;
	int h = screenH;

	// Render scene in HDR and RGB on separate render texture
	renderHdr();
	renderRgb();

	// Render to real framebuffer
	getRenderer()->clearBuffer();

	// HACK split-screen
	// left side -> LDR
	// right side -> HDR

	// Draw RGB render texture on left part of the screen
	rtRgb->bind();
	cam->initQuad(w, h);
	cam->drawQuadLeft();

	// Draw HDR render texture on right part of the screen
	rtHdr->bind();
	cam->initQuad(w, h);
	cam->drawQuadRight();

	// Draw text
	renderText();
}

void HdrApp::renderHdr()
{
	// Screen size
	int w = screenW;
	int h = screenH;

	// Render skybox to HDR render texture
	rtHdr->beginCapture();
		getRenderer()->clearBuffer();
		cam->useRight();
		getRenderer()->renderSkyBox(cubeMapHdr);
		renderMesh(textureHdr, cubeMapHdr);
	rtHdr->endCapture();

	// Extract very bright areas
	rtBloom->beginCapture();
		getRenderer()->clearBuffer();
		cam->initQuad(w, h);
		shaderBloom.begin();
			shaderBloom.setUniform("tex", 0);
			shaderBloom.setUniform("brightThreshold", brightThreshold);
			rtHdr->bind();
			cam->drawQuad();
		shaderBloom.end();
	rtBloom->endCapture();

	// Downsampled 4 times to blur
	downSampler->update(rtBloom, &shaderBlur, 4);

	// Add the four blurred render texture with additive blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	rtBloom->beginCapture();
		getRenderer()->clearBuffer();
		cam->initQuad(w, h);

		downSampler->getRenderTexture(0)->bind();
		cam->drawQuad();

		downSampler->getRenderTexture(1)->bind();
		cam->drawQuad();

		downSampler->getRenderTexture(2)->bind();
		cam->drawQuad();

		downSampler->getRenderTexture(3)->bind();
		cam->drawQuad();

	rtBloom->endCapture();
	glDisable(GL_BLEND);

	// Compose the final image with original and blurred texture and with appropriate exposure
	rtHdr->beginCapture();
		glActiveTexture(GL_TEXTURE0);
		rtHdr->bind();
		glActiveTexture(GL_TEXTURE1);
		rtBloom->bind();
		shaderTone.begin();
		shaderTone.setUniform("brightThreshold", brightThreshold);
		shaderTone.setUniform("bloomFactor", bloomFactor);
		shaderTone.setUniform("exposure", exposure);
		shaderTone.setUniform("tex", 0);
		shaderTone.setUniform("bloom", 1);
		cam->initQuad(w, h);
		cam->drawQuad();
		shaderTone.end();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
	rtHdr->endCapture();
}

void HdrApp::renderRgb()
{
	// Render the skybow and the mesh
	rtRgb->beginCapture();
		getRenderer()->clearBuffer();
		cam->useLeft();
		getRenderer()->renderSkyBox(cubeMapRgb);
		renderMesh(textureRgb, cubeMapRgb);
	rtRgb->endCapture();
}

void HdrApp::renderText()
{
	// General informations
	Color c = Color::WHITE;
	font.addText(2, screenH-20, c, "OpenGL High Dynamic Range demo");
	font.addText(screenW-120, screenH-20, c, "Fps: %.1f", getAvgFps());

	// Selection informations
	c = Color(1.0f, 1.0f, 0.0f);
	font.addText(2, 2, c, "Press key to increase and Ctrl+key to decrease value.");
	font.addText(2, 22, c, "(m) Mesh: %s - %u faces", fileMesh.at(currentMesh).c_str(), mesh->getNumVertices()/3);
	font.addText(2, 42, c, "(c) Environment map: %s", fileCubeMap.at(currentCubeMap).c_str());
	c = Color(0.6f, 0.6f, 0.8f);
	font.addText(2, 62, c, "(h) Effect: %s", fileShaderEffect.at(currentEffect).c_str());

	// Brightness Threshold
	c = Color(1.0f, 1.0f, 0.0f);
	font.addText(2, 82, c, "(t) Bright: %.2f", brightThreshold);

	// Bloom factor
	font.addText(2, 102, c, "(o) Bloom: %.2f", bloomFactor);

	// Exposure
	font.addText(2, 122, c, "(e) Exposure: %.2f", exposure);

	// Fresnel parameters
	if (currentEffect == 3 || currentEffect == 4)
	{
		c = Color(0.6f, 0.6f, 0.8f);
		font.addText(screenW-190, 62, c, "Fresnel effect:");
		font.addText(screenW-170, 42, c, "(b) bias: %.2f ", fresnelBias);
		font.addText(screenW-170, 22, c, "(s) scale: %.2f ", fresnelScale);
		font.addText(screenW-170, 2, c, "(p) power: %.2f ", fresnelPower);
	}

	// Material color
	if (currentEffect == 0)
	{
		c = Color(0.6f, 0.6f, 0.8f);
		font.addText(screenW-160, 62, c, "Material color:");
		font.addText(screenW-140, 42, c, "(1) R: %.2f ", matColor.r);
		font.addText(screenW-140, 22, c, "(2) G: %.2f ", matColor.g);
		font.addText(screenW-140, 2, c, "(3) B: %.2f ", matColor.b);
	}

	// eta ratio
	if (currentEffect == 2 || currentEffect == 3)
	{
		c = Color(0.6f, 0.6f, 0.8f);
		font.addText(2, 142, c, "(a) eta ratio: %.2f", etaRatio.x);
	}

	// eta ratio : rgb
	if (currentEffect == 4)
	{
		c = Color(0.6f, 0.6f, 0.8f);
		font.addText(screenW-360, 62, c, "(a) eta ratio:");
		font.addText(screenW-340, 42, c, "(5) R: %.2f ", etaRatio.x);
		font.addText(screenW-340, 22, c, "(6) G: %.2f ", etaRatio.y);
		font.addText(screenW-340, 2, c, "(7) B: %.2f ", etaRatio.z);
	}

	// Reflection factor
	if (currentEffect == 0 || currentEffect == 1)
	{
		c = Color(0.6f, 0.6f, 0.8f);
		font.addText(2, 142, c, "(r) Reflection factor: %.2f", reflectionFactor);
	}

	// HDR and LDR
	c = Color(1.0f, 0.4f, 0.4f);
	font.addText((int)(mid*screenW), screenH-40, c, "HDR-->");
	font.addText((int)(mid*screenW)-70, screenH-40, c, "<--LDR");

	font.render();
}

void HdrApp::resize(int w, int h)
{
	// Notify camera
	cam->resize(screenW = w, screenH = h);
	cam->setMid(mid);

	// Release all render textures
	rtHdr->release();
	rtRgb->release();
	rtBloom->release();
	downSampler->release();

	// And recreate them
	rtHdr->init(w, h, GL_RGBA16F_ARB);
	rtRgb->init(w, h, GL_RGBA);
	rtBloom->init(w, h, GL_RGBA16F_ARB);
	downSampler->init(w, h, GL_RGBA16F_ARB);

	// Resize the font
	font.resize(w, h);
}

void HdrApp::release()
{
	// Release and destroy the downsampler
	downSampler->release();
	delete downSampler;

	// Release the cube maps
	cubeMapHdr.release();
	cubeMapRgb.release();

	// Release shaders
	shaderBloom.release();
	shaderBlur.release();
	shaderTone.release();

	// Release effect shaders
	for (size_t i=0 ; i<shaderEffect.size() ; ++i)
		shaderEffect.at(i).release();

	// Release and delete all render textures
	rtRgb->release();
	rtHdr->release();
	rtBloom->release();
	delete rtHdr;
	delete rtRgb;
	delete rtBloom;

	// Release OpenGLTexture2D
	textureHdr.release();
	textureRgb.release();

	// Release the font
	font.release();
}

void HdrApp::resetEffect()
{
	// First effect
	currentEffect = 0;

	// Effect parameters
	exposureControl = 0.86f;
	fresnelBias = 0.2f;
	fresnelScale = 1.0f;
	fresnelPower = 1.0f;
	reflectionFactor = 0.2f;
	refractionFactor = 0.2f;
	matColor = Color(0.1f, 0.1f, 0.12f);
	etaRatio = Vector3(0.8f, 0.8f, 0.8f);
	bloomFactor = 0.5f;
	brightThreshold = 1.2f;
}

void HdrApp::keyIncControlDec(int key, float &val, float offset, float min, float max)
{
	if (isKeyDown(key))
	{
		if (isKeyDown(SDLK_RCTRL) || isKeyDown(SDLK_LCTRL))
			val = Math::clamp(val-offset, min, max);
		else
			val = Math::clamp(val+offset, min, max);
	}
}

bool HdrApp::keyIncControlDecLoop(int key, int &val, int max)
{
	bool change = false;
	if (isKeyJustDown(key))
	{
		change = true;
		if (isKeyDown(SDLK_RCTRL) || isKeyDown(SDLK_LCTRL))
		{
			if (--val < 0)
				val = max-1;
		}
		else
		{
			if (++val >= max)
				val = 0;
		}
	}
	return change;
}
