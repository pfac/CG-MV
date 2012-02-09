#pragma once

#include "vsShaderLib.h"
#include "vsResModelLib.h"
#include "vsMathLib.h"
#include "vsProfileLib.h"

#include "common.h"

namespace GLSL_Naps {

	class Render {
		
	static VSShaderLib *shader;
	static VSResModelLib earth;
	static VSMathLib *vsml;

	static float lightPos[];
	static float lightDir[];
	static float lightRadius;

	static float diffuse[];
	static float specular[];
	static float shininess;

	public:
		static void init(int *argc, char **argv);
		static void changeSize(int w, int h);
		static void render();

		static void setupBuffers();
		static void setupVSL();
		static void setupShaders();
		static void reloadShaders();
	};
}