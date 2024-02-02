#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct TerrainConfig {
	int width;
	int height;
	std::vector<int> mipsLevels;
	std::string sourcePath;
	std::string generatedPath;

	// Y offset of the current mips image
	int GetMipsYOffset(int mipsLevel);

	// Total image height 
	int TotalMipsHeight();
};

void from_json(const json& j, TerrainConfig& c);