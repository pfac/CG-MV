#ifndef __OPENGL_RENDERER_H__
#define __OPENGL_RENDERER_H__

#include "std.h"
#include "OpenGL.h"
#include "Math.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "Color.h"
#include "OpenGLTexture2D.h"
#include "OpenGLTextureCube.h"
#include "ImageManager.h"
#include "TriMeshManager.h"
#include "OpenGLFBO.h"
#include "OpenGLShader.h"
#include "OpenGLFont.h"
#include "Camera.h"
#include "DownSampler.h"

class OpenGLRenderer
{
public:
	OpenGLRenderer();

	// Set basic OepnGL states
	void init();

	// Change viewport
	void setViewport(int x, int y, int w, int h);

	// Clear selected buffers
	void clearBuffer(bool color = true, bool depth = true, bool stencil = false);

	// Change clear color
	void setClearColor(const Color &c);

	// Change projection matrix
	void setOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	void setFrustum(float left, float right, float bottom, float top, float zNear, float zFar);
	void setPerspective(float fovy, float aspect, float zNear, float zFar);
	void setProjectionMatrix(const Matrix4 &proj);

	// Change model-view matrix
	void setModelView(const Matrix4& model, const Matrix4& view=Matrix4::IDENTITY);

	// Render a mesh
	void renderTriMesh(TriMesh *m);

	// Render a skybox from a cube map
	void renderSkyBox(const OpenGLTextureCube &cubeMap);

private:
	Color clearColor;
};

#endif // __OPENGL_RENDERER_H__

