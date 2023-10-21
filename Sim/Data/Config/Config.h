#pragma once
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// A simple class for logging program events out to a file.
struct GraphicsConfig {
	int width;
	int height;
	int depthBits;
	int stencilBits;
	int antialiasingLevel;
	int openGlMajor;
	int openGlMinor;
};

struct RenderingConfig {
	bool letterbox;
	float aspectRatio;
	float fovY;
	float nearPlane;
	float farPlane;
};

struct DiagnosticsConfig {
	int width;
	int height;
};

void from_json(const json& j, GraphicsConfig& c) {
	j.at("width").get_to(c.width);
	j.at("height").get_to(c.height);
	j.at("depthBits").get_to(c.depthBits);
	j.at("stencilBits").get_to(c.stencilBits);
	j.at("antialiasingLevel").get_to(c.antialiasingLevel);
	j.at("openGlMajor").get_to(c.openGlMajor);
	j.at("openGlMinor").get_to(c.openGlMinor);
}

void from_json(const json& j, RenderingConfig& c) {
	j.at("letterbox").get_to(c.letterbox);
	j.at("aspectRatio").get_to(c.aspectRatio);
	j.at("fovY").get_to(c.fovY);
	j.at("nearPlane").get_to(c.nearPlane);
	j.at("farPlane").get_to(c.farPlane);
}

void from_json(const json& j, DiagnosticsConfig& c) {
	j.at("width").get_to(c.width);
	j.at("height").get_to(c.height);
}
