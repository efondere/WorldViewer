#include <glad/gl.h>
#include "glUtil.hpp"
#include <iostream>

void glClearErrors()
{
    while (glGetError() != GL_NO_ERROR);
}

bool glLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
        return false; // for demo. purposes
    }
    return true;
}
