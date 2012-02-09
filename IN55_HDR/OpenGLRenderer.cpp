#include "OpenGLRenderer.h"
#include "AllocOverLoaderOn.h"

OpenGLRenderer::OpenGLRenderer()
{
	clearColor = Color::BLACK;
}

void OpenGLRenderer::init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void OpenGLRenderer::setViewport(int x, int y, int w, int h)
{
	glViewport(x, y, w, h);
}

void OpenGLRenderer::clearBuffer(bool color, bool depth, bool stencil)
{
	GLbitfield mask = 0;
	if (color)
		mask |= GL_COLOR_BUFFER_BIT;
	if (depth)
		mask |= GL_DEPTH_BUFFER_BIT;
	if (stencil)
		mask |= GL_STENCIL_BUFFER_BIT;

	if (mask != 0)
		glClear(mask);
}

void OpenGLRenderer::setClearColor(const Color &c)
{
	glClearColor(c.r, c.g, c.b, c.a);
}

void OpenGLRenderer::setOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(left, right, bottom, top, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
}

void OpenGLRenderer::setFrustum(float left, float right, float bottom, float top, float zNear, float zFar)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(left, right, bottom, top, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
}

void OpenGLRenderer::setPerspective(float fovy, float aspect, float zNear, float zFar)
{
	float xmin, xmax, ymin, ymax;

	ymax = zNear * Math::tan(fovy * Math::PI / 360.0f);
	ymin = -ymax;
	xmin = ymin * aspect;
	xmax = ymax * aspect;

	setFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
}

void OpenGLRenderer::setProjectionMatrix(const Matrix4 &proj)
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(proj);
	glMatrixMode(GL_MODELVIEW);
}

void OpenGLRenderer::setModelView(const Matrix4& model, const Matrix4& view)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(view);
	glMultMatrixf(model);
}

void OpenGLRenderer::renderTriMesh(TriMesh *m)
{
	GLsizei stride = sizeof(TriMesh::Vertex);
	glVertexPointer(3, GL_FLOAT, stride, m->getVertex(0).p);
	glNormalPointer(GL_FLOAT, stride, m->getVertex(0).n);
	glTexCoordPointer(2, GL_FLOAT, stride, &m->getVertex(0).u);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glDrawArrays(GL_TRIANGLES, 0, m->getNumVertices());

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);	
}

void OpenGLRenderer::renderSkyBox(const OpenGLTextureCube &cubeMap)
{
	// Activate environnment mapping
	glPushMatrix();
		glLoadIdentity();
		GLfloat s_plane[] = { 1.0, 0.0, 0.0, 0.0 };
		GLfloat t_plane[] = { 0.0, 1.0, 0.0, 0.0 };
		GLfloat r_plane[] = { 0.0, 0.0, 1.0, 0.0 };
		glTexGenfv(GL_S, GL_OBJECT_PLANE, s_plane);
		glTexGenfv(GL_T, GL_OBJECT_PLANE, t_plane);
		glTexGenfv(GL_R, GL_OBJECT_PLANE, r_plane);
    glPopMatrix();

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glEnable(GL_TEXTURE_CUBE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	cubeMap.bind();

	// Draw a cube
	float s = 500.0f;
	glBegin(GL_QUADS); 
		glVertex3f(+s, -s, -s);
		glVertex3f(+s, -s, +s);
		glVertex3f(+s, +s, +s);
		glVertex3f(+s, +s, -s);

		glVertex3f(-s, -s, -s);
		glVertex3f(-s, +s, -s);
		glVertex3f(-s, +s, +s);
		glVertex3f(-s, -s, +s);

		glVertex3f(-s, +s, -s);
		glVertex3f(+s, +s, -s);
		glVertex3f(+s, +s, +s);
		glVertex3f(-s, +s, +s);

		glVertex3f(-s, -s, -s);
		glVertex3f(-s, -s, +s);
		glVertex3f(+s, -s, +s);
		glVertex3f(+s, -s, -s);

		glVertex3f(-s, -s, -s);
		glVertex3f(+s, -s, -s);
		glVertex3f(+s, +s, -s);
		glVertex3f(-s, +s, -s);

		glVertex3f(-s, -s, +s);
		glVertex3f(-s, +s, +s);
		glVertex3f(+s, +s, +s);
		glVertex3f(+s, -s, +s);
	glEnd();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_CUBE_MAP);
}
