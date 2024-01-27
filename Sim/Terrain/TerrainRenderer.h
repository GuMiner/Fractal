#pragma once
#include "../GLCore/ShaderFactory.h"
#include "../Camera.h"
#include "TerrainModel.h"

class TerrainRenderer
{
    GLuint modelProgram;

public:
    TerrainRenderer();
    bool Init(ShaderFactory* shaderFactory);
    void Render(Camera* camera, float currentTime, TerrainModel* model, glm::mat4 position);
};

