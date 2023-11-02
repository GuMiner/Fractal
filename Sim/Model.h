#pragma once
#include "GLCore/ShaderFactory.h"

class Model
{
    GLuint modelProgram;

public:
    Model();
    bool Init(ShaderFactory* shaderFactory);
};