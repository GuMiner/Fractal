#include <limits>
#include <iostream>
#include <glm\gtc\matrix_transform.hpp>
#include "../Telemetry/Logger.h"
#include "../Data/Shapes/Cube.h"
#include "../Data/Shapes/Plane.h"
#include "InfiniGrid.h"
#include "../Time.h"

InfiniGrid::InfiniGrid(): uniforms() { // TODO configurable instance count + fog info. Consider putting in debug area
	
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
    tileSize = 40.0f;
    Cube::Create(planeInstance, tileSize);
    glBufferData(GL_ARRAY_BUFFER, planeInstance.size() * sizeof(glm::vec3), &planeInstance[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &partialCubeNormalsVbo);
    glBindBuffer(GL_ARRAY_BUFFER, partialCubeNormalsVbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, nullptr);
    glEnableVertexAttribArray(1);

    Cube::CreateNormals(planeInstanceNormals);
    glBufferData(GL_ARRAY_BUFFER, planeInstanceNormals.size() * sizeof(glm::vec3), &planeInstanceNormals[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

	return true;
}

void InfiniGrid::Render(Camera* camera) {
    glUseProgram(gridProgram);
    glBindVertexArray(partialCubeVao);

    uniforms.SetMat4("view", camera->View);
    uniforms.SetMat4("perspective", camera->Perspective);

    uniforms.SetMaterialLighting(
        glm::vec3(0.2f, 0.2f, 0.2f), // ambient
        glm::vec3(0.3f, 0.6f, 0.3f), // diffuse
        glm::vec3(0.5f, 0.5f, 0.8f), // specular
        2.0f); // specular strength

    auto lightDirection = Time::GlobalTime->SunDirection();
    auto lightIntensity = Time::GlobalTime->SunIntensity() * 0.5f;

    auto viewLightDirection = camera->View * glm::vec4(lightDirection, 1.0f);
   uniforms.SetDirectionalLighting(
       glm::normalize(glm::vec3(viewLightDirection)),
       glm::vec3(lightIntensity / 2),
       glm::vec3(lightIntensity)
   );

   // TODO move to a lighting area somewhere.
   auto pointOnePosition = camera->Position() + (10.0f * camera->Forwards());
   auto pointOneViewPos = camera->View * glm::vec4(pointOnePosition, 1.0f);
   uniforms.SetPointOneLighting(
       glm::vec3(pointOneViewPos),
       glm::vec3(0.2f, 0.2f, 0.2f),
       glm::vec3(0.9f, 0.8f, 0.4f)
   );
    
    // TODO configurable
    int width = 1000;
    int instanceCount = width * width;
    uniforms.SetInt("width", width);
    uniforms.SetFloat("tileSize", tileSize * 2);
    uniforms.SetFloat("time", Time::GlobalTime->GameTime());

    glDrawArraysInstanced(GL_TRIANGLES, 0, planeInstance.size(), instanceCount);

    glBindVertexArray(0);
    glUseProgram(0);
}

InfiniGrid::~InfiniGrid() {
	// TODO
}