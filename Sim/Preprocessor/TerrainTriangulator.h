#pragma once
#include <string>
#include <sstream>
#include <ostream>
#include "../Data/Config/TerrainConfig.h"

class TerrainTriangulator
{
	TerrainConfig config;

	bool FileExists(const std::string& fileName);
	void EnsureFolderExists(const std::string& folder);
	
	void SetMipMapOutputFilePath(std::stringstream* outputFilePath, int x, int y);
	void ReduceTile(const char* inputFile, const char* outputFile);

	unsigned char* LoadMipsImage(int x, int y);
	void CopyEdges(int x, int y);

	void SetOutputFilePath(std::stringstream* outputFilePath, int x, int y, int mipsLevel);
	void TriangulateTile(int x, int y, std::string outputFile, int mipsLevel);
public:
	TerrainTriangulator();
	void GenerateMipMaps();
	void TriangulateTerrain();
};
