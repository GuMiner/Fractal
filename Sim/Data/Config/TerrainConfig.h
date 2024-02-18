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
	int maxHeight;
	int scaleX;
	int scaleY;
	int scaleZ;

	// Y offset of the current mips image
	int GetMipsYOffset(int mipsLevel);

	// Index in mipsLevels of the mipsLevel
	int MipsLevelIndex(int mipsLevel);

	// Total image height 
	int TotalMipsHeight();

	int MinMipsLevel();
	int MaxMipsLevel();
};

void from_json(const json& j, TerrainConfig& c);