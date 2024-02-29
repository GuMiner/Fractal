#include <iostream>
#include <limits>
#include <glm\gtc\matrix_transform.hpp>
#include <gl/glew.h>
#include "../Data/BinaryModel.h"
#include "../GLCore/VectorMath.h"
#include "../Telemetry/Logger.h"
#include "TerrainModel.h"


TerrainModel::TerrainModel(int mipsLevel): 
    mipsLevel(mipsLevel), readyToSync(false), readyToRender(false) {
}

bool TerrainModel::Load(int tileX, int tileY) {
    std::stringstream loadPath;
    loadPath << "Config/Terrain/Generated/" << tileY << "/" << tileX << "-" << mipsLevel << ".off";
    if (!BinaryModel::LoadCompressed(loadPath.str(), vertices, faces))
    {
        Logger::LogError("Cannot read test input file");
        return false;
    }

    VectorMath::ComputeFaceNormals(vertices, faces, normals);
    assert(faces.size() == normals.size());
    assert(mipsLevel * mipsLevel * 2 == normals.size());

    readyToSync = true;
    return true;
}

bool TerrainModel::SendMesh() {
    // Prep VAO, VBO, normal texture
    glGenVertexArrays(1, &modelVao);
    glBindVertexArray(modelVao);

    glGenBuffers(1, &positionVbo);
    glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &indexVbo);

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &normalTexture);

    glBindTexture(GL_TEXTURE_2D, normalTexture);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, mipsLevel, mipsLevel * 2);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mipsLevel, mipsLevel * 2, GL_RGB, GL_FLOAT, &normals[0]);

    glBindTexture(GL_TEXTURE_2D, 0);

    // Send the data over to OpenGL / GPU
    glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(glm::ivec3), &faces[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Order important!
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << "Terrain Init: " << error << " " << glewGetErrorString(error) << std::endl;
        return false;
    }

    readyToRender = true;
    return true;
}

GLuint TerrainModel::GetVao() {
    return modelVao;
}

GLuint TerrainModel::GetNormalTexture() {
    return normalTexture;
}

GLsizei TerrainModel::GetFaceCount() {
    return faces.size();
}

int TerrainModel::GetMipsLevel() {
    return mipsLevel;
}

bool TerrainModel::ReadyToSync() {
    return readyToSync;
}

bool TerrainModel::ReadyToRender() {
    return readyToRender;
}
