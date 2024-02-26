#pragma once
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// A simple class for logging program events out to a file.
struct GraphicsConfig {
	std::string title;
	int width;
	int height;
	int depthBits;
	int stencilBits;
	int antialiasingLevel;
	int openGlMajor;
	int openGlMinor;
};

struct DiagnosticsConfig {
	int width;
	int height;
};

void from_json(const json& j, GraphicsConfig& c);

void from_json(const json& j, DiagnosticsConfig& c);

