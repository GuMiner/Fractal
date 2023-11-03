#pragma once
#include "GLCore/ShaderFactory.h"
#include "Camera.h"

class Model
{
    GLuint modelProgram;

    GLuint modelVao;
    GLuint positionVbo;
    GLuint normalVbo;
    GLuint indexVbo;

public:
    Model();
    bool Init(ShaderFactory* shaderFactory);
    bool SendMesh();
    void Render(Camera* camera, float currentTime);
};