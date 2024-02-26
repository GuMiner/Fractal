#include <fstream>
#include <iostream>
#include <glm/geometric.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Time.h"
#include "Data/Config/CameraConfig.h"
#include "Camera.h"

Camera::Camera() {
	std::ifstream f("Config/camera.json");
    CameraConfig config = json::parse(f).template get<CameraConfig>();
    
	position = config.position;
	forwards = config.forwards;
	up = config.up;

	fovY = config.fovY;
	aspectRatio = config.aspectRatio;
	nearPlane = config.nearPlane;

    UpdateNormalsAndMatrixes();

    speedMultiplier = 128.0;
    wasSpeedUpPressed = false;
    wasSpeedDownPressed = false;
}

void Camera::Reset(glm::vec3 position, glm::vec3 forwards, glm::vec3 up) {
    this->position = position;
    this->forwards = forwards;
    this->up = up;
}

void Camera::UpdateNormalsAndMatrixes() {
    up = glm::normalize(up);
    forwards = glm::normalize(forwards);

    right = glm::cross(up, forwards); // In LH geometry this gives us the X+ normalized vector
    target = position + forwards;

    View = glm::lookAtLH(position, target, up);
    RotationOnlyView = glm::lookAtLH(glm::vec3(0, 0, 0), forwards, up);
    Perspective = glm::infinitePerspectiveLH(glm::radians(fovY), aspectRatio, nearPlane);
}

// TODO put in constants elsewhere
#define MOUSE_ROTATION_SPEED 0.0015f
#define KEYBOARD_MOVEMENT_SPEED 0.8f
#define KEYBOARD_ROLL_SPEED 0.5f

bool Camera::CheckMouseRotation() {
    bool rotatedAroundUp = false;
    bool rotatedAroundRight = false;

    glm::vec2 difference = Input::GetMouseDelta();

    glm::vec2 rotation = MOUSE_ROTATION_SPEED * glm::vec2(-(float)difference.x, -(float)difference.y);
    if (difference.x != 0) {
        forwards = glm::rotate(forwards, rotation.x, up);
        rotatedAroundUp = true;
    }

    if (difference.y != 0) {
        up = glm::rotate(up, rotation.y, right);
        forwards = glm::rotate(forwards, rotation.y, right);
        rotatedAroundRight = true;
    }
    return rotatedAroundUp || rotatedAroundRight;
}


void Camera::Update() {
    float frameTime = std::max(0.1f, Time::GlobalTime->LastFrameInterval());

    if (Input::IsKeyPressed(GLFW_KEY_1) && !wasSpeedDownPressed) {
        wasSpeedDownPressed = true;
        speedMultiplier /= 2;
        std::cout << speedMultiplier << std::endl;
    }
    else if (!Input::IsKeyPressed(GLFW_KEY_1) && wasSpeedDownPressed) {
        wasSpeedDownPressed = false;
    }

    if (Input::IsKeyPressed(GLFW_KEY_2) && !wasSpeedUpPressed) {
        wasSpeedUpPressed = true;
        speedMultiplier *= 2;
        std::cout << speedMultiplier << std::endl;
    }
    else if (!Input::IsKeyPressed(GLFW_KEY_2) && wasSpeedUpPressed) {
        wasSpeedUpPressed = false;
    }

    float speed = frameTime * KEYBOARD_MOVEMENT_SPEED * speedMultiplier; // TODO constant somewhere, or configurable speed
    float SIDEWAYS_DIVISOR = 4;
    bool movedForwards = DialKey(GLFW_KEY_A, GLFW_KEY_Z, speed * forwards, &position);
    bool movedLeftRight = DialKey(GLFW_KEY_W, GLFW_KEY_Q, speed * right / SIDEWAYS_DIVISOR, &position);
    bool movedUpDown = DialKey(GLFW_KEY_S, GLFW_KEY_X, speed * up / SIDEWAYS_DIVISOR, &position);

    bool mouseRotated = CheckMouseRotation();

    bool keyRotated = false;
    if (Input::IsKeyPressed(GLFW_KEY_E)) {
        up = glm::rotate(up, KEYBOARD_ROLL_SPEED * frameTime, forwards);
        keyRotated = true;
    }

    if (Input::IsKeyPressed(GLFW_KEY_D)) {
        up = glm::rotate(up, -KEYBOARD_ROLL_SPEED * frameTime, forwards);
        keyRotated = true;
    }

    if (movedForwards || movedLeftRight || movedUpDown || keyRotated || mouseRotated) {
        UpdateNormalsAndMatrixes();
    }
}

glm::vec3 Camera::Position() {
    return position;
}

glm::vec3 Camera::Forwards() {
    return forwards;
}

glm::vec3 Camera::Up() {
    return up;
}