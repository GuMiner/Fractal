#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm\vec3.hpp>
#include "logging\Logger.h"
#include "ColorGradient.h"
#include "Input.h"
#include "JuliaFractalExtension.h"

JuliaFractalExtension::JuliaFractalExtension(Viewer* viewer) : viewer(viewer), maxIterations(1000)
{
}

bool JuliaFractalExtension::Init(GLuint programId)
{
    cLocation = glGetUniformLocation(programId, "c");
    timeLocation = glGetUniformLocation(programId, "time");
    fractalGradientLocation = glGetUniformLocation(programId, "fractalGradient");
    maxIterationsLocation = glGetUniformLocation(programId, "maxIterations");

    std::vector<glm::vec3> gradient = ColorGradient::GetColorGradient(maxIterations, 1.0f, 1.0f);
    
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &fractalGradientTextureId);
    glBindTexture(GL_TEXTURE_1D, fractalGradientTextureId);
    glTexStorage1D(GL_TEXTURE_1D, 1, GL_RGB32F, maxIterations);
    glTexSubImage1D(GL_TEXTURE_1D, 0, 0, maxIterations, GL_RGB, GL_FLOAT, &gradient[0]);

    return true;
}

void JuliaFractalExtension::Render(float gameTime)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_1D, fractalGradientTextureId);
    glUniform1i(fractalGradientLocation, 0);
    

    glm::vec2 cPos = glm::vec2(Input::GetMousePos().x, Input::GetMousePos().y) / glm::vec2((float)viewer->ScreenWidth, (float)viewer->ScreenHeight);
    cPos -= glm::vec2(0.5f);

    glUniform2f(cLocation, cPos.x, cPos.y);
    glUniform1i(maxIterationsLocation, maxIterations);
    glUniform1f(timeLocation, gameTime);
}

JuliaFractalExtension::~JuliaFractalExtension()
{
    glDeleteTextures(1, &fractalGradientTextureId);
}