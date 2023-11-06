#pragma once
#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <nlohmann/json.hpp>
#include "../StringExtensions.h"

using json = nlohmann::json;

struct CameraConfig {
	glm::vec3 position;
	glm::vec3 forwards;
	glm::vec3 up;
	bool letterbox;
	float aspectRatio;
	float fovY;
	float nearPlane;
	float farPlane;
};

void ParseVector(const json& j, const char* field, glm::vec3& vector) {
	std::string fieldStr;
	j.at(field).get_to(fieldStr);

	std::vector<std::string> parts;
	StringExtensions::Split(fieldStr, ',', true, parts);
	vector = glm::vec3(
		strtof(parts[0].c_str(), nullptr),
		strtof(parts[1].c_str(), nullptr),
		strtof(parts[2].c_str(), nullptr));
}

void from_json(const json& j, CameraConfig& c) {
	ParseVector(j, "position", c.position);
	ParseVector(j, "forwards", c.forwards);
	ParseVector(j, "up", c.up);

	j.at("letterbox").get_to(c.letterbox);
	j.at("aspectRatio").get_to(c.aspectRatio);
	j.at("fovY").get_to(c.fovY);
	j.at("nearPlane").get_to(c.nearPlane);
	j.at("farPlane").get_to(c.farPlane);
}