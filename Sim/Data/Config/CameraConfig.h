#pragma once
#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <nlohmann/json.hpp>
#include "../StringExtensions.h"

using json = nlohmann::json;


struct CameraConfig {
	glm::vec3 position;
	bool letterbox;
	float aspectRatio;
	float fovY;
	float nearPlane;
	float farPlane;
};

void from_json(const json& j, CameraConfig& c) {
	std::string positionStr;
	j.at("position").get_to(positionStr);
	 
	std::vector<std::string> parts;
	StringExtensions::Split(positionStr, ',', true, parts);
	c.position = glm::vec3(
		strtof(parts[0].c_str(), nullptr),
		strtof(parts[1].c_str(), nullptr),
		strtof(parts[2].c_str(), nullptr));

	j.at("letterbox").get_to(c.letterbox);
	j.at("aspectRatio").get_to(c.aspectRatio);
	j.at("fovY").get_to(c.fovY);
	j.at("nearPlane").get_to(c.nearPlane);
	j.at("farPlane").get_to(c.farPlane);
}