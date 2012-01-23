#pragma once

#include "common.h"

namespace GLSL_Naps {

	class Input {
	public:
		static float cam[3];
		static int startX;
		static int startY;
		static int tracking;
		static float alpha, beta, r;

	public:
		static void processKeys(unsigned char key, int xx, int yy);
		static void processMouseButtons(int button, int state, int xx, int yy);
		static void processMouseMotion(int xx, int yy);
		static void mouseWheel(int wheel, int direction, int x, int y);

	private:
		static void updateCam();
		static void updateCam(float r, float alpha, float beta);
	};
}