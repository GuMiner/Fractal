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
