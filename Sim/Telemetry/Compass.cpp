#include <iostream>
#include <gl/glew.h>
#include <glm\gtc\matrix_transform.hpp>
#include "../Telemetry/Logger.h"
#include "Compass.h"

Compass::Compass(): vertices(), colors() {
}

bool Compass::Init(ShaderFactory* shaderFactory) {
    if (!shaderFactory->CreateShaderProgram("Config/Shaders/compass", &compassProgram))
    {
        Logger::LogError("Failed to load the compass rendering shader; cannot continue.");
        return false;
    }

    glGenVertexArrays(1, &tripleAxisVao);
    glBindVertexArray(tripleAxisVao);

    glGenBuffers(1, &positionVbo);
    glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &colorVbo);
    glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
    glVertexAttribPointer(1, 4, GL_FLOAT, false, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << "Compass Init: " << error << std::endl;
        return false;
    }
    return true;
}

bool Compass::SendMesh() {
    // Triple X/Y/Z axis mesh
    vertices.push_back(glm::vec3(0, 0, 0));
    vertices.push_back(glm::vec3(1, 0, 0));
    colors.push_back(glm::vec4(1, 1, 1, 1));
    colors.push_back(glm::vec4(1, 0, 0, 1));

    vertices.push_back(glm::vec3(0, 0, 0));
    vertices.push_back(glm::vec3(0, 1, 0));
    colors.push_back(glm::vec4(1, 1, 1, 1));
    colors.push_back(glm::vec4(0, 1, 0, 1));

    vertices.push_back(glm::vec3(0, 0, 0));
    vertices.push_back(glm::vec3(0, 0, 1));
    colors.push_back(glm::vec4(1, 1, 1, 1));
    colors.push_back(glm::vec4(0, 0, 1, 1));

    glBindVertexArray(tripleAxisVao);

    glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), &colors[0], GL_STATIC_DRAW);

    glBindVertexArray(0);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << "Compass Data Init: " << error << std::endl;
        return false;
    }
    return true;
}

void Compass::Render(Camera* camera) {
    glUseProgram(compassProgram);
    glBindVertexArray(tripleAxisVao);

    glm::mat4 compassLocation = glm::translate(
        glm::mat4(1.0f), camera->Position() + 10.0f * camera->Forwards());

    GLint model = glGetUniformLocation(compassProgram, "model");
    glUniformMatrix4fv(model, 1, GL_FALSE, &compassLocation[0][0]);

    GLint view = glGetUniformLocation(compassProgram, "view");
    glUniformMatrix4fv(view, 1, GL_FALSE, &camera->View[0][0]);

    GLint perspective = glGetUniformLocation(compassProgram, "perspective");
    glUniformMatrix4fv(perspective, 1, GL_FALSE, &camera->Perspective[0][0]);

    glLineWidth(3.0f);
    glDrawArrays(GL_LINES, 0, vertices.size());
    glLineWidth(1.0f);

    glBindVertexArray(0);
    glUseProgram(0);
}