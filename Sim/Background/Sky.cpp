#include <iostream>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/gtx/color_space.hpp>
#include <gl/glew.h>
#include "Sky.h"

bool Sky::Init(ShaderFactory* shaderFactory) {
    if (!shaderFactory->CreateShaderProgram("Config/Shaders/sky", &programId)) {
        std::cout << "Cannot load fractal" << std::endl;
        return false;
    }

    // Rainbow texture for the fractal
    // Full-screen quad
    std::vector<glm::vec2> cwQuad;
    cwQuad.push_back(glm::vec2(1, 1));
    cwQuad.push_back(glm::vec2(1, -1));
    cwQuad.push_back(glm::vec2(-1, 1));
    cwQuad.push_back(glm::vec2(-1, 1));
    cwQuad.push_back(glm::vec2(1, -1));
    cwQuad.push_back(glm::vec2(-1, -1));
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, nullptr);

    //2 -- 2 floats / vertex. 4 -- float32
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * 4, &cwQuad[0], GL_STATIC_DRAW);
    
    // Reset everything to not interfere with SFML
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "Init: " << error << std::endl;
        return false;
    }  

    return true;
}

void Sky::Render(float currentTime) {
    glUseProgram(programId);

    // TODO: Temporary before camera movement is in place
    GLint time = glGetUniformLocation(programId, "time");
    glUniform1f(time, currentTime * 0.5f);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glUseProgram(0);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "Program Error:" << error << std::endl;
    }
}