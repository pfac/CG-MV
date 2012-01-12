
#include <string>
#include <vector>


// include GLEW to access OpenGL 3.3 functions
#include <GL/glew.h>

// GLUT is the toolkit to interface with the OS
#include <GL/freeglut.h>


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

	glutSwapBuffers();

}



// --------------------------------------------------------
//
// Shader Stuff
//


GLuint setupShaders() {

// ADD STUFF HERE

	return(1);
}



// ------------------------------------------------------------
//
// Model loading and OpenGL setup
//


void initGL()
{

// ADD STUFF HERE

	// some GL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
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

	glutInitContextVersion (4, 2);
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
	if (glewIsSupported("GL_VERSION_4_2"))
		printf("Ready for OpenGL 4.2\n");
	else {
		printf("OpenGL 4.2 not supported\n");
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

	setupShaders();
	initGL();

	//  GLUT main loop
	glutMainLoop();

	return(1);

}

