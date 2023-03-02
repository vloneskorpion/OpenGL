#include <glad/glad.h>
#include "easylogging/easylogging++.h"

#include "ErrorHandle.hpp"

void GLClearError()
{
    while(glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while(auto error = glGetError())
    {
        LOG(ERROR) << "[OpenGL Error] (" <<  error << ") " << function << " " << file << ":" << line;
        return false;
    }
    return true;
}