#include <iostream>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/gtx/color_space.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>

#include "../Time.h"
#include "Cube.h"
#include "Sky.h"

bool Sky::Init(ShaderFactory* shaderFactory) {
    if (!shaderFactory->CreateShaderProgram("Config/Shaders/sky", &programId)) {
        std::cout << "Cannot load sky shader!" << std::endl;
        return false;
    }

    // Invert scale to swap the triangle orientation because we are in the cube
    Cube::Create(skyBox, -100000.0f);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);
    glEnableVertexAttribArray(0);
    glBufferData(GL_ARRAY_BUFFER, skyBox.size() * sizeof(glm::vec3), &skyBox[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Reset to not interfere with SFML
    glBindVertexArray(0);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "Init: " << error << std::endl;
        return false;
    }  

    return true;
}

void Sky::Render(Camera* camera) {
    glUseProgram(programId);

    GLint time = glGetUniformLocation(programId, "time");
    glUniform1f(time, Time::GlobalTime->GameTime());

    GLint view = glGetUniformLocation(programId, "view");
    glUniformMatrix4fv(view, 1, GL_FALSE, &camera->View[0][0]);

    GLint perspective = glGetUniformLocation(programId, "perspective");
    glUniformMatrix4fv(perspective, 1, GL_FALSE, &camera->Perspective[0][0]);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, skyBox.size());

    glBindVertexArray(0);
    glUseProgram(0);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "Program Error:" << error << std::endl;
    }
}