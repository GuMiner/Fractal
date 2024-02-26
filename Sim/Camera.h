#pragma once
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include "Input/Input.h"

class Camera {
    float fovY;

    float aspectRatio;
    float nearPlane;

    glm::vec3 position;
    glm::vec3 forwards; // Starts at Z+, normalized
    glm::vec3 up; // Y+, normalized

    glm::vec3 right; // X+, computed
    glm::vec3 target; // Where the camera points to, normalized.

    // Movement data
    float speedMultiplier;
    bool wasSpeedUpPressed;
    bool wasSpeedDownPressed;

    template <typename T>
    bool DialKey(GlfwKey posKeyId, GlfwKey negKeyId, T amount, T* variable) const {
        bool updated = false;
        if (Input::IsKeyPressed(posKeyId)) {
            (*variable) += amount;
            updated = true;
        }

        if (Input::IsKeyPressed(negKeyId)) {
            (*variable) -= amount;
            updated = true;
        }

        return updated;
    }

    bool CheckMouseRotation();
    void UpdateNormalsAndMatrixes();
public:
	glm::mat4 Perspective;
	glm::mat4 View;
    glm::mat4 RotationOnlyView;

	Camera();
    void Reset(glm::vec3 position, glm::vec3 forwards, glm::vec3 up);
    void Update();
    glm::vec3 Position();
    glm::vec3 Forwards();
    glm::vec3 Up();
};

