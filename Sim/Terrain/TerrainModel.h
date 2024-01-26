#pragma once
#include <igl/readOFF.h>
#include "../GLCore/ShaderFactory.h"
#include "../Camera.h"

class TerrainModel
{
    GLuint modelProgram;

    GLuint modelVao;
    GLuint positionVbo;
    GLuint normalVbo;
    GLuint indexVbo;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::ivec3> faces;

    float scaleFactor;
    float offsetFactor;

public:
    TerrainModel();
    bool Init(ShaderFactory* shaderFactory);
    bool SendMesh();
    void Render(Camera* camera, float currentTime);
};