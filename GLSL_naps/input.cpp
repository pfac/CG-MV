#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>

#include "vsProfileLib.h"

#include "input.h"

namespace GLSL_Naps {

	float	Input::cam[] = {0, 0, 5};
	int		Input::startX;
	int		Input::startY;
	int		Input::tracking = 0;
	float	Input::alpha	= 0.0f;
	float	Input::beta		= 0.0f;
	float	Input::r		= 5.0f;

	void Input::processKeys(unsigned char key, int xx, int yy) {
		switch(key) {
			case 27:
				glutLeaveMainLoop();
				break;

			case 'z':
				r -= 0.1f;
				updateCam();
				break;

			case 'x':
				r += 0.1f;
				updateCam();
				break;
			
			case 'm': glEnable(GL_MULTISAMPLE); break;
			case 'n': glDisable(GL_MULTISAMPLE); break;
			case 'k': VSProfileLib::Reset(); break;
			case 'p':
				std::string s = VSProfileLib::DumpLevels();
					printf("%s\n", s.c_str());
					break;
		}

	//  uncomment this if not using an idle func
	//	glutPostRedisplay();
	}

	void Input::processMouseButtons(int button, int state, int xx, int yy) {
		// start tracking the mouse
		if (state == GLUT_DOWN)  {
			startX = xx;
			startY = yy;
			if (button == GLUT_LEFT_BUTTON)
				tracking = 1;
			else if (button == GLUT_RIGHT_BUTTON)
				tracking = 2;
		}

		//stop tracking the mouse
		else if (state == GLUT_UP) {
			if (tracking == 1) {
				alpha -= (xx - startX);
				beta += (yy - startY);
			} else if (tracking == 2) {
				r += (yy - startY) * 0.01f;
			}
			tracking = 0;
		}
	}

	// Track mouse motion while buttons are pressed
	void Input::processMouseMotion(int xx, int yy) {

		int deltaX, deltaY;
		float alphaAux, betaAux;
		float rAux;

		deltaX =  - xx + startX;
		deltaY =    yy - startY;

		// left mouse button: move camera
		if (tracking == 1) {
			alphaAux = alpha + deltaX;
			betaAux = beta + deltaY;

			if (betaAux > 85.0f)
				betaAux = 85.0f;
			else if (betaAux < -85.0f)
				betaAux = -85.0f;

			rAux = r;

			updateCam(rAux, alphaAux, betaAux);
		}
		// right mouse button: zoom
		else if (tracking == 2) {

			alphaAux = alpha;
			betaAux = beta;
			rAux = r + (deltaY * 0.01f);

			updateCam(rAux, alphaAux, betaAux);
		}
	//  uncomment this if not using an idle func
	//	glutPostRedisplay();
	}

	void Input::mouseWheel(int wheel, int direction, int x, int y) {
		r += direction * 0.1f;
		updateCam();
	//  uncomment this if not using an idle func
	//	glutPostRedisplay();
	}

	void Input::updateCam() {
		updateCam(r, alpha, beta);
	}

	void Input::updateCam(float r, float alpha, float beta) {
		cam[0] = r * sin(DEG2RAD(alpha)) * cos(DEG2RAD(beta));
		cam[2] = r * cos(DEG2RAD(alpha)) * cos(DEG2RAD(beta));
		cam[1] = r * sin(DEG2RAD(beta));
	}
}