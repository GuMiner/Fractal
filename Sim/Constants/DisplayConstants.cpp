#include "DisplayConstants.h"

float DisplayConstants::FOV_Y;
float DisplayConstants::ASPECT;
float DisplayConstants::NEAR_PLANE;
float DisplayConstants::FAR_PLANE;
glm::mat4 DisplayConstants::PerspectiveMatrix;

DisplayConstants::DisplayConstants()
{
    FOV_Y = 50.0f;
    ASPECT = 1.77778f; // 16:9
    NEAR_PLANE = 0.10f;
    FAR_PLANE = 1000.0f;

    PerspectiveMatrix = glm::mat4(); //  MatrixOps::Perspective(Constants::FOV_Y, Constants::ASPECT, Constants::NEAR_PLANE, Constants::FAR_PLANE);
}
