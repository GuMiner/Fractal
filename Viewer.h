#pragma once
#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>

class Viewer
{
    float fovY;

    float aspectRatio;
    float nearPlane;
    float farPlane;

    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;

    bool CheckMoveAxis(int posKeyId, int negKeyId, float frameTime, float* eye, float* target) const;
    bool DialVariable(int posKeyId, int negKeyId, float dialAmount, float* value) const;
    void UpdateMatrices();

public:
    glm::mat4 perspectiveMatrix;
    glm::mat4 viewMatrix;

    int ScreenWidth;
    int ScreenHeight;
    int MaxFramerate;

    Viewer();

    // Updates the view position from user input.
    void Update(float frameTime);
    void SetScreenSize(int width, int height);

    float GetAspectRatio() const;
};

