#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "../GLCore/ShaderFactory.h"
#include "../Camera.h"

class Compass {
	GLuint compassProgram;

	GLuint tripleAxisVao;
	GLuint positionVbo;
	GLuint colorVbo;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec4> colors;

public:
	Compass();
	bool Init(ShaderFactory* shaderFactory);
	bool SendMesh();

	void Render(Camera* camera); 
};
