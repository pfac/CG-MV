#include "DownSampler.h"
#include "Math.h"
#include "Color.h"

DownSampler::DownSampler()
{

}

DownSampler::~DownSampler()
{
	renderTextures.clear();
}
                        
void DownSampler::init(int w, int h, int format)
{
	// Calculate the number of texture if we want to get a 1x1 texture
	int x = w;
	int y = h;

	int nx = 0;
	while (x>1)
	{
		nx++;
		x = Math::ceil(x/2.0f);
	}

	int ny = 0;
	while (y>1)
	{
		ny++;
		y = Math::ceil(y/2.0f);
	}

	int n = (nx > ny) ? nx : ny;
	renderTextures.resize(n);

	// Create FrameBuffer Objects
	for (size_t i=0 ; i<renderTextures.size() ; ++i)
	{
		w = Math::ceil(w/2.0f);
		h = Math::ceil(h/2.0f);
		renderTextures[i] = new OpenGLFBO();
		renderTextures[i]->init(w, h, format);
	}
}

void DownSampler::release()
{
	for (size_t i=0 ; i<renderTextures.size() ; ++i)
	{
		renderTextures[i]->release();
		delete renderTextures[i];
	}
}

void DownSampler::update(OpenGLFBO *mainRt, OpenGLShader *shader, int limit)
{
	assert(Math::ceil(mainRt->getW()/2.0f) == renderTextures[0]->getW());
	assert(Math::ceil(mainRt->getH()/2.0f) == renderTextures[0]->getH());

	// Set max number of textures
	int n = limit;
	if (limit != 0)
		n = (limit < (int)renderTextures.size()) ? limit : (int)renderTextures.size();
	else
		n = (int)renderTextures.size();

	// Set global state
	glEnable(GL_TEXTURE_2D);

	// Generate all down-sampled render texture
	for (int i=0 ; i<n ; ++i)
	{
		OpenGLFBO *curRt = renderTextures[i];
		OpenGLFBO *prevRt = (i==0 ? mainRt : renderTextures[i-1]);
		GLint w = curRt->getW();
		GLint h = curRt->getH();

		// Render on current fbo
		curRt->beginCapture();
		glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);

		// Set viewport and ortho projection
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0f, 1.0f, 0.0f, 1.0f, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// Use previous render texture as input texture
		prevRt->bind();

		// User shader to render
		if (shader)
		{
			shader->begin();
			shader->setUniform("w", (float)w);
			shader->setUniform("h", (float)h);
		}

		// Draw quad on the screen
		glColor3fv(Color::WHITE);
		glBegin(GL_QUADS); 
			glTexCoord2i(0, 0);	glVertex2i(0, 0);
			glTexCoord2i(0, 1);	glVertex2i(0, 1);
			glTexCoord2i(1 ,1);	glVertex2i(1, 1);
			glTexCoord2i(1, 0);	glVertex2i(1, 0);
		glEnd();

		// Restore shader
		if (shader)
			shader->end();
	}

	// Restore state
	OpenGLFBO::endCapture();
}
