#pragma once
#include "../GLCore/ShaderFactory.h"
#include "../Camera.h"

class TerrainModel {
    int mipsLevel;
    bool readyToSync;
    bool readyToRender;

    GLuint modelVao;
    GLuint positionVbo;
    GLuint indexVbo;
    GLuint normalTexture;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::ivec3> faces;

public:
    TerrainModel(int mipsLevel);
    bool Load(int tileX, int tileY);
    bool SendMesh();

    GLuint GetVao();
    GLuint GetNormalTexture();
    GLsizei GetFaceCount();

    int GetMipsLevel();
    bool ReadyToSync();
    bool ReadyToRender();
};