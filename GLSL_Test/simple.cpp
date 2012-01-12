#pragma comment(lib, "glew32.lib")

#include <string>
#include <vector>
#include "textfile.h"

// include GLEW to access OpenGL 3.3 functions
#include <GL/glew.h>

// GLUT is the toolkit to interface with the OS
#include <GL/freeglut.h>

GLuint vao, prog;

// ------------------------------------------------------------
//
// Reshape Callback Function
//

void changeSize(int w, int h) {

	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);
}

// --------------------------------------------------------
//
// Render Stuff
//

void renderScene() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// ADD STUFF HERE
	glUseProgram(prog);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glutSwapBuffers();

}

GLuint setupShaders() {

// ADD STUFF HERE
	GLuint f,v;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	char *vs = textFileRead("color.vert");
	char *fs = textFileRead("color.frag");

	const char *vv = vs;
	const char *ff = fs;

	glShaderSource(v, 1, &vv, NULL);
	glShaderSource(f, 1, &ff, NULL);

	free(vs);
	free(fs);

	glCompileShader(v);
	glCompileShader(f);

	prog = glCreateProgram();
	glAttachShader(prog,v);
	glAttachShader(prog,f);
	glLinkProgram(prog);
	printf("Shaders done\n");
	return(1);
}



// ------------------------------------------------------------
//
// Model loading and OpenGL setup
//


void initGL()
{

	setupShaders();

// ADD STUFF HERE
	float position[12] = {-0.5f, -0.5f, 0.0f, 1.0f,
							0.0f, 0.5f, 0.0f, 1.0f, 
							0.5f,   -0.5f, 0.0f, 1.0f};

	float color[9] = {	1.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 1.0f};

	GLint posLoc = glGetAttribLocation(prog, "position");
	GLint colorLoc =  glGetAttribLocation(prog, "color");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint buffer;
	glGenBuffers(1, &buffer);

	//bind buffer for positions  and copy data  into buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);
	glEnableVertexAttribArray(posLoc);
	glVertexAttribPointer(posLoc, 4, GL_FLOAT, 0, 0, 0);

	glGenBuffers(1, &buffer);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
	glEnableVertexAttribArray(colorLoc);
	glVertexAttribPointer(colorLoc, 3, GL_FLOAT, 0, 0, 0);


	//glGenBuffers(1, &buffer);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

	// some GL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

}


// ------------------------------------------------------------
//
// Main function
//


int main(int argc, char **argv) {

//  GLUT initialization
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA|GLUT_MULTISAMPLE);

	glutInitContextVersion (3, 3);
	glutInitContextProfile (GLUT_CORE_PROFILE );
	glutInitContextFlags(GLUT_DEBUG);

	glutInitWindowPosition(100,100);
	glutInitWindowSize(640,360);
	glutCreateWindow("Lighthouse3D - Simplest Demo");

//  Callback Registration
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);


//	return from main loop
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

//	Init GLEW
	glewExperimental = GL_TRUE;
	glewInit();
	if (glewIsSupported("GL_VERSION_3_3"))
		printf("Ready for OpenGL 3.3\n");
	else {
		printf("OpenGL 3.3 not supported\n");
		exit(1);
	}

   printf ("Vendor: %s\n", glGetString (GL_VENDOR));
   printf ("Renderer: %s\n", glGetString (GL_RENDERER));
   printf ("Version: %s\n", glGetString (GL_VERSION));
   printf ("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));
   int param;
   glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &param);
   if (param == GL_CONTEXT_CORE_PROFILE_BIT)
	   printf("Context Profile: Core\n");
   else
	   printf("Context Profile: Compatibility\n");
   glGetIntegerv(GL_CONTEXT_FLAGS, &param);

	initGL();

	//  GLUT main loop
	glutMainLoop();

	return(1);
}