#include <GL/glew.h>
#include <GL/freeglut.h>

#include "render.h"
#include "input.h"


namespace GLSL_Naps {
	VSShaderLib*	Render::shader;
	VSResModelLib	Render::earth;
	VSMathLib*		Render::vsml;
	float			Render::lightPos[]		= {3.0f, 0.0f, 3.0f, 1.0f};	
	float			Render::lightDir[]		= {1.0f, 0.0f, 0.0f, 0.0f};
	float			Render::lightRadius		= 90.0f;

	float			Render::diffuse[]	= {0.8f, 0.8f, 0.8f};
	float			Render::specular[]	= {0.1f, 0.1f, 0.1f};
	float			Render::shininess	= 4;

	void Render::render() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	glLoadIdentity();

		vsml->loadIdentity(VSMathLib::VIEW);
		vsml->loadIdentity(VSMathLib::MODEL);
		vsml->lookAt(Input::cam[0], Input::cam[1], Input::cam[2], 0,0,0, 0,1,0);
		earth.render();

		glutSwapBuffers();
		
	}

	#pragma region "Setup"
	void Render::setupShaders() {
		shader = new VSShaderLib();
		shader->init();
		reloadShaders();
	}
	
	void Render::reloadShaders() {
		VSShaderLib *new_shader = new VSShaderLib();
		new_shader->init();
		new_shader->loadShader(VSShaderLib::VERTEX_SHADER, "resources/shaders/texture.vert");
		new_shader->loadShader(VSShaderLib::FRAGMENT_SHADER, "resources/shaders/texture.frag");
		new_shader->setVertexAttribName(VSShaderLib::VERTEX_COORD_ATTRIB, "position");
		new_shader->setVertexAttribName(VSShaderLib::NORMAL_ATTRIB, "normal");
		new_shader->setVertexAttribName(VSShaderLib::TEXTURE_COORD_ATTRIB, "texCoord");
	
		new_shader->prepareProgram();
		printf("%s\n",new_shader->getAllInfoLogs().c_str());

		glValidateProgram(new_shader->getProgramIndex());
		int validation;
		glGetProgramiv(new_shader->getProgramIndex(), GL_VALIDATE_STATUS, &validation);
		if (validation == GL_TRUE) {
			new_shader->setUniform("lightPos", lightPos);
			new_shader->setUniform("lightDir", lightDir);
			//new_shader->setUniform("lightRadius", lightRadius);
			//new_shader->setUniform("diffuse", diffuse);
			//new_shader->setUniform("specular", specular);
			//new_shader->setUniform("shininess", shininess);
			new_shader->setUniform("texDay", 0);
			new_shader->setUniform("texNight", 1);
			new_shader->setUniform("texClouds", 2);
			new_shader->setUniform("texSpec", 3);
			glUseProgram(new_shader->getProgramIndex());

			delete shader;
			shader = new_shader;
		} else {
			delete new_shader;
		}
	}

	void Render::setupVSL() {
		vsml = VSMathLib::getInstance();
		vsml->setUniformName(VSMathLib::PROJ_VIEW_MODEL, "pvm");
		vsml->loadIdentity(VSMathLib::MODEL);
		vsml->loadIdentity(VSMathLib::VIEW);
		vsml->setUniformName(VSMathLib::VIEW_MODEL, "vm");
		vsml->setUniformName(VSMathLib::NORMAL, "normalMat");
		
	}

	void Render::setupBuffers() {
		earth.load("resources/models/sphere.obj");
		earth.addTexture(0, "resources/textures/earthDay.jpg");
		earth.addTexture(1, "resources/textures/earthNight.jpg");
		earth.addTexture(2, "resources/textures/earthClouds.jpg");
		earth.addTexture(3, "resources/textures/earthSpec.jpg");

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
		//glGetIntegerv(GL_CONTEXT_FLAGS, &param);

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