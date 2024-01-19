#pragma once
#include <string>
#include <sstream>
#include <ostream>

class TerrainTriangulator
{
	bool FileExists(const std::string& fileName);
	void EnsureFolderExists(const std::string& folder);
	void SetOutputFilePath(std::stringstream* outputFilePath, int x, int y);
	void TriangulateTile(const char* inputFile, std::ofstream& outputFile);
public:
	void TriangulateTerrain();
};
