#include "Camera.h"
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(): 
	position(glm::vec3(0, 0, -5.0f)), forwards(glm::vec3(0, 0, 1)), up(glm::vec3(0, 1, 0)),
	fovY(30.0f), aspectRatio(1.77778f), nearPlane(0.10f), farPlane(1000.0f) {
	ComputeNormals();

	// TODO get view settings from config
	View = glm::lookAtLH(position, target, up);
	Perspective = glm::perspectiveLH(glm::radians(fovY), aspectRatio, nearPlane, farPlane);
}

void Camera::ComputeNormals()
{
    up = glm::normalize(up);
    forwards = glm::normalize(forwards);

    right = glm::cross(up, forwards); // In LH geometry this gives us the X+ normalized vector
    target = position + forwards;
}