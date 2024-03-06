#include <limits>
#include <iostream>
#include <glm\gtc\matrix_transform.hpp>
#include "../Telemetry/Logger.h"
#include "../Data/Shapes/Cube.h"
#include "../Data/Shapes/Plane.h"
#include "Clouds.h"
#include "../Time.h"

Clouds::Clouds(): uniforms() {
	
}

bool Clouds::Init(ShaderFactory* shaderFactory) {
    // TODO actually make this an interesting shader for more realistic clouds
	if (!shaderFactory->CreateShaderProgram("Config/Shaders/clouds", &cloudProgram))
	{
		Logger::LogError("Failed to load the grid rendering shader; cannot continue.");
		return false;
	}

    uniforms.SetProgramId(cloudProgram);

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

void Clouds::Render(Camera* camera) {
    glUseProgram(cloudProgram);
    glBindVertexArray(partialCubeVao);

    uniforms.SetMat4("view", camera->View);
    uniforms.SetMat4("perspective", camera->Perspective);

    uniforms.SetMaterialLighting(
        glm::vec3(0.7f), // ambient
        glm::vec3(0.4f), // diffuse
        glm::vec3(0.1f), // specular
        2.0f); // specular strength

    auto lightDirection = Time::GlobalTime->SunDirection();
    auto lightIntensity = Time::GlobalTime->SunIntensity() * 0.5f;

    auto viewLightDirection = camera->View * glm::vec4(lightDirection, 1.0f);
   uniforms.SetDirectionalLighting(
       glm::normalize(glm::vec3(viewLightDirection)),
       glm::vec3(0.9f), // ambient
       glm::vec3(0.8f) // diffuse
       // Clouds are white glm::vec3(lightIntensity / 2),
       // glm::vec3(lightIntensity)
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
    int width = 5;
    int instanceCount = width * width * width;
    uniforms.SetInt("width", width);
    uniforms.SetFloat("tileSize", tileSize * 2);
    uniforms.SetFloat("time", Time::GlobalTime->GameTime());

    uniforms.SetVec3("cloudPos", glm::vec3(0, 0, 0));

    glDrawArraysInstanced(GL_TRIANGLES, 0, planeInstance.size(), instanceCount);

    uniforms.SetVec3("cloudPos", glm::vec3(1000, (int)(Time::GlobalTime->GameTime() * 2000) % 20000, 0));
    glDrawArraysInstanced(GL_TRIANGLES, 0, planeInstance.size(), instanceCount);

    glBindVertexArray(0);
    glUseProgram(0);
}

Clouds::~Clouds() {
	// TODO
}