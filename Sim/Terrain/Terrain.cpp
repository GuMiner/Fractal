#include <iostream>
#include <fstream>
#include <glm\gtc\matrix_transform.hpp>
#include "Terrain.h"

Terrain::Terrain() {
	renderer = new TerrainRenderer();

	std::ifstream f("Config/terrain.json");
	config = json::parse(f).template get<TerrainConfig>();
}

int Terrain::GetTileIndex(int x, int y, int mipsLevel) {
	int baseIndex = x + y * config.width;
	int multipliedIndex = baseIndex * config.mipsLevels.size();
	return multipliedIndex + config.MipsLevelIndex(mipsLevel);
}

bool Terrain::Init(ShaderFactory* shaderFactory) {
	if (!renderer->Init(shaderFactory)) {
		return false;
	}

	int minMipsLevel = config.MinMipsLevel();
	for (int x = 0; x < config.width; x++) {
		std::cout << "Loaded basic terrain column " << x << std::endl;
		for (int y = 0; y < config.height; y++) {
			auto model = new TerrainModel();
			if (!model->Load(x, y, minMipsLevel)) {
				return false;
			}

			if (!model->SendMesh()) {
				return false;
			}

			models[GetTileIndex(x, y, minMipsLevel)] = model;
		}
	}

	return true;
}


void Terrain::Render(Camera* camera) {
	int minMipsLevel = config.MinMipsLevel();

	int mipsLevel = minMipsLevel;
	float scale = (float)minMipsLevel / (float)mipsLevel;
	renderer->StartRender(camera);

	TerrainModel* anchorModel = models[GetTileIndex(0, 0, mipsLevel)];
	for (int x = 0; x < config.width; x++) {
		for (int y = 0; y < config.height; y++) {
			auto position =
				glm::translate(
					glm::scale(glm::mat4(1.0), glm::vec3(scale, scale, anchorModel->scaleFactor / 25.0f)),
					glm::vec3(0, 0, anchorModel->offsetFactor)); //-109544));
			
			// TODO need to determine an anchor mipmap and scale everything to that instead of offseting by 8
			position = glm::translate(position, glm::vec3(x * mipsLevel, y * mipsLevel, 1.0f));
			renderer->Render(camera, position, models[GetTileIndex(x, y, mipsLevel)]);
		}
	}

	renderer->StopRender();
}