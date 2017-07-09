#include <glm\gtc\matrix_transform.hpp>
#include "logging\Logger.h"
#include "Input.h"
#include "Viewer.h"

// Default our view to -64 x to +64 x, -36 y to +36 y, with Z+ into the screen and y+ up.
Viewer::Viewer()
    : fovY(2 * 74.47589f), aspectRatio(1.77778f), nearPlane(0.10f), farPlane(1000.0f), 
      ScreenWidth(1280), ScreenHeight(720), MaxFramerate(120)
{
    position = glm::vec3(0, 0, -6.0f); // LH coordinate system, Z+ is into the screen.
    target = glm::vec3(0, 0, 0);
    up = glm::vec3(0, 1, 0);
    UpdateMatrices();
}

void Viewer::Update(float frameTime)
{
    bool movedLeftRight = CheckMoveAxis(GLFW_KEY_W, GLFW_KEY_Q, frameTime, &position.x, &target.x);
    bool movedUpDown = CheckMoveAxis(GLFW_KEY_S, GLFW_KEY_X, frameTime, &position.y, &target.y);

    bool movedInOut = CheckMoveAxis(GLFW_KEY_A, GLFW_KEY_Z, frameTime, &position.z, &target.z);
    if (movedInOut && position.z > -0.1f)
    {
        position.z = -0.1f;
        target.z = 9.9f;
    }

    if (movedLeftRight || movedUpDown || movedInOut)
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
    viewMatrix = glm::lookAtLH(position, target, up);
    perspectiveMatrix = glm::perspectiveLH(glm::radians(fovY), aspectRatio, nearPlane, farPlane);
    Logger::LogDebug("View Position ", position, ". Target ", target, ". Up ", up, ".");
    Logger::LogDebug("View Projection: Aspect ", aspectRatio, ". FOV-Y: ", fovY, ". Near Plane: ", nearPlane, ". Far Plane: ", farPlane, ".");
}

bool Viewer::CheckMoveAxis(int posKeyId, int negKeyId, float frameTime, float* eye, float* target) const
{
    const float motionSpeed = 2.0f;
    DialVariable(posKeyId, negKeyId, motionSpeed * frameTime, eye);
    return DialVariable(posKeyId, negKeyId, motionSpeed * frameTime, target);
}

// Dials a variable positively or negatively by an amount based on the key pressed.
bool Viewer::DialVariable(int posKeyId, int negKeyId, float dialAmount, float* value) const
{
    if (Input::IsKeyPressed(posKeyId))
    {
        (*value) += dialAmount;
        return true;
    }

    if (Input::IsKeyPressed(negKeyId))
    {
        (*value) -= dialAmount;
        return true;
    }

    return false;
}
