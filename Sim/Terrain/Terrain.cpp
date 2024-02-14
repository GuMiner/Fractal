#include <iostream>
#include <glm\gtc\matrix_transform.hpp>
#include "Terrain.h"

Terrain::Terrain() {
	renderer = new TerrainRenderer();
}

bool Terrain::Init(ShaderFactory* shaderFactory) {
	if (!renderer->Init(shaderFactory)) {
		return false;
	}

	// TODO add grid of models 
	for (int x = 0; x < 70; x++) {
		std::cout << x << std::endl;
		for (int y = 0; y < 70; y++) {
			auto model = new TerrainModel();
			if (!model->Load(x, y)) {
				return false;
			}

			if (!model->SendMesh()) {
				return false;
			}

			models[x + y * 70] = model;
		}
	}

	return true;
}


void Terrain::Render(Camera* camera) {
	int mipsLevel = 16; // TODO distance based mips 
	float scale = 8.0f / (float)mipsLevel;
	renderer->StartRender(camera);
	TerrainModel* anchorModel = models[0];
	for (int x = 0; x < 70; x++) {
		for (int y = 0; y < 70; y++) {
			auto position =
				glm::translate(
					glm::scale(glm::mat4(1.0), glm::vec3(0.1f, 0.1f, anchorModel->scaleFactor / 25.0f)), // 0.01f)), //glm::rotate(glm::mat4(1.0), currentTime * 0.5f, glm::vec3(0, 1, 0)),
					glm::vec3(0, 0, anchorModel->offsetFactor)); //-109544));
			
			// TODO need to determine an anchor mipmap and scale everything to that instead of offseting by 8
			position = glm::translate(position, glm::vec3(x * 16, y * 16, 1.0f));
			renderer->Render(camera, position, models[x + y * 70]);
		}
	}

	renderer->StopRender();
}