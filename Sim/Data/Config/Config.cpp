#include "Config.h"

void from_json(const json& j, GraphicsConfig& c) {
	j.at("title").get_to(c.title);
	j.at("width").get_to(c.width);
	j.at("height").get_to(c.height);
	j.at("depthBits").get_to(c.depthBits);
	j.at("stencilBits").get_to(c.stencilBits);
	j.at("antialiasingLevel").get_to(c.antialiasingLevel);
	j.at("openGlMajor").get_to(c.openGlMajor);
	j.at("openGlMinor").get_to(c.openGlMinor);
}

void from_json(const json& j, DiagnosticsConfig& c) {
	j.at("width").get_to(c.width);
	j.at("height").get_to(c.height);
}