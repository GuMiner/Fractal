#pragma once
#include "../GLCore/ShaderFactory.h"
#include "../Camera.h"

class InfiniGrid {
    GLsizei instanceCount;

    GLuint gridProgram;
    GLuint partialCubeVao;
    GLuint partialCubeVbo;

public:
    InfiniGrid();
    bool Init(ShaderFactory* shaderFactory);
    void Render(Camera* camera);

    ~InfiniGrid();
};