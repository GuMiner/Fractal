#pragma once
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// A simple class for logging program events out to a file.
struct GraphicsConfig {
	int width;
	int height;
};

struct DiagnosticsConfig {
	int width;
	int height;
};

void from_json(const json& j, GraphicsConfig& c) {
	j.at("width").get_to(c.width);
	j.at("height").get_to(c.height);
}

void from_json(const json& j, DiagnosticsConfig& c) {
	j.at("width").get_to(c.width);
	j.at("height").get_to(c.height);
}
