#include <limits>
#include <iostream>
#include <glm\gtc\matrix_transform.hpp>
#include "../Telemetry/Logger.h"
#include "InfiniGrid.h"

InfiniGrid::InfiniGrid(): instanceCount(10000) { // TODO configurable instance count + fog info. Consider putting in debug area
	
}

bool InfiniGrid::Init(ShaderFactory* shaderFactory) {
	if (!shaderFactory->CreateShaderProgramWithGeometryShader("Config/Shaders/grid", &gridProgram))
	{
		Logger::LogError("Failed to load the grid rendering shader; cannot continue.");
		return false;
	}

    glGenVertexArrays(1, &partialCubeVao);
    glBindVertexArray(partialCubeVao);

    glGenBuffers(1, &partialCubeVbo);
    glBindBuffer(GL_ARRAY_BUFFER, partialCubeVbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);
    glEnableVertexAttribArray(0);

    // Send the data over to OpenGL / GPU
    // TODO glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}


void InfiniGrid::Render(Camera* camera) {
    glUseProgram(gridProgram);
	// TODO glDrawArraysInstanced(GL_TRIANGLES, 0, vertices.size(), instanceCount)

    glBindVertexArray(0);
    glUseProgram(0);
}

InfiniGrid::~InfiniGrid() {
	// TODO
}