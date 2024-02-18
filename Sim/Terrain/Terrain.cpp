#include <fstream>
#include <limits>
#include <iostream>
#include <glm\gtc\matrix_transform.hpp>
#include "Terrain.h"

Terrain::Terrain() {
	renderer = new TerrainRenderer();

	std::ifstream f("Config/terrain.json");
	config = json::parse(f).template get<TerrainConfig>();

	xTileRatio = (float)config.scaleX / (float)(config.width * config.MinMipsLevel());
	yXRatio = (float)config.scaleY / (float)config.scaleX;
	scaleZ = (float)config.scaleZ / (float)config.maxHeight;

	lastCameraTileX = -1;
	lastCameraTileY = -1;
}

int Terrain::GetTileIndex(int x, int y, int mipsLevel) {
	int baseIndex = x + y * config.width;
	int multipliedIndex = baseIndex * config.mipsLevels.size();
	return multipliedIndex + config.MipsLevelIndex(mipsLevel);
}


TerrainModel* Terrain::EnsureTileCached(int x, int y, int mipsLevel) {
	int tileIdx = GetTileIndex(x, y, mipsLevel);
	if (models.count(tileIdx) != 0) {
		return models[tileIdx];
	}

	auto model = new TerrainModel();
	if (!model->Load(x, y, mipsLevel)) {
		return nullptr;
	}

	if (!model->SendMesh()) {
		return nullptr;
	}

	models[tileIdx] = model;
	return model;
}

bool Terrain::Init(ShaderFactory* shaderFactory) {
	if (!renderer->Init(shaderFactory)) {
		return false;
	}

	int minMipsLevel = config.MinMipsLevel();
	for (int x = 0; x < config.width; x++) {
		std::cout << "Loaded basic terrain column " << x << std::endl;
		for (int y = 0; y < config.height; y++) {
			auto model = EnsureTileCached(x, y, minMipsLevel);
			if (model == nullptr) {
				return false;
			}
		}
	}

	return true;
}

void Terrain::Render(Camera* camera) {
	int minMipsLevel = config.MinMipsLevel();
	int totalMipsLevels = config.mipsLevels.size();

	// Scale for apx 1ft x 1ft x 1ft dimensions
	// Z-fighting -- dropped down to 100ft x 100ft
	float scaleX = xTileRatio;
	float scaleY = yXRatio * xTileRatio;

	float offsetXMultipler = minMipsLevel * scaleX;
	float offsetYMultipler = minMipsLevel * scaleY;

	int cameraTileX = camera->Position().x / offsetXMultipler;
	int cameraTileY = camera->Position().y / offsetYMultipler;
	if (cameraTileX != lastCameraTileX || cameraTileY != lastCameraTileY) {
		lastCameraTileX = cameraTileX;
		lastCameraTileY = cameraTileY;
		std::cout << "T: " << lastCameraTileX << ", " << lastCameraTileY << std::endl;
	}

	renderer->StartRender(camera);
	for (int x = 0; x < config.width; x++) {
		for (int y = 0; y < config.height; y++) {

			// TODO don't hardcode total number of levels and also preload many of the level details
			// TODO need more details in front of the player instead of just underneath the player
			// TODO definitely need these to be preloaded
			int mipsLevel = minMipsLevel;
			if (x == cameraTileX && y == cameraTileY) {
				mipsLevel = config.mipsLevels[1]; // Center, 128
			} else if (abs(x - cameraTileX) <= 1 && abs(y - cameraTileY) <= 1) {
				mipsLevel = config.mipsLevels[2]; // Center - 1, 64
			} else if (abs(x - cameraTileX) <= 2 && abs(y - cameraTileY) <= 2) {
				mipsLevel = config.mipsLevels[3]; // Center - 2, 32
			} else if (abs(x - cameraTileX) <= 3 && abs(y - cameraTileY) <= 3) {
				mipsLevel = config.mipsLevels[4]; // Center - 3, 16
			}

			TerrainModel* model = EnsureTileCached(x, y, mipsLevel);
			assert(model != nullptr);

			float mipsScale = (float)minMipsLevel / (float)mipsLevel;
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(
				scaleX * mipsScale, scaleY * mipsScale, scaleZ
			));

			glm::mat4 offset = glm::translate(glm::mat4(1.0f), glm::vec3(x * offsetXMultipler, y * offsetYMultipler, 1.0f));
			renderer->Render(camera, offset * scale, model);
		}
	}

	renderer->StopRender();
}