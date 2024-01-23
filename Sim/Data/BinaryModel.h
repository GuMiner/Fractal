#pragma once
#include <string>
#include <vector>
#include <glm\vec3.hpp>

class BinaryModel
{
	static void WriteVector(std::ofstream& outputStream, std::vector<glm::vec3>& vector);
	static void WriteVectorI(std::ofstream& outputStream, std::vector<glm::ivec3>& vector);
	static void ReadVector(std::ifstream& inputStream, std::vector<glm::vec3>& vector);
	static void ReadVectorI(std::ifstream& inputStream, std::vector<glm::ivec3>& vector);
public:
	static bool Save(
		std::string fileName,
		std::vector<glm::vec3>& vertices, std::vector<glm::ivec3>& faces);
	static bool Load(
		std::string fileName,
		std::vector<glm::vec3>& vertices, std::vector<glm::ivec3>& faces);
};

