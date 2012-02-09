#ifndef __DOWN_SAMPLER_H__
#define __DOWN_SAMPLER_H__

#include "OpenGLFBO.h"
#include "OpenGLShader.h"
#include "std.h"

class DownSampler
{
public:
	 DownSampler();
	~DownSampler();

	// Create the downsampler
	void init(int w, int h, int format);

	// Free memory
	void release();

	// Downsample mainRt using the shader
	void update(OpenGLFBO *mainRt, OpenGLShader *shader, int limit=0);

	// Number of render texture used
	int getNbRenderTexture() const { return (int)renderTextures.size(); }

	// Get a downsampled render texture
	OpenGLFBO* getRenderTexture(int n) const { return renderTextures.at(n); }

private:
	std::vector<OpenGLFBO*> renderTextures;
};

#endif	// __DOWN_SAMPLER_H__
