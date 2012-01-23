#include <GL/glew.h>
#include <GL/freeglut.h>

#include "render.h"
#include "input.h"


namespace GLSL_Naps {
	VSShaderLib		Render::shader;
	VSResModelLib	Render::teapot;
	VSMathLib*		Render::vsml;
	float			Render::lightPos[] = {1.0f, 0.0f, 1.0f, 0.0f};

	void Render::render() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	glLoadIdentity();

		vsml->loadIdentity(VSMathLib::MODELVIEW);
		vsml->lookAt(Input::cam[0], Input::cam[1], Input::cam[2], 0,0,0, 0,1,0);
		teapot.render();

		glutSwapBuffers();
	}

	#pragma region "Setup"
	void Render::setupShaders() {
		shader.init();
		shader.loadShader(VSShaderLib::VERTEX_SHADER, "resources/shaders/toon.vert");
		shader.loadShader(VSShaderLib::FRAGMENT_SHADER, "resources/shaders/toon.frag");
		shader.setVertexAttribName(VSShaderLib::VERTEX_COORD_ATTRIB, "position");
		shader.setVertexAttribName(VSShaderLib::NORMAL_ATTRIB, "normal");
	
		shader.prepareProgram();
		printf("%s\n",shader.getAllInfoLogs().c_str());
	
		shader.setUniform("lightDir", lightPos);
		glUseProgram(shader.getProgramIndex());
	}

	void Render::setupVSL() {
		vsml = VSMathLib::getInstance();
		vsml->setUniformName(VSMathLib::PROJMODELVIEW, "pvm");
		vsml->setUniformName(VSMathLib::NORMAL, "normalMat");
	}

	void Render::setupBuffers() {
		teapot.load("resources/models/CatFinal.3ds");
	}
	#pragma endregion

	#pragma region "GL Init"
	void Render::init(int *argc, char **argv) {
		glutInit(argc, argv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowSize(640, 640);
		glutCreateWindow("GLSL - Aula 2 - Toon Teapot");
	
		glutReshapeFunc(&Render::changeSize);
		glutDisplayFunc(&Render::render);
		glutIdleFunc(Render::render);

		//	Mouse and Keyboard Callbacks
		glutKeyboardFunc(Input::processKeys);
		glutMouseFunc(Input::processMouseButtons);
		glutMotionFunc(Input::processMouseMotion);
		glutMouseWheelFunc(Input::mouseWheel);

		glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

		glewExperimental = GL_TRUE;
		glewInit();
		if (glewIsSupported("GL_VERSION_3_3"))
			printf( "OpenGL 3.3 supported\n");
		else {
			printf( "OpenGL 3.3 not supported. Aborting\n");
		}

		printf("Vendor:\t%s\n", glGetString(GL_VENDOR));
		printf("Renderer:\t%s\n", glGetString(GL_RENDERER));
		printf("Version:\t%s\n", glGetString(GL_VERSION));
		printf("GLSL:\t%s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		int param;
		glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &param);
		if (param == GL_CONTEXT_CORE_PROFILE_BIT)
			printf("Context Profile: Core\n");
		else
			printf("Context Profile: Compatibility\n");
		glGetIntegerv(GL_CONTEXT_FLAGS, &param);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
	}

	void Render::changeSize(int w, int h) {
		// Prevent a divide by zero, when window is too short
		// (you cant make a window of zero width).
		if(h == 0)
			h = 1;
		float ratio = 1.0f * w / h;
		// Set the viewport to be the entire window
		glViewport(0, 0, w, h);

		// Reset Matrixobrigada
		vsml->loadIdentity(VSMathLib::PROJECTION);
 
		// Set the correct perspective.
		vsml->perspective(45.0f, ratio, 0.1f, 100.0f);

		// Set the correct perspective.
		glMatrixMode(GL_MODELVIEW);
	}
	#pragma endregion
}