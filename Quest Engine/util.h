#pragma once

#define DEBUG 0

#include <GL/glew.h>

/* Report GL errors, if any, to stderr. */
inline void checkError(const char* functionName = __builtin_FUNCTION())
{
#if DEBUG
	GLenum error;
	
	while ((error = glGetError()) != GL_NO_ERROR) {
        fprintf(stderr, "GL error 0x%04X detected in %s\n", error, functionName);
	}
#endif
}
