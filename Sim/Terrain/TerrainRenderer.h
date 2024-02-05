#pragma once
#include "../GLCore/ShaderFactory.h"
#include "../Camera.h"
#include "TerrainModel.h"

class TerrainRenderer
{
    GLuint modelProgram;

    GLint modelShaderPos;
    GLint normalMatrixShaderPos;

public:
    TerrainRenderer();
    bool Init(ShaderFactory* shaderFactory);
    void StartRender(Camera* camera, float currentTime);
    void StopRender();
    void Render(Camera* camera, glm::mat4 position, TerrainModel* model);
};

