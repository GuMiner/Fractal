#pragma once
#include "../GLCore/ShaderFactory.h"
#include "../GLCore/ShaderUniforms.h"
#include "../Camera.h"

class InfiniGrid {
    GLuint gridProgram;
    ShaderUniforms uniforms;

    GLuint partialCubeVao;
    GLuint partialCubeVbo;
    GLuint partialCubeNormalsVbo;

    float tileSize;

    std::vector<glm::vec3> planeInstance;
    std::vector<glm::vec3> planeInstanceNormals;

public:
    InfiniGrid();
    bool Init(ShaderFactory* shaderFactory);
    void Render(Camera* camera);

    ~InfiniGrid();
};