#pragma once
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>

class Camera
{
    float fovY;

    float aspectRatio;
    float nearPlane;
    float farPlane;

    glm::vec3 position;
    glm::vec3 forwards; // Starts at Z+, normalized
    glm::vec3 up; // Y+, normalized

    glm::vec3 right; // X+, computed
    glm::vec3 target; // Where the camera points to, normalized.

    void ComputeNormals();
public:
	glm::mat4 Perspective;
	glm::mat4 View;

	Camera();
    
};

