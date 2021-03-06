#include <imgui\imgui.h>
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

float Viewer::GetFovY() const
{
    return fovY;
}

Camera Viewer::GetCamera() const
{
    return camera;
}

void Viewer::UpdateMatrices()
{
    viewMatrix = glm::lookAtLH(camera.position, camera.target, camera.up);
    perspectiveMatrix = glm::perspectiveLH(glm::radians(fovY), aspectRatio, nearPlane, farPlane);
}

void Viewer::Render()
{
    ImGui::Begin("Camera");
    ImGui::InputFloat3("Position", &camera.position[0], 2);
    ImGui::InputFloat3("Look At", &camera.forwards[0], 2);
    ImGui::InputFloat3("Up", &camera.up[0], 2);
    ImGui::End();
}