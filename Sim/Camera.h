#pragma once
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include "KeyboardInput.h"

class Camera {
    float fovY;

    float aspectRatio;
    float nearPlane;
    float farPlane;

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
    bool DialKey(sf::Keyboard::Key posKeyId, sf::Keyboard::Key negKeyId, T amount, T* variable) const {
        bool updated = false;
        if (KeyboardInput::IsKeyPressed(posKeyId)) {
            (*variable) += amount;
            updated = true;
        }

        if (KeyboardInput::IsKeyPressed(negKeyId)) {
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
    void Update();
    glm::vec3 Position();
    glm::vec3 Forwards();
};

