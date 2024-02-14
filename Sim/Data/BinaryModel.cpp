#include <fstream>
#include <iterator>
#include <iostream>
#include <ostream>
#include <vector>
#include <lz4frame.h>
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

bool BinaryModel::SaveCompressed(
	std::string fileName,
	std::vector<glm::vec3>& vertices, std::vector<glm::ivec3>& faces) {
	// Copy over all vertices and faces into a separate binary buffer
	unsigned int verticesLength = vertices.size();
	unsigned int facesLength = faces.size();
	unsigned int length = sizeof(unsigned int) + 
		sizeof(glm::vec3) * verticesLength +
		sizeof(unsigned int) +
		sizeof(glm::ivec3) * facesLength;

	char* uncompressedData = new char[length];

	int index = 0;
	std::memcpy(&uncompressedData[index], &verticesLength, sizeof(unsigned int));
	index += sizeof(unsigned int);

	std::memcpy(&uncompressedData[index], &vertices[0], sizeof(glm::vec3) * verticesLength);
	index += sizeof(glm::vec3) * verticesLength;

	std::memcpy(&uncompressedData[index], &facesLength, sizeof(unsigned int));
	index += sizeof(unsigned int);

	std::memcpy(&uncompressedData[index], &faces[0], sizeof(glm::ivec3) * facesLength);

	// Compress that buffer
	// TODO consider setting compression preferences.

	int outputLength = LZ4F_compressFrameBound(length, nullptr);
	char* compressedData = new char[outputLength];

	size_t compressedLength = LZ4F_compressFrame(&compressedData[0], outputLength, &uncompressedData[0], length, nullptr);
	if (LZ4F_isError(compressedLength)) {
		std::cout << "Compression failure: " << LZ4F_getErrorName(compressedLength) << std::endl;
		return false;
	}

	// Save the compressed data
	std::ofstream outputFile(fileName, std::ios::out | std::ios::binary);
	outputFile.write((char*)&length, sizeof(unsigned int));
	outputFile.write(compressedData, compressedLength);
	outputFile.close();

	delete[] compressedData;
	delete[] uncompressedData;
	return true;
}

bool BinaryModel::LoadCompressed(
	std::string fileName,
	std::vector<glm::vec3>& vertices, std::vector<glm::ivec3>& faces) {
	// Read entire file into memory
	std::ifstream infile(fileName, std::ios::in | std::ios::binary);
	if (!infile.good()) {
		std::cout << "File " << fileName << " does not exist!" << std::endl;
		return false;
	}

	std::vector<char> memoryFile(
		(std::istreambuf_iterator<char>(infile)),
		std::istreambuf_iterator<char>());
	size_t remainingBytes = memoryFile.size();

	unsigned int uncompressedLength;
	std::memcpy(&uncompressedLength, &memoryFile[0], sizeof(unsigned int));
	remainingBytes -= sizeof(unsigned int);

	// Decompress
	LZ4F_dctx* context;
	LZ4F_errorCode_t result = LZ4F_createDecompressionContext(&context, LZ4F_VERSION);
	if (LZ4F_isError(result)) {
		std::cout << "Decompression failure: " << LZ4F_getErrorName(result) << std::endl;
		return false;
	}

	size_t processedBytes = uncompressedLength;

	char* decompressedData = new char[uncompressedLength];
	result = LZ4F_decompress(context, &decompressedData[0], &processedBytes, &memoryFile[sizeof(unsigned int)], &remainingBytes, nullptr);
	if (LZ4F_isError(result)) {
		std::cout << "Decompression failure: " << LZ4F_getErrorName(result) << std::endl;
		return false;
	}

	if (result != 0) {
		std::cout << "Decompression took multiple steps: " << result << ". Unexpected, as there should be only one frame." << std::endl;
		return false;
	}

	result = LZ4F_freeDecompressionContext(context);
	if (LZ4F_isError(result)) {
		std::cout << "Decompression failure: " << LZ4F_getErrorName(result) << std::endl;
		return false;
	}

	// Load vertices and faces from raw data.
	unsigned int index = 0;
	unsigned int verticesLength;
	std::memcpy(&verticesLength, &decompressedData[index], sizeof(unsigned int));
	index += sizeof(unsigned int);

	vertices.resize(verticesLength);
	std::memcpy(&vertices[0], &decompressedData[index], sizeof(glm::vec3) * verticesLength);
	index += sizeof(glm::vec3) * verticesLength;

	unsigned int facesLength;
	std::memcpy(&facesLength, &decompressedData[index], sizeof(unsigned int));
	index += sizeof(unsigned int);

	faces.resize(facesLength);
	std::memcpy(&faces[0], &decompressedData[index], sizeof(glm::ivec3) * facesLength);

	delete[] decompressedData;
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