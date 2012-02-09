#include "Camera.h"

Camera::Camera(int w, int h, OpenGLRenderer *r) : screenW(w), screenH(h), renderer(r)
{	
	// Projection matrix
	fovy = 45.0f;
	zNear = 1.0f;
	zFar = 20.0f;
	recalculateProjMatrix();

	// View Matrix
	rho = 50.0f;
	theta = 0.0f;
	phi = 90.0f;
	recalculateViewMatrix();
}

void Camera::setPerspective(float fovy, float zNear, float zFar)
{
	this->fovy = fovy;
	this->zNear = zNear;
	this->zFar = zFar;
	recalculateProjMatrix();
}

void Camera::resize(int w, int h)
{
	screenW = w;
	screenH = h;
	recalculateProjMatrix();
}

void Camera::setMid(float mid)
{
	midPoint = (int)(mid*screenW);
	recalculateProjMatrix();
}

void Camera::use()
{
	renderer->setViewport(0, 0, screenW, screenH);
	renderer->setPerspective(fovy, (float)screenW / (float)screenH, zNear, zFar);
	renderer->setModelView(Matrix4::IDENTITY, view);
}

void Camera::useLeft()
{
	//renderer->setViewport(0, 0, screenW, screenH);
	renderer->setViewport(0, 0, midPoint, screenH);
	renderer->setProjectionMatrix(projLeft);
	renderer->setModelView(Matrix4::IDENTITY, view);
}

void Camera::useRight()
{
	renderer->setViewport(midPoint, 0, screenW-midPoint, screenH);
	//renderer->setViewport(0, 0, screenW, screenH);
	renderer->setProjectionMatrix(projRight);
	renderer->setModelView(Matrix4::IDENTITY, view);
}

void Camera::zoom(float d)
{
	rho -= d;

	// Update view matrix
	recalculateViewMatrix();
}

void Camera::rotateX(float deg)
{
	theta -= deg;

	// Update view matrix
	recalculateViewMatrix();
}

void Camera::rotateY(float deg)
{
	phi -= deg;

	// Clamp value to prevent camera flip
	if (phi > 179)
		phi = 179;
	if (phi < 1)
		phi = 1;

	// Update view matrix
	recalculateViewMatrix();
}

Vector3 Camera::getPos() const
{
	// Convert spherical coordinates to cartesian coordinates
	float cosTheta = Math::cos(Math::deg2rad(theta));
	float sinTheta = Math::sin(Math::deg2rad(theta));
	float cosPhi = Math::cos(Math::deg2rad(phi));
	float sinPhi = Math::sin(Math::deg2rad(phi));
	return Vector3(rho*sinTheta*sinPhi, rho*cosPhi, rho*cosTheta*sinPhi);
}

void Camera::recalculateProjMatrix()
{
	// Perspective projection
	float aspect = (float)screenW / screenH;
	float ymax = zNear * Math::tan(Math::deg2rad(fovy) * 0.5f);
	float ymin = -ymax;
	float xmin = ymin * aspect;
	float xmax = ymax * aspect;

	// Linear interpolation
	float x = (float)xmin + (float)midPoint * (xmax - xmin) / (float)screenW;

	// Set frustum matrix
	projLeft  = mat4Frustum(xmin, x, ymin, ymax, zNear, zFar);
	projRight = mat4Frustum(x, xmax, ymin, ymax, zNear, zFar);
}

void Camera::recalculateViewMatrix()
{
	view = mat4LookAt(getPos(), Vector3::ZERO, Vector3::UNIT_Y);
}

void Camera::initQuad(int w, int h)
{
	renderer->setViewport(0, 0, w, h);
	renderer->setOrtho(0.0f, 1.0f, 0.0f, 1.0f, -1.0, 1.0);
	glLoadIdentity();
}

void Camera::drawQuad()
{
	glBegin(GL_QUADS); 
		glTexCoord2i(0, 0);	glVertex2i(0, 0);
		glTexCoord2i(0, 1);	glVertex2i(0, 1);
		glTexCoord2i(1 ,1);	glVertex2i(1, 1);
		glTexCoord2i(1, 0);	glVertex2i(1, 0);
	glEnd();
}

void Camera::drawQuadLeft()
{
	GLfloat x = (float)midPoint/screenW;
	glBegin(GL_QUADS); 
		glTexCoord2f(0, 0);	glVertex2f(0, 0);
		glTexCoord2f(0, 1);	glVertex2f(0, 1);
		glTexCoord2f(x ,1);	glVertex2f(x, 1);
		glTexCoord2f(x, 0);	glVertex2f(x, 0);
	glEnd();
}

void Camera::drawQuadRight()
{
	GLfloat x = (float)midPoint/screenW;
	glBegin(GL_QUADS); 
		glTexCoord2f(x, 0);	glVertex2f(x, 0);
		glTexCoord2f(x, 1);	glVertex2f(x, 1);
		glTexCoord2f(1 ,1);	glVertex2f(1, 1);
		glTexCoord2f(1, 0);	glVertex2f(1, 0);
	glEnd();
}
