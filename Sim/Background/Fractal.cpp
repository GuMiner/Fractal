#include <iostream>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/gtx/color_space.hpp>
#include <gl/glew.h>
#include "../Time.h"
#include "Fractal.h"

bool Fractal::Init(ShaderFactory* shaderFactory) {
    if (!shaderFactory->CreateShaderProgram("Config/Shaders/juliaFractal", &programId)) {
        std::cout << "Cannot load fractal" << std::endl;
        return false;
    }

    // Rainbow texture for the fractal
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &rainbowTexture);

    std::vector<glm::vec3> gradient;
    int steps = 256;
    for (int i = 0; i < steps; i++)
    {
        // Full HSV goes from 0 to 260. We stop at mid-purple.
        float hue = 320.0f * ((float)i / (float)steps);
        gradient.push_back(glm::rgbColor(glm::vec3(hue, 1.0f, 1.0f)));
    }

    glBindTexture(GL_TEXTURE_1D, rainbowTexture);
    glTexStorage1D(GL_TEXTURE_1D, 1, GL_RGB32F, steps);
    glTexSubImage1D(GL_TEXTURE_1D, 0, 0, steps, GL_RGB, GL_FLOAT, &gradient[0]);

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
    glBindTexture(GL_TEXTURE_1D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "Init: " << error << std::endl;
        return false;
    }

    return true;
}

void Fractal::Render() {
    glUseProgram(programId);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_1D, rainbowTexture);

    GLint fractalGradientLoc = glGetUniformLocation(programId, "fractalGradient");
    glUniform1i(fractalGradientLoc, 0); //  Texture location - GL_TEXTURE0

    GLint maxIterationsLoc = glGetUniformLocation(programId, "maxIterations");
    glUniform1i(maxIterationsLoc, 400);

    GLint time = glGetUniformLocation(programId, "time");
    glUniform1f(time, Time::GlobalTime->GameTime() * 100.0f);

    GLint mousePos = glGetUniformLocation(programId, "c");
    glUniform2f(mousePos, -0.55f, 0.75f);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glUseProgram(0);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "Program Error:" << error << std::endl;
    }
}