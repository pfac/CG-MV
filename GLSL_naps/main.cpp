#include <GL/glew.h>
#include <GL/freeglut.h>

#include "input.h"
#include "render.h"
using namespace GLSL_Naps;

int main(int argc, char **argv) {
	Render::init(&argc, argv);
	Render::setupVSL();
	Render::setupShaders();
	Render::setupBuffers();
	glutMainLoop();
}