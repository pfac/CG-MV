#include "OpenGLFont.h"
#include "ImageManager.h"
#include "AllocOverLoaderOn.h"

void OpenGLFont::init(int w, int h)
{
	// Load font texture
	tex.load("font.bmp", GL_RGBA);

	// init size
	resize(w, h);

	// Generate display list (based on NeHe code)
	base = glGenLists(256);
	tex.bind();
	for (int loop=0; loop<256; loop++)
	{
		float cx = float(loop%16) / 16.0f;
		float cy = float(loop/16) / 16.0f;

		glNewList(base+loop,GL_COMPILE);
			glBegin(GL_QUADS);
				glTexCoord2f(cx, 1-cy-0.0625f);
				glVertex2i(0, 0);
				glTexCoord2f(cx+0.0625f, 1-cy-0.0625f);
				glVertex2i(16, 0);
				glTexCoord2f(cx+0.0625f, 1-cy);
				glVertex2i(16, 16);
				glTexCoord2f(cx, 1-cy);
				glVertex2i(0, 16);
			glEnd();
			glTranslated(10, 0, 0);
		glEndList();
	}

	texts.clear();
}

void OpenGLFont::resize(int w, int h)
{
	screenW = w;
	screenH = h;
}

void OpenGLFont::release()
{
	texts.clear();
	tex.release();
	glDeleteLists(base, 256);
}

void OpenGLFont::addText(int x, int y, const Color &c, const char *str, ...)
{
	char msg[256];
	va_list	ap;

	va_start(ap, str);
	vsprintf_s(msg, str, ap);
	va_end(ap);

	texts.push_back(Text(msg, x, y, c));
}

void OpenGLFont::render()
{
	// Set states
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	tex.bind();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		glOrtho(0, screenW, 0, screenH, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		// Render all waiting texts
		for (std::list<Text>::iterator it = texts.begin(); it!=texts.end() ; ++it)
		{
				glLoadIdentity();
				glTranslated(it->x, it->y, 0);
				glColor3fv(it->c);
				glListBase(base - 32);
				glCallLists((GLsizei)it->msg.size(), GL_UNSIGNED_BYTE, it->msg.c_str());
		}
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	texts.clear();
}
