#include "TerrainConfig.h"

// Y offset of the current mips image
int TerrainConfig::GetMipsYOffset(int mipsLevel) {
	int offset = 0;
	int idx = 0;
	while (mipsLevels[idx] != mipsLevel) {
		offset += mipsLevels[idx];
		idx++;
	}

	return offset;
}

int TerrainConfig::TotalMipsHeight() {
	int mipsHeight = 0;
	for (int mipsLevel : mipsLevels) {
		mipsHeight += mipsLevel;
	}

	return mipsHeight;
}


void from_json(const json& j, TerrainConfig& c) {
	j.at("width").get_to(c.width);
	j.at("height").get_to(c.height);
	j.at("mipsLevels").get_to(c.mipsLevels);
	j.at("sourcePath").get_to(c.sourcePath);
	j.at("generatedPath").get_to(c.generatedPath);
}