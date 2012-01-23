#pragma comment(lib, "glew32.lib")

#include <string>
#include <vector>
#include "textfile.h"

// include GLEW to access OpenGL 3.3 functions
#include <GL/glew.h>

// GLUT is the toolkit to interface with the OS
#include <GL/freeglut.h>

#include "vsShaderLib.h"

VSShaderLib shader;
GLuint vao;
float lpos[4] = {1.0,0.0,1.0,0.0};

// ------------------------------------------------------------
//
// Reshape Callback Function
//

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	float ratio = 1.0* w / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,1,100);
	glMatrixMode(GL_MODELVIEW);

}

// --------------------------------------------------------
//
// Render Stuff
//

void renderScene() {

// ADD STUFF HERE
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0.0,5.0,5.0, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	glutSolidTeapot(1);

	glutSwapBuffers();

}

void printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
        free(infoLog);
    }
}

void printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
        free(infoLog);
    }
}

GLuint v,f,f2,p;

GLuint setupShaders() {

// ADD STUFF HERE
	
	shader.init();
	shader.loadShader(VSShaderLib::VERTEX_SHADER, "anim.vert");
	shader.loadShader(VSShaderLib::FRAGMENT_SHADER, "anim.frag");
	//shader.setProgramOutput(0, "colorFS");
	shader.setVertexAttribName(VSShaderLib::VERTEX_COORD_ATTRIB, "position");
	shader.prepareProgram();
	glUseProgram(shader.getProgramIndex());
	return 1;

	char *vs = NULL,*fs = NULL,*fs2 = NULL;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);
	f2 = glCreateShader(GL_FRAGMENT_SHADER);

	vs = textFileRead("anim.vert");
	fs = textFileRead("anim.frag");

	const char * vv = vs;
	const char * ff = fs;

	glShaderSource(v, 1, &vv,NULL);
	glShaderSource(f, 1, &ff,NULL);

	free(vs);free(fs);

	glCompileShader(v);
	glCompileShader(f);

	printShaderInfoLog(v);
	printShaderInfoLog(f);
	printShaderInfoLog(f2);

	p = glCreateProgram();
	glAttachShader(p,v);
	glAttachShader(p,f);

	glLinkProgram(p);
	//printProgramInfoLog(p);

	glUseProgram(p);
	GLuint loc = glGetUniformLocation(p,"time");
	return 0;
}




// ------------------------------------------------------------
//
// Model loading and OpenGL setup
//


void initGL() {

	setupShaders();

// ADD STUFF HERE
	float position[12] = {0.0f, 1.0f, 0.0, 1.0f,
							1.0f, 0.0f, 0.0f, 1.0f, 
							-1.0f,   0.0f, 0.0f, 1.0f};

	float color[9] = {	1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 0.0f};

	//GLint posLoc = glGetAttribLocation(prog, "position");
	//GLint colorLoc =  glGetAttribLocation(prog, "color");

	/*glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint buffer;
	glGenBuffers(1, &buffer);

	//bind buffer for positions  and copy data  into buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);
	glEnableVertexAttribArray(VSShaderLib::VERTEX_COORD_ATTRIB);
	glVertexAttribPointer(VSShaderLib::VERTEX_COORD_ATTRIB, 4, GL_FLOAT, 0, 0, 0);

	glGenBuffers(1, &buffer);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
	glEnableVertexAttribArray(shader.);
	glVertexAttribPointer(VSShaderLib::VERTEX_COORD_ATTRIB, 3, GL_FLOAT, 0, 0, 0);*/

	// some GL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_MULTISAMPLE);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

}


// ------------------------------------------------------------
//
// Main function
//


int main(int argc, char **argv) {

//  GLUT initialization
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);

	//glutInitContextVersion (3, 3);
	//glutInitContextProfile (GLUT_CORE_PROFILE );
	//glutInitContextFlags(GLUT_DEBUG);

	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
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

	glEnable(GL_DEPTH_TEST);
	setupShaders();
	glutMainLoop();

	return(1);
}