#pragma once
#include <GL/glew.h>
#include <iostream>

/* Report GL errors, if any, to stderr. */
void checkError(const char* functionName = __builtin_FUNCTION())
{
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
		std::cerr << "GL error " << error << " detected in " << functionName << std::endl;
	}
}
