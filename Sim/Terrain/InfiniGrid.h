#pragma once
#include "../GLCore/ShaderFactory.h"
#include "../GLCore/ShaderUniforms.h"
#include "../Camera.h"

class InfiniGrid {
    GLuint gridProgram;
    ShaderUniforms uniforms;

    GLuint partialCubeVao;
    GLuint partialCubeVbo;

    float tileSize;

    std::vector<glm::vec3> planeInstance;

public:
    InfiniGrid();
    bool Init(ShaderFactory* shaderFactory);
    void Render(Camera* camera);

    ~InfiniGrid();
};