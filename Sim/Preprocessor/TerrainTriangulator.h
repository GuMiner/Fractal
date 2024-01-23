#pragma once
#include <string>
#include <sstream>
#include <ostream>

class TerrainTriangulator
{
	bool FileExists(const std::string& fileName);
	void EnsureFolderExists(const std::string& folder);
	
	void ReduceTile(const char* inputFile, const char* outputFile);
	void TriangulateTile(const char* inputFile, std::string outputFile, int mipsLevel);

	void SetMipMapOutputFilePath(std::stringstream* outputFilePath, int x, int y);
	void SetOutputFilePath(std::stringstream* outputFilePath, int x, int y, int mipsLevel);
public:
	void GenerateMipMaps();
	void TriangulateTerrain();
};
