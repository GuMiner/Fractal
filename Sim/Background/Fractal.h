#pragma once
#include "../GLCore/ShaderFactory.h"

class Fractal {
    GLuint programId;

    GLuint vao, vbo;
    GLuint rainbowTexture;

public:
    bool Init(ShaderFactory* shaderFactory);
    void Render();
};

