#pragma once
#include "../GLCore/ShaderFactory.h"

class Sky {
    GLuint programId;

    GLuint vao, vbo;

public:
    bool Init(ShaderFactory* shaderFactory);
    void Render(float currentTime);
};

