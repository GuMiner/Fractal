#pragma once
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct PreprocessorConfig {
	int width;
	int height;
};

void from_json(const json& j, PreprocessorConfig& c) {
	j.at("width").get_to(c.width);
	j.at("height").get_to(c.height);
}
