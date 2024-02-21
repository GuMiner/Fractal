#pragma once
#include <sstream>
#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <nlohmann/json.hpp>
#include "../StringExtensions.h"

using json = nlohmann::json;

struct SaveConfig {
	glm::vec3 position;
	glm::vec3 forwards;
	glm::vec3 up;
};

void ParseVector(const json& j, const char* field, glm::vec3& vector);

void from_json(const json& j, SaveConfig& c);

namespace nlohmann {
	template <>
	struct adl_serializer<glm::vec3> {
		static void to_json(json& j, const glm::vec3& opt) {
			std::stringstream glmStream;
			glmStream << opt.x << ", " << opt.y << ", " << opt.z;
			j = glmStream.str();
		}
	};
}

void to_json(json& j, const SaveConfig& c);