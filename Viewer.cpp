#include <glm/gtc/matrix_transform.hpp>
#include "logging\Logger.h"
#include "DescentMotionTranslator.h"
#include "Viewer.h"

Viewer::Viewer()
    : fovY(30.0f), aspectRatio(1.77778f), nearPlane(0.10f), farPlane(1000.0f), 
      ScreenWidth(1280), ScreenHeight(720), MaxFramerate(120), camera(), motionTranslator(new DescentMotionTranslator())
{
    UpdateMatrices();
}

Viewer::~Viewer()
{
    delete motionTranslator;
}

void Viewer::Update(float frameTime)
{
    if (motionTranslator->Update(&camera, frameTime))
    {
        UpdateMatrices();
    }
}

void Viewer::SetScreenSize(int width, int height)
{
    ScreenWidth = width;
    ScreenHeight = height;
}

float Viewer::GetAspectRatio() const
{
    return aspectRatio;
}

void Viewer::UpdateMatrices()
{
    viewMatrix = glm::lookAtLH(camera.position, camera.target, camera.up);
    perspectiveMatrix = glm::perspectiveLH(glm::radians(fovY), aspectRatio, nearPlane, farPlane);
    Logger::LogDebug("View Position ", camera.position, ". Target ", camera.target, ". Up ", camera.up, ".");
    Logger::LogDebug("View Projection: Aspect ", aspectRatio, ". FOV-Y: ", fovY, ". Near Plane: ", nearPlane, ". Far Plane: ", farPlane, ".");
}