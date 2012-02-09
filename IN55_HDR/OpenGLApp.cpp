#include "OpenGLApp.h"
#include "SDLException.h"
#include "AllocOverLoaderOn.h"

OpenGLApp *g_app;
void loadThread()
{
	try
	{
		g_app->loadResources();
	}
	catch (const std::exception&) {}
	g_app->loading = false;
}

OpenGLApp::OpenGLApp()
{
	loading = true;
}

OpenGLApp::~OpenGLApp()
{
	delete renderer;
}

void OpenGLApp::run(int w, int h)
{
	// Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		throw SDLException("SDL_Init()");

	// Ask a doublebuffer
	if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) < 0)
		throw SDLException("SDL_GL_SetAttribute()");

	// Set video flags
	int videoFlags = SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_HWPALETTE | SDL_RESIZABLE;
	const SDL_VideoInfo *videoInfo = SDL_GetVideoInfo();
	if (videoInfo->hw_available)
		videoFlags |= SDL_HWSURFACE;
	else
		videoFlags |= SDL_SWSURFACE;

	if (videoInfo->blit_hw)
		videoFlags |= SDL_HWACCEL;

	// Set video mode
	SDL_Surface *surface = SDL_SetVideoMode(w, h, 32, videoFlags);
	if (surface == 0)
		throw SDLException("SDL_SetVideoMode()");

	// Check OpenGL extensions
	if (!GLEE_VERSION_2_0)
		throw Exception("You need OpenGL 2.0 suport to run this demo");

	if (!GLEE_EXT_framebuffer_object)
		throw Exception("Extension missing: EXT_framebuffer_object");

	if (!GLEE_ARB_texture_non_power_of_two)
		throw Exception("Extension missing: ARB_texture_non_power_of_two");

	if (!GLEE_ARB_texture_float)
		throw Exception("Extension missing: ARB_texture_float");

	// Initalize timer
	time = SDL_GetTicks();

	// Initialize input
	resetInput(true);

	// Create the renderer
	renderer = new OpenGLRenderer();
	renderer->init();

	// Initialize user application
	init(w, h);

	// Load ressources in a separate thread
	g_app = this;
	boost::thread loadingThread(loadThread);

	// Render loading screen while loading resources
	do
	{
		renderLoadingScreen();
		SDL_GL_SwapBuffers();
	} while (loading);

	// Render last loading string
	renderLoadingScreen();
	SDL_GL_SwapBuffers();

	// Load OpenGL resources : not in thread because OpenGL is not multithread safe
	loadOpenGLResources();

	// Reset fps counter
	for (int i=0 ; i<NUM_FPS_VALUES ; ++i)
		fps[i] = 0.0;

	// Main loop
	bool done = false;
	while (!done)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			int key;
			switch (event.type)
			{
			case SDL_QUIT:
				done = true;
				break;

			case SDL_VIDEORESIZE:
				resize(event.resize.w, event.resize.h);
				break;

			case SDL_KEYDOWN:
				key = event.key.keysym.sym;
				keyPress[key] = keyJustPress[key] = true;
				break;

			case SDL_KEYUP:
				key = event.key.keysym.sym;
				keyPress[key] = keyJustPress[key] = false;
				break;

			case SDL_MOUSEBUTTONDOWN:
				key = event.button.button - 1;
				buttonPress[key] = buttonJustPress[key] = true;
				break;

			case SDL_MOUSEBUTTONUP:
				key = event.button.button - 1;
				buttonPress[key] = buttonJustPress[key] = false;
				break;

			case SDL_MOUSEMOTION:
				mouseRelX = event.motion.xrel;
				mouseRelY = event.motion.yrel;
				mouseX += mouseRelX;
				mouseY += mouseRelY;
				break;
			}
		}

		// Update
		Uint32 t = SDL_GetTicks();
		float deltaTime = (t - time) * 0.001f;
		update(deltaTime);
		time = t;
		resetInput(false);

		// Calculate FPS
		static int currentFps = 0;
		if (deltaTime != 0.0f)
		{
			fps[currentFps] = 1.0f / deltaTime;
			if (++currentFps >= NUM_FPS_VALUES)
				currentFps = 0;
		}

		// Render
		render();
		SDL_GL_SwapBuffers();
	}

	// Release ressources
	release();
	SDL_Quit();
}

void OpenGLApp::setTitle(const char *title)
{
	SDL_WM_SetCaption(title, NULL);
}

void OpenGLApp::requestQuit()
{
	SDL_Event e;
	e.type = SDL_QUIT;
	SDL_PushEvent(&e);
}

float OpenGLApp::getTime() const
{
	return time * 0.001f;
}

void OpenGLApp::resetInput(bool all)
{
	// Mouse position
	mouseRelX = 0;
	mouseRelY = 0;

	// Keys state
	for (int i=0 ; i<SDLK_LAST ; i++)
		keyJustPress[i] = false;

	// Buttons state
	for (int j=0 ; j<5 ; j++)
		buttonJustPress[j] = false;

	if (all)
	{
		mouseX = 0;
		mouseY = 0;
		for (int i=0 ; i<SDLK_LAST ; i++)
			keyPress[i] = false;

		for (int j=0 ; j<5 ; j++)
		buttonPress[j] = false;
	}
}

float OpenGLApp::getAvgFps() const
{
	float avg = 0.0f;
	for (int i=0 ; i<NUM_FPS_VALUES ; i++)
		avg += fps[i];
	return avg / NUM_FPS_VALUES;
}
