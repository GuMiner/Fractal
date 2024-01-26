#pragma once
#include <vector>
#include <glm\vec3.hpp>

class VectorMath {
public:
	static void ComputeNormals(
		std::vector<glm::vec3>& vertices, std::vector<glm::ivec3>& faces,
		std::vector<glm::vec3>& normals);
};

