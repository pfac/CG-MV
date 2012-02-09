#ifndef __OPENGL_FONT_H__
#define __OPENGL_FONT_H__

#include "std.h"
#include "OpenGLTexture2D.h"
#include "Color.h"

class OpenGLFont
{
public:
	// Create the font
	void init(int w, int h);

	// Free OpenGL memory
	void release();

	// Screen is resizing : update infos
	void resize(int w, int h);
	
	// Add a text to be rendered
	void addText(int x, int y, const Color &c, const char *str, ...);

	// Render and flush all texts
	void render();

private:
	GLuint base;
	GLuint screenW, screenH;
	OpenGLTexture2D tex;

	// An instance of a text on the screen
	struct Text
	{
		Text(const std::string &msg, int x, int y, const Color &c)
		{
			this->msg = msg;
			this->x = x;
			this->y = y;
			this->c = c;
		}

		std::string msg;
		int x, y;
		Color c;
	};

	// List of texts to print
	std::list<Text> texts;
};


#endif // __OPENGL_FONT_H__

