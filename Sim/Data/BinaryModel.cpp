#include <fstream>
#include <iostream>
#include <ostream>
#include "BinaryModel.h"

bool BinaryModel::Save(
	std::string fileName,
	std::vector<glm::vec3>& vertices, std::vector<glm::ivec3>& faces) {

	std::ofstream outputFile(fileName, std::ios::out | std::ios::binary);

	WriteVector(outputFile, vertices);
	WriteVectorI(outputFile, faces);

	outputFile.close();
	return true;
}

bool BinaryModel::Load(
	std::string fileName,
	std::vector<glm::vec3>& vertices, std::vector<glm::ivec3>& faces) {
	std::ifstream infile(fileName, std::ios::in | std::ios::binary);
	if (!infile.good()) {
		std::cout << "File " << fileName << " does not exist!" << std::endl;
		return false;
	}

	ReadVector(infile, vertices);
	ReadVectorI(infile, faces);

	return true;
}

void BinaryModel::WriteVector(std::ofstream& outputStream, std::vector<glm::vec3>& vector)
{
	unsigned int size = (unsigned int)vector.size();
	outputStream.write((char*)&size, sizeof(unsigned int));
	for (unsigned int i = 0; i < size; i++)
	{
		outputStream.write((char*)&(vector[i]), sizeof(glm::vec3));
	}
}

void BinaryModel::WriteVectorI(std::ofstream& outputStream, std::vector<glm::ivec3>& vector)
{
	unsigned int size = (unsigned int)vector.size();
	outputStream.write((char*)&size, sizeof(unsigned int));
	for (unsigned int i = 0; i < size; i++)
	{
		outputStream.write((char*)&(vector[i]), sizeof(glm::ivec3));
	}
}

void BinaryModel::ReadVector(std::ifstream& inputStream, std::vector<glm::vec3>& vector)
{
	unsigned int size;
	inputStream.read((char*)&size, sizeof(unsigned int));

	vector.resize(size);
	inputStream.read((char*)&vector[0], sizeof(glm::vec3) * size);
}

void BinaryModel::ReadVectorI(std::ifstream& inputStream, std::vector<glm::ivec3>& vector)
{
	unsigned int size;
	inputStream.read((char*)&size, sizeof(unsigned int));

	vector.resize(size);
	inputStream.read((char*)&vector[0], sizeof(glm::ivec3) * size);
}