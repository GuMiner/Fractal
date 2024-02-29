#pragma once
#include "../GLCore/ShaderFactory.h"
#include "../GLCore/ShaderUniforms.h"
#include "../Camera.h"
#include "TerrainModel.h"

class TerrainRenderer
{
    GLuint modelProgram;
    ShaderUniforms uniforms;

    GLint modelShaderPos;
    GLint normalMatrixShaderPos;

public:
    TerrainRenderer();
    bool Init(ShaderFactory* shaderFactory);
    void StartRender(Camera* camera, float maxHeight);
    void StopRender();
    void Render(Camera* camera, glm::mat4 position, TerrainModel* model);
};

