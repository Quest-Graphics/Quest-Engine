#pragma once

#include <GL/glew.h>

/* Report GL errors, if any, to stderr. */
inline void checkError(const char* functionName = __builtin_FUNCTION())
{
	GLenum error;
	
	while ((error = glGetError()) != GL_NO_ERROR) {
        fprintf(stderr, "GL error 0x%04X detected in %s\n", error, functionName);
	}
}
