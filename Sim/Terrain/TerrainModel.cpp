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

bool TerrainModel::Init(ShaderFactory* shaderFactory) {
    if (!shaderFactory->CreateShaderProgram("Config/Shaders/model", &modelProgram))
    {
        Logger::LogError("Failed to load the model rendering shader; cannot continue.");
        return false;
    }

    if (!BinaryModel::Load("Config/Terrain/Generated/0/0-8.off", vertices, faces))
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
    std::cout << scaleFactor << " " << offsetFactor << " " << std::endl;
    

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
    glUseProgram(modelProgram);
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
    glUseProgram(0);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << "Init: " << error << std::endl;
        return false;
    }
    return true;
}

void TerrainModel::Render(Camera* camera, float currentTime) {
    glUseProgram(modelProgram);
    glBindVertexArray(modelVao);

    // Projection 
    auto position =
        glm::translate(
            glm::scale(glm::mat4(1.0), glm::vec3(1.0f, 1.0f, scaleFactor)), // 0.01f)), //glm::rotate(glm::mat4(1.0), currentTime * 0.5f, glm::vec3(0, 1, 0)),
            glm::vec3(0, 0, offsetFactor)); //-109544));
    GLint model = glGetUniformLocation(modelProgram, "model");
    glUniformMatrix4fv(model, 1, GL_FALSE, &position[0][0]);

    GLint view = glGetUniformLocation(modelProgram, "view");
    glUniformMatrix4fv(view, 1, GL_FALSE, &camera->View[0][0]);

    GLint perspective = glGetUniformLocation(modelProgram, "perspective");
    glUniformMatrix4fv(perspective, 1, GL_FALSE, &camera->Perspective[0][0]);

    GLint normalMatrix = glGetUniformLocation(modelProgram, "normalMatrix");
    glm::mat4 normalMatrixComputed = glm::transpose(glm::inverse(camera->View * position));

    glUniformMatrix4fv(normalMatrix, 1, GL_FALSE, &normalMatrixComputed[0][0]);


    // Lighting
    GLint ambient = glGetUniformLocation(modelProgram, "ambient");
    glUniform3f(ambient, 0.51f, 0.51f, 0.51f);

    GLint diffuse = glGetUniformLocation(modelProgram, "diffuse");
    glUniform3f(diffuse, 0.51f, 0.51f, 0.51f);

    GLint specularColor = glGetUniformLocation(modelProgram, "specularColor");
    glUniform3f(specularColor, 0.51f, 0.51f, 0.51f);

    GLint specular = glGetUniformLocation(modelProgram, "specular");
    glUniform1f(specular, 4.0f);

    GLint dLightDirection = glGetUniformLocation(modelProgram, "dLightDirection");
    auto lightDirection = glm::normalize(glm::vec3(1.0f, 2.0f, 1.0f));
    glUniform3f(dLightDirection, lightDirection.x, lightDirection.y, lightDirection.z);

    GLint dLightAmbient = glGetUniformLocation(modelProgram, "dLightAmbient");
    glUniform3f(dLightAmbient, 0.1f, 0.1f, 0.1f);

    GLint dLightDiffuse = glGetUniformLocation(modelProgram, "dLightDiffuse");
    glUniform3f(dLightDiffuse, 0.5f, 0.5f, 0.5f);

    GLint pLightPosition = glGetUniformLocation(modelProgram, "pLightPosition");
    auto lightPosition =
        glm::rotate(glm::mat4(1.0f), -2.1f * currentTime, glm::vec3(0, 1, 0))
        * glm::vec4(5.0f, 5.0f, -5.0f, 1.0f);
    glUniform3f(pLightPosition, lightPosition.x, lightPosition.y, lightPosition.z);

    GLint pLightAmbient = glGetUniformLocation(modelProgram, "pLightAmbient");
    glUniform3f(pLightAmbient, 0.2f, 0.2f, 0.2f);

    GLint pLightDiffuse = glGetUniformLocation(modelProgram, "pLightDiffuse");
    glUniform3f(pLightDiffuse, 0.8f, 0.8f, 0.8f);

    glDrawElements(GL_TRIANGLES, 3 * faces.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}