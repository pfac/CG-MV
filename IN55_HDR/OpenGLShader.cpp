#include "OpenGLShader.h"

std::string OpenGLShader::dir = "shaders/";

OpenGLShader::OpenGLShader()
{
	progObj = 0;
	vertObj = 0;
	fragObj = 0;
}

OpenGLShader::~OpenGLShader()
{
	release();
}

void OpenGLShader::setDir(const std::string &path)
{
	dir = path;
}

void OpenGLShader::load(const std::string &vert, const std::string &frag)
{
	// Create program object
	progObj = glCreateProgramObjectARB();
	if (progObj == 0)
		throw Exception("Can't create OpenGLShader : " + vert + " & " + frag);

	// Create vertex shader object
	vertObj = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	if (vertObj == 0)
		throw Exception("Can't create OpenGLShader : " + vert);

	// Load vertex shader from file
	loadShaderFromFile(vertObj, dir+vert);

	// Compile vertex shader
    glCompileShaderARB(vertObj);

	// Attach vertex shader to program
	glAttachObjectARB(progObj, vertObj);
    
	// Create fragment shader object
	fragObj = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	if (fragObj == 0)
		throw Exception("Can't create OpenGLShader : " + frag);

	// Load fragment shader from file
	loadShaderFromFile(fragObj, dir+frag);

	// Compile fragment shader
    glCompileShaderARB(fragObj);

	// Attach fragment shader to program
	glAttachObjectARB(progObj, fragObj);
    
	// Link program
	GLint linked;
	glLinkProgramARB(progObj);
	glGetObjectParameterivARB(progObj, GL_OBJECT_LINK_STATUS_ARB, &linked);
	if (linked == 0)
		throw Exception("Can't link OpenGLShader : " + vert + " & " + frag);
}


void OpenGLShader::release()
{
	if (vertObj != 0)
	{
		glDetachObjectARB(progObj, vertObj);
		glDeleteObjectARB(vertObj);
		vertObj = 0;
	}

	if (fragObj != 0)
	{
		glDetachObjectARB(progObj, fragObj);
		glDeleteObjectARB(fragObj);
		fragObj = 0;
	}

	if (progObj != 0)
	{
		
		glDeleteObjectARB(progObj);
		progObj = 0;
	}
}

void OpenGLShader::begin() const
{
	glUseProgramObjectARB(progObj);
}

void OpenGLShader::end()
{
	glUseProgramObjectARB(0);
}

void OpenGLShader::loadShaderFromFile(GLhandleARB obj, const std::string &filename)
{
	// Open file to read
	FILE* f = fopen(filename.c_str(), "rb");
	if (f == 0)
		throw Exception("File missing : " + filename);

	// Count char
	char c;
	long len = 0;
	while (feof(f) == 0)
	{
		fread(&c, sizeof(char), 1, f);
		len++;
	}

	// Read file
	rewind(f);
	char *str = new char[len];
	fread( str, sizeof(char), len-1, f);
	str[len-1] = '\0';

	// Assign source
	const char* source = (const char*)str;
	glShaderSourceARB(obj, 1, &source, NULL);

	// Free resources
	delete [] str;
	fclose(f);
}

void OpenGLShader::setUniform(const char* str, int i) const
{
	glUniform1iARB(glGetUniformLocation(progObj, str), i);
}

void OpenGLShader::setUniform(const char* str, float f) const
{
	glUniform1fARB(glGetUniformLocation(progObj, str), f);
}

void OpenGLShader::setUniform(const char* str, const Color &c) const
{
	glUniform4fvARB(glGetUniformLocation(progObj, str), 1, (const GLfloat*)&c);
}

void OpenGLShader::setUniform(const char* str, const Vector3 &v) const
{
	glUniform3fvARB(glGetUniformLocation(progObj, str), 1, (const GLfloat*)&v);
}
