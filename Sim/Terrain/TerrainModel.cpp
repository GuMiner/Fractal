#include <iostream>
#include <limits>
#include <glm\gtc\matrix_transform.hpp>
#include <gl/glew.h>
#include "../Data/BinaryModel.h"
#include "../GLCore/VectorMath.h"
#include "../Telemetry/Logger.h"
#include "TerrainModel.h"


TerrainModel::TerrainModel() {
}

bool TerrainModel::Load(int tileX, int tileY) {
    std::stringstream loadPath;
    loadPath << "Config/Terrain/Generated/" << tileY << "/" << tileX << "-8.off"; // TODO figure out mips
    if (!BinaryModel::Load(loadPath.str(), vertices, faces))
    {
        Logger::LogError("Cannot read test input file");
        return false;
    }

    VectorMath::ComputeNormals(vertices, faces, normals);

    // Compute test scaling factors
    float minZ = std::numeric_limits<float>::max();
    float maxZ = std::numeric_limits<float>::min();
    for (int i = 0; i < vertices.size(); i++) {
        minZ = std::min(minZ, vertices[i].z);
        maxZ = std::max(maxZ, vertices[i].z);
    }

    // Scale from 0-5
    scaleFactor = 5.0 / (maxZ - minZ);
    offsetFactor = -maxZ;
    // std::cout << scaleFactor << " " << offsetFactor << " " << std::endl;
    

    // Create new OpenGL primitives
    // TODO some of this should be extracted away as this needs to exist per model object
    glGenVertexArrays(1, &modelVao);
    glBindVertexArray(modelVao);

    glGenBuffers(1, &positionVbo);
    glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &normalVbo);
    glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &indexVbo);

    glBindVertexArray(0);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << "Init: " << error << std::endl;
        return false;
    }
    return true;
}

bool TerrainModel::SendMesh() {
    glBindVertexArray(modelVao);

    glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(glm::ivec3), &faces[0], GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Order important!
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << "Init: " << error << std::endl;
        return false;
    }
    return true;
}

GLuint TerrainModel::GetVao() {
    return modelVao;
}

GLsizei TerrainModel::GetFaceCount() {
    return faces.size();
}