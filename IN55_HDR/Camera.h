#ifndef __CAMERA_H___
#define __CAMERA_H___

#include "OpenGLRenderer.h"

class Camera
{
public:
	// Init the camera
	Camera(int w, int h, OpenGLRenderer *r);

	// Change perspective
	void setPerspective(float fovy, float zNear, float zFar);

	// Screen is resizing : update camera
	void resize(int w, int h);

	// Load projection and view matrix
	void use();

	// Load projection and view matrix for the left part
	void useLeft();

	// Load projection and view matrix for the right part
	void useRight();

	// Prepare to render a quad : set viewport ortho projection
	void initQuad(int w, int h);

	// Render a quad covering the screen
	void drawQuad();

	// Set mid point for dual rendering
	void setMid(float mid);

	// Render a quad covering the left part of the screen
	void drawQuadLeft();

	// Render a quad covering the right part of the screen
	void drawQuadRight();

	// Control 3D camera
	void zoom(float d);
	void rotateX(float deg);
	void rotateY(float deg);
	Vector3 getPos() const;

private:
	// View matrix
	float theta, phi, rho;

	// Screen size
	int screenW, screenH, midPoint;

	// View camera
	float fovy, zNear, zFar;
	Matrix4 view, projLeft, projRight;

	// Update proj matrix
	void recalculateProjMatrix();

	// Update view matrix
	void recalculateViewMatrix();

	// Save access to the renderer
	OpenGLRenderer *renderer;
};

#endif // __CAMERA_H___
