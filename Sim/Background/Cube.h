#pragma once
#include <vector>
#include <glm\vec3.hpp>

class Cube {
public:
	static void Create(std::vector<glm::vec3>& vertices, float scale);
};