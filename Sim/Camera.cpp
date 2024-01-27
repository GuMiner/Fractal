#include <fstream>
#include <iostream>
#include <glm/geometric.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Data/Config/CameraConfig.h"
#include "Camera.h"

using Key = sf::Keyboard::Key;

Camera::Camera() {
	std::ifstream f("Config/camera.json");
    CameraConfig config = json::parse(f).template get<CameraConfig>();
    
	position = config.position;
	forwards = config.forwards;
	up = config.up;

	fovY = config.fovY;
	aspectRatio = config.aspectRatio;
	nearPlane = config.nearPlane;
	farPlane = config.farPlane;

    lastFrameTime = -1;
    UpdateNormalsAndMatrixes();

    speedMultiplier = 1.0;
    wasSpeedUpPressed = false;
    wasSpeedDownPressed = false;
}

void Camera::UpdateNormalsAndMatrixes() {
    up = glm::normalize(up);
    forwards = glm::normalize(forwards);

    right = glm::cross(up, forwards); // In LH geometry this gives us the X+ normalized vector
    target = position + forwards;

    View = glm::lookAtLH(position, target, up);
    Perspective = glm::infinitePerspectiveLH(glm::radians(fovY),
        aspectRatio, nearPlane);// glm::perspectiveLH(, aspectRatio, nearPlane, farPlane);
}

// TODO put in constants elsewhere
#define MOUSE_ROTATION_SPEED 0.0015f
#define KEYBOARD_MOVEMENT_SPEED 0.8f
#define KEYBOARD_ROLL_SPEED 0.5f

bool Camera::CheckMouseRotation() {
    bool rotatedAroundUp = false;
    bool rotatedAroundRight = false;

    glm::ivec2 difference = KeyboardInput::GetMouseDelta();

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


void Camera::Update(float currentTime) {
    float frameTime = std::max(0.1f, currentTime - lastFrameTime);
    lastFrameTime = currentTime;

    if (KeyboardInput::IsKeyPressed(Key::Num1) && !wasSpeedDownPressed) {
        wasSpeedDownPressed = true;
        speedMultiplier /= 2;
        std::cout << speedMultiplier << std::endl;
    }
    else if (!KeyboardInput::IsKeyPressed(Key::Num1) && wasSpeedDownPressed) {
        wasSpeedDownPressed = false;
    }

    if (KeyboardInput::IsKeyPressed(Key::Num2) && !wasSpeedUpPressed) {
        wasSpeedUpPressed = true;
        speedMultiplier *= 2;
        std::cout << speedMultiplier << std::endl;
    }
    else if (!KeyboardInput::IsKeyPressed(Key::Num2) && wasSpeedUpPressed) {
        wasSpeedUpPressed = false;
    }

    float speed = frameTime * KEYBOARD_MOVEMENT_SPEED * speedMultiplier; // TODO constant somewhere, or configurable speed
    float SIDEWAYS_DIVISOR = 4;
    bool movedForwards = DialKey(Key::A, Key::Z, speed * forwards, &position);
    bool movedLeftRight = DialKey(Key::W, Key::Q, speed * right / SIDEWAYS_DIVISOR, &position);
    bool movedUpDown = DialKey(Key::S, Key::X, speed * up / SIDEWAYS_DIVISOR, &position);

    bool mouseRotated = CheckMouseRotation();

    bool keyRotated = false;
    if (KeyboardInput::IsKeyPressed(Key::E)) {
        up = glm::rotate(up, KEYBOARD_ROLL_SPEED * frameTime, forwards);
        keyRotated = true;
    }

    if (KeyboardInput::IsKeyPressed(Key::D)) {
        up = glm::rotate(up, -KEYBOARD_ROLL_SPEED * frameTime, forwards);
        keyRotated = true;
    }

    if (movedForwards || movedLeftRight || movedUpDown || keyRotated || mouseRotated) {
        UpdateNormalsAndMatrixes();
    }
}