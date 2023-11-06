#include <fstream>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
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
	farPlane = config.farPlane;
	
	ComputeNormals();

	// TODO get view settings from config
	View = glm::lookAtLH(position, target, up);
	Perspective = glm::perspectiveLH(glm::radians(fovY), aspectRatio, nearPlane, farPlane);
}

void Camera::ComputeNormals() {
    up = glm::normalize(up);
    forwards = glm::normalize(forwards);

    right = glm::cross(up, forwards); // In LH geometry this gives us the X+ normalized vector
    target = position + forwards;
}