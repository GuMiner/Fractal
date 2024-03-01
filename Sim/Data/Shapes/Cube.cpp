#include "Cube.h"

#define AddVertex(x, y, z) vertices.push_back(glm::vec3(x, y, z)) 

void Cube::Create(std::vector<glm::vec3>& vertices, float scale) {
	// Bottom
	AddVertex(-scale, -scale, -scale);
	AddVertex(scale, -scale, -scale);
	AddVertex(-scale, -scale, scale);
	AddVertex(scale, -scale, -scale);
	AddVertex(scale, -scale, scale);
	AddVertex(-scale, -scale, scale);

	// Top
	AddVertex(-scale, scale, -scale);
	AddVertex(-scale, scale, scale);
	AddVertex(scale, scale, -scale);
	AddVertex(scale, scale, -scale);
	AddVertex(-scale, scale, scale);
	AddVertex(scale, scale, scale);

	// Front
	AddVertex(-scale, -scale, scale);
	AddVertex(scale, -scale, scale);
	AddVertex(-scale, scale, scale);
	AddVertex(scale, -scale, scale);
	AddVertex(scale, scale, scale);
	AddVertex(-scale, scale, scale);

	// Back
	AddVertex(-scale, -scale, -scale);
	AddVertex(-scale, scale, -scale);
	AddVertex(scale, -scale, -scale);
	AddVertex(scale, -scale, -scale);
	AddVertex(-scale, scale, -scale);
	AddVertex(scale, scale, -scale);

	// Left
	AddVertex(-scale, -scale, scale);
	AddVertex(-scale, scale, -scale);
	AddVertex(-scale, -scale, -scale);
	AddVertex(-scale, -scale, scale);
	AddVertex(-scale, scale, scale);
	AddVertex(-scale, scale, -scale);

	// Right
	AddVertex(scale, -scale, scale);
	AddVertex(scale, -scale, -scale);
	AddVertex(scale, scale, -scale);
	AddVertex(scale, -scale, scale);
	AddVertex(scale, scale, -scale);
	AddVertex(scale, scale, scale);
}


void Cube::CreateNormals(std::vector<glm::vec3>& vertices) {
	// Bottom
	AddVertex(0.0f, -1.0f, 0.0f);
	AddVertex(0.0f, -1.0f, 0.0f);
	AddVertex(0.0f, -1.0f, 0.0f);
	AddVertex(0.0f, -1.0f, 0.0f);
	AddVertex(0.0f, -1.0f, 0.0f);
	AddVertex(0.0f, -1.0f, 0.0f);

	// Top
	AddVertex(0.0f, 1.0f, 0.0f);
	AddVertex(0.0f, 1.0f, 0.0f);
	AddVertex(0.0f, 1.0f, 0.0f);
	AddVertex(0.0f, 1.0f, 0.0f);
	AddVertex(0.0f, 1.0f, 0.0f);
	AddVertex(0.0f, 1.0f, 0.0f);

	// Front
	AddVertex(0.0f, 0.0f, 1.0f);
	AddVertex(0.0f, 0.0f, 1.0f);
	AddVertex(0.0f, 0.0f, 1.0f);
	AddVertex(0.0f, 0.0f, 1.0f);
	AddVertex(0.0f, 0.0f, 1.0f);
	AddVertex(0.0f, 0.0f, 1.0f);

	// Back
	AddVertex(0.0f, 0.0f, -1.0f);
	AddVertex(0.0f, 0.0f, -1.0f);
	AddVertex(0.0f, 0.0f, -1.0f);
	AddVertex(0.0f, 0.0f, -1.0f);
	AddVertex(0.0f, 0.0f, -1.0f);
	AddVertex(0.0f, 0.0f, -1.0f);

	// Left
	AddVertex(-1.0f, 0.0f, 0.0f);
	AddVertex(-1.0f, 0.0f, 0.0f);
	AddVertex(-1.0f, 0.0f, 0.0f);
	AddVertex(-1.0f, 0.0f, 0.0f);
	AddVertex(-1.0f, 0.0f, 0.0f);
	AddVertex(-1.0f, 0.0f, 0.0f);

	// Right
	AddVertex(1.0f, 0.0f, 0.0f);
	AddVertex(1.0f, 0.0f, 0.0f);
	AddVertex(1.0f, 0.0f, 0.0f);
	AddVertex(1.0f, 0.0f, 0.0f);
	AddVertex(1.0f, 0.0f, 0.0f);
	AddVertex(1.0f, 0.0f, 0.0f);
}
