#ifndef __OPENGL_APP_
#define __OPENGL_APP_

#include "OpenGLRenderer.h"

class OpenGLApp
{
public:
	OpenGLApp();
	virtual ~OpenGLApp();

	// Start the application
	void run(int w, int h);

protected:
	// Overload these functions in your application, they will be auto called
	virtual void init(int w, int h) = 0;
	virtual void loadResources() = 0;
	virtual void loadOpenGLResources() = 0;
	virtual void update(float dt) = 0;
	virtual void render() = 0;
	virtual void renderLoadingScreen() = 0;
	virtual void resize(int w, int h) = 0;
	virtual void release() = 0;

	// Change the title of the window
	void setTitle(const char *title);

	// Terminate application
	void requestQuit();

	// Time elapsed since the begining of the application
	float getTime() const;

	// Returns true if the key is pushed
	bool isKeyDown(int i) const			{ return keyPress[i]; }

	// Returns true the first time the key is pushed (no repeat)
	bool isKeyJustDown(int i) const		{ return keyJustPress[i]; }

	// Returns true if the mouse button is pushed
	bool isButtonDown(int i) const		{ return buttonPress[i]; }

	// Returns true the first time the mouse button is pushed (no repeat)
	bool isButtonJustDown(int i) const	{ return buttonJustPress[i]; }

	// Mouse relative movements
	int getMouseRelX() const { return mouseRelX; }
	int getMouseRelY() const { return mouseRelY; }

	// The OpenGL renderer
	OpenGLRenderer *getRenderer() const { return renderer; }

	// Get average Frame Per Second
	float getAvgFps() const;

private:
	// Application's time
	Uint32 time;

	// Loading flag
	bool loading;

	// FPS
	const static int NUM_FPS_VALUES = 10;
	float fps[NUM_FPS_VALUES];

	// Input
	bool keyPress[SDLK_LAST];
	bool keyJustPress[SDLK_LAST];
	bool buttonPress[5];
	bool buttonJustPress[5];
	int mouseX, mouseY;
	int mouseRelX, mouseRelY;
	void resetInput(bool all);

	// OpenGL
	OpenGLRenderer *renderer;

	// Thread to load resources
	friend void loadThread();
};

#endif //__OPENGL_APP_
