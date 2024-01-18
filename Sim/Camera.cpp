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
}

void Camera::UpdateNormalsAndMatrixes() {
    up = glm::normalize(up);
    forwards = glm::normalize(forwards);

    right = glm::cross(up, forwards); // In LH geometry this gives us the X+ normalized vector
    target = position + forwards;

    View = glm::lookAtLH(position, target, up);
    Perspective = glm::perspectiveLH(glm::radians(fovY), aspectRatio, nearPlane, farPlane);
}

// TODO put in constants elsewhere
#define MOUSE_ROTATION_SPEED 0.001f
#define KEYBOARD_MOVEMENT_SPEED 0.8f
#define KEYBOARD_ROLL_SPEED 0.5f

bool Camera::CheckMouseRotation() {
    bool rotatedAroundUp = false;
    bool rotatedAroundRight = false;
    if (KeyboardInput::IsMouseButtonPressed(sf::Mouse::Button::Right)) {
        if (!wasMouseDown) {
            wasMouseDown = true;
            lastMousePos = KeyboardInput::GetMousePos();
            return true;
        }

        glm::ivec2 currentMousePos = KeyboardInput::GetMousePos();
        glm::ivec2 difference = currentMousePos - lastMousePos;
        lastMousePos = currentMousePos;

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
    }
    else {
        wasMouseDown = false;
    }

    return rotatedAroundUp || rotatedAroundRight;
}


void Camera::Update(float currentTime)
{
    float frameTime = std::max(0.1f, currentTime - lastFrameTime);
    lastFrameTime = currentTime;


    float speed = frameTime * KEYBOARD_MOVEMENT_SPEED; // TODO constant somewhere, or configurable speed

    bool movedForwards = DialKey(Key::A, Key::Z, speed * forwards, &position);
    bool movedLeftRight = DialKey(Key::W, Key::Q, speed * right, &position);
    bool movedUpDown = DialKey(Key::S, Key::X, speed * up, &position);

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