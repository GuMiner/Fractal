#pragma once
#include "../GLCore/ShaderFactory.h"
#include "../Camera.h"

class TerrainModel {
    GLuint modelVao;
    GLuint positionVbo;
    GLuint indexVbo;
   // GLuint normalVbo;
    GLuint normalTexture;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::ivec3> faces;

public:

    float scaleFactor;
    float offsetFactor; // TODO shared scale and offset
    TerrainModel();
    bool Load(int tileX, int tileY, int mipsLevel);
    bool SendMesh();

    GLuint GetVao();
    GLuint GetNormalTexture();
    GLsizei GetFaceCount();
};