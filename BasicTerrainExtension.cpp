#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm\vec3.hpp>
#include "logging\Logger.h"
#include "ColorGradient.h"
#include "Input.h"
#include "BasicTerrainExtension.h"

BasicTerrainExtension::BasicTerrainExtension(Viewer* viewer) : viewer(viewer)
{
}

bool BasicTerrainExtension::Init(GLuint programId)
{
    cameraPositionLocation = glGetUniformLocation(programId, "cameraPosition");
    lookAtLocation = glGetUniformLocation(programId, "lookAt");
    upLocation = glGetUniformLocation(programId, "up");
    aspectRatioLocation = glGetUniformLocation(programId, "aspectRatio");
    fovYLocation = glGetUniformLocation(programId, "fovY");

    return true;
}

void BasicTerrainExtension::Render(float gameTime)
{
    Camera camera = viewer->GetCamera();
    glUniform3fv(cameraPositionLocation, 1, &camera.position[0]);
    glUniform3fv(lookAtLocation, 1, &camera.forwards[0]);
    glUniform3fv(upLocation, 1, &camera.up[0]);
    glUniform1f(aspectRatioLocation, viewer->GetAspectRatio());
    glUniform1f(fovYLocation, viewer->GetFovY());
}

BasicTerrainExtension::~BasicTerrainExtension()
{
}