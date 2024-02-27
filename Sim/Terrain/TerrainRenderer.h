#pragma once
#include "../GLCore/ShaderFactory.h"
#include "../Camera.h"
#include "TerrainModel.h"

class TerrainRenderer
{
    GLuint modelProgram;

    GLint modelShaderPos;
    GLint normalMatrixShaderPos;

    void SetFloat(const char* name, float value);
    void SetVec3(const char* name, glm::vec3 value);

public:
    TerrainRenderer();
    bool Init(ShaderFactory* shaderFactory);
    void StartRender(Camera* camera, float maxHeight);
    void StopRender();
    void Render(Camera* camera, glm::mat4 position, TerrainModel* model);
};

