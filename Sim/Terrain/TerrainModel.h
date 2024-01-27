#pragma once
#include "../GLCore/ShaderFactory.h"
#include "../Camera.h"

class TerrainModel {
    GLuint modelVao;
    GLuint positionVbo;
    GLuint normalVbo;
    GLuint indexVbo;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::ivec3> faces;

public:

    float scaleFactor;
    float offsetFactor; // TODO shared scale and offset
    TerrainModel();
    bool Load(int tileX, int tileY);
    bool SendMesh();

    GLuint GetVao();
    GLsizei GetFaceCount();
};