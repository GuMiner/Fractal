#include <limits>
#include <iostream>
#include <glm\gtc\matrix_transform.hpp>
#include "../Telemetry/Logger.h"
#include "../Data/Shapes/Plane.h"
#include "InfiniGrid.h"

InfiniGrid::InfiniGrid(): instanceCount(10000), uniforms() { // TODO configurable instance count + fog info. Consider putting in debug area
	
}

bool InfiniGrid::Init(ShaderFactory* shaderFactory) {
	if (!shaderFactory->CreateShaderProgram("Config/Shaders/grid", &gridProgram))
	{
		Logger::LogError("Failed to load the grid rendering shader; cannot continue.");
		return false;
	}

    uniforms.SetProgramId(gridProgram);

    glGenVertexArrays(1, &partialCubeVao);
    glBindVertexArray(partialCubeVao);

    glGenBuffers(1, &partialCubeVbo);
    glBindBuffer(GL_ARRAY_BUFFER, partialCubeVbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);
    glEnableVertexAttribArray(0);

    // Send the data over to OpenGL / GPU
    Plane::Create(planeInstance, 1.0f);
    glBufferData(GL_ARRAY_BUFFER, planeInstance.size() * sizeof(glm::vec3), &planeInstance[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	return true;
}


void InfiniGrid::Render(Camera* camera) {
    glUseProgram(gridProgram);
    glBindVertexArray(partialCubeVao);

    uniforms.SendMat4("view", camera->View);
    uniforms.SendMat4("perspective", camera->Perspective);

    glDrawArraysInstanced(GL_TRIANGLES, 0, planeInstance.size(), instanceCount);

    glBindVertexArray(0);
    glUseProgram(0);
}

InfiniGrid::~InfiniGrid() {
	// TODO
}