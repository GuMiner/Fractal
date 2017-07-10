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
    cameraOrientationLocation = glGetUniformLocation(programId, "cameraOrientation");
    aspectRatioLocation = glGetUniformLocation(programId, "aspectRatio");
    fovYLocation = glGetUniformLocation(programId, "fovY");

    return true;
}

void BasicTerrainExtension::Render()
{
    glUniformMatrix4fv(cameraOrientationLocation, 1, GL_FALSE, &viewer->viewMatrix[0][0]);
    glUniform1f(aspectRatioLocation, viewer->GetAspectRatio());
    glUniform1f(fovYLocation, viewer->GetFovY());
}

BasicTerrainExtension::~BasicTerrainExtension()
{
}