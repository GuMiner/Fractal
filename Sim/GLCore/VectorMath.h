#pragma once
#include <vector>
#include <glm\vec3.hpp>

class VectorMath {
public:
	static bool ConvertOffToBinaryOff(std::string inputFile, std::string outputFile);
	static void ComputeVertexNormals(
		std::vector<glm::vec3>& vertices, std::vector<glm::ivec3>& faces,
		std::vector<glm::vec3>& normals);
	static void ComputeFaceNormals(
		std::vector<glm::vec3>& vertices, std::vector<glm::ivec3>& faces,
		std::vector<glm::vec3>& normals);
};

