#include "Plane.h"

#define AddVertex(x, y, z) vertices.push_back(glm::vec3(x, y, z)) 

void Plane::Create(std::vector<glm::vec3>& vertices, float scale) {
	// Top
	AddVertex(-scale, -scale, 0);
	AddVertex(scale, -scale, 0);
	AddVertex(-scale, scale, 0);
	AddVertex(scale, -scale, 0);
	AddVertex(scale, scale, 0);
	AddVertex(-scale, scale, 0);
}

void Plane::CreateNormals(std::vector<glm::vec3>& vertices) {
	AddVertex(0.0f, 0.0f, 1.0f);
	AddVertex(0.0f, 0.0f, 1.0f);
	AddVertex(0.0f, 0.0f, 1.0f);
	AddVertex(0.0f, 0.0f, 1.0f);
	AddVertex(0.0f, 0.0f, 1.0f);
	AddVertex(0.0f, 0.0f, 1.0f);
}
