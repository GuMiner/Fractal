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

    uniforms.SendMat4("view", camera->View);
    uniforms.SendMat4("perspective", camera->Perspective);


    GLint ambient = glGetUniformLocation(gridProgram, "ambient");
    glUniform3f(ambient, 0.501f, 0.801f, 0.501f);

    GLint diffuse = glGetUniformLocation(gridProgram, "diffuse");
    glUniform3f(diffuse, 0.51f, 0.81f, 0.51f);

    GLint specularColor = glGetUniformLocation(gridProgram, "specularColor");
    glUniform3f(specularColor, 0.51f, 0.51f, 0.81f);

    GLint specular = glGetUniformLocation(gridProgram, "specular");
    glUniform1f(specular, 4.0f);

    auto lightDirection = Time::GlobalTime->SunDirection();
    auto lightIntensity = Time::GlobalTime->SunIntensity() * 2.0f;

    GLint dLightDirection = glGetUniformLocation(gridProgram, "dLightDirection");
    glUniform3f(dLightDirection, lightDirection.x, lightDirection.y, lightDirection.z);

    GLint dLightAmbient = glGetUniformLocation(gridProgram, "dLightAmbient");
    glUniform3f(dLightAmbient, lightIntensity / 2, lightIntensity / 2, lightIntensity / 2);

    GLint dLightDiffuse = glGetUniformLocation(gridProgram, "dLightDiffuse");
    glUniform3f(dLightDiffuse, lightIntensity, lightIntensity, lightIntensity);
    
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