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

void Terrain::LoadTerrainAsync() {
	// Iterate from low->high resolution
	for (int i = config.mipsLevels.size() - 1; i >= 0; i--) {
		int mipsLevel = config.mipsLevels[i];
		for (int x = 0; x < config.width; x++) {
			for (int y = 0; y < config.height; y++) {
				TerrainModel* model = new TerrainModel(mipsLevel);
				bool result = model->Load(x, y);
				assert(result);

				models[GetTileIndex(x, y, mipsLevel)] = model;

				if (stopSync) {
					std::cout << "Stopping terrain sync." << std::endl;
					return;
				}
			}
		}

		std::cout << "Loaded all terrain of resolution " << mipsLevel << std::endl;
	}
}

// Empirical, may change
const int MAX_SYNC_PER_FRAME = 500;

TerrainModel* Terrain::EnsureTileCached(int x, int y, int mipsIndex, int* gpuCounter) {
	while (mipsIndex < config.mipsLevels.size()) {
		TerrainModel* model = TryCacheTile(x, y, mipsIndex, gpuCounter);
		if (model != nullptr) {
			return model;
		}

		// Try lower resolution models if none available
		mipsIndex++;
	}

	// No models fully loaded, ready to sync or MAX_SYNC_PER_FRAME exceeded
	return nullptr;
}

TerrainModel* Terrain::TryCacheTile(int x, int y, int mipsIndex, int* gpuCounter) {
	int mipsLevel = config.mipsLevels[mipsIndex];
	int tileIdx = GetTileIndex(x, y, mipsLevel);
	if (models.count(tileIdx) != 0) {
		auto model = models[tileIdx];
		if (!model->ReadyToRender() && model->ReadyToSync()) {
			if (*gpuCounter < MAX_SYNC_PER_FRAME) {
				(*gpuCounter)++;
				bool result = model->SendMesh();
				assert(result);
			}
		}

		// May still be false if MAX_SYNC_PER_FRAME was hit
		if (model->ReadyToRender()) {
			return model;
		}
	}

	return nullptr;
}

bool Terrain::Init(ShaderFactory* shaderFactory) {
	if (!renderer->Init(shaderFactory)) {
		return false;
	}

	stopSync = false;
	terrainLoader = new std::thread(&Terrain::LoadTerrainAsync, this);

	return true;
}

bool Terrain::WithinRegion(int x, int y, int maxDist) {
	return abs(x - lastCameraTileX) <= maxDist && abs(y - lastCameraTileY) <= maxDist;
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

	int modelsSentToGpu = 0;
	renderer->StartRender(camera);
	for (int x = 0; x < config.width; x++) {
		for (int y = 0; y < config.height; y++) {

			// TODO don't hardcode total number of levels
			// TODO need more details in front of the player instead of just underneath the player
			int mipsIndex = config.mipsLevels.size() - 1;
			if (WithinRegion(x, y, 2)) {
				mipsIndex = 0; // 256
			} else if (WithinRegion(x, y, 4)) {
				mipsIndex = 1; // 128
			} else if (WithinRegion(x, y, 6)) {
				mipsIndex = 2; // 64
			} else if (WithinRegion(x, y, 8)) {
				mipsIndex = 3; // 32
			} else if (WithinRegion(x, y, 10)) {
				mipsIndex = 4; // 16
			}

			TerrainModel* model = EnsureTileCached(x, y, mipsIndex, &modelsSentToGpu);
			if (model == nullptr) {
				// None available to render at the moment
				continue;
			}

			// Because lower resolution models can be loaded if the requested isn't available,
			//  we need to recompute the scale here
			float mipsScale = (float)minMipsLevel / (float)(model->GetMipsLevel());
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(
				scaleX * mipsScale, scaleY * mipsScale, scaleZ
			));

			glm::mat4 offset = glm::translate(glm::mat4(1.0f), glm::vec3(x * offsetXMultipler, y * offsetYMultipler, 1.0f));
			renderer->Render(camera, offset * scale, model);
		}
	}

	renderer->StopRender();
}

Terrain::~Terrain() {
	stopSync = true;
	if (terrainLoader != nullptr) {
		terrainLoader->join();
		delete terrainLoader;
	}

	delete renderer;

	// TODO delete all loaded models
}