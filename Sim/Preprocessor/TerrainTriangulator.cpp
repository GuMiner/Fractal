#include <direct.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ostream>
#include <stb_image.h>
#include <glm\vec3.hpp>
#include "../Data/Config/PreprocessorConfig.h"
#include "TerrainTriangulator.h"

void TerrainTriangulator::TriangulateTerrain() {
    std::ifstream f("Config/preprocessor.json");
    PreprocessorConfig config = json::parse(f).template get<PreprocessorConfig>();

    for (int x = 0; x < config.width; x++) {
        for (int y = 0; y < config.height; y++) {
            std::stringstream inputFile;
            inputFile << "Config/Terrain/Source/" << y << "/" << x << ".png";
            if (!FileExists(inputFile.str())) {
                std::stringstream outputFilePath;
                SetOutputFilePath(&outputFilePath, x, y);

                std::ofstream outputFile(outputFilePath.str(), std::ios::out);
                TriangulateTile(inputFile.str().c_str(), outputFile);
                outputFile.close();
            }
        }
    }
}

void TerrainTriangulator::TriangulateTile(const char* inputFile, std::ofstream& outputFile) {
    // Save in the OFF file format
    outputFile << "OFF" << std::endl;

    int width = 0;
    int height = 0;
    int channels = 0;
    unsigned char* imageData = stbi_load(inputFile, &width, &height, &channels, STBI_rgb_alpha);

    std::vector<glm::vec3> vertices;
    std::vector<glm::ivec3> faces;

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            int imageCoord = (x + y * width) * 4;
            int height = imageData[imageCoord] + (int)imageData[imageCoord] << 8; // TODO might need modifications

            // V1 -- just flat plane, connectors TBD
            int vs = vertices.size();
            faces.push_back(glm::ivec3(vs, vs + 1, vs + 2)); // CW faces of the top plane
            faces.push_back(glm::ivec3(vs, vs + 2, vs + 3));
            vertices.push_back(glm::vec3(x, y, height));
            vertices.push_back(glm::vec3(x, y + 1, height));
            vertices.push_back(glm::vec3(x + 1, y + 1, height));
            vertices.push_back(glm::vec3(x + 1, y, height));
        }
    }

    outputFile << vertices.size() << " " << faces.size() << " " << 0 << std::endl;
    for (glm::vec3 vertex : vertices) {
        outputFile << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
    }

    for (glm::ivec3 face : faces) {
        outputFile << face.x << " " << face.y << " " << face.z << std::endl;
    }
}

void TerrainTriangulator::SetOutputFilePath(std::stringstream* outputFilePath, int x, int y) {
    *outputFilePath << "Config/Terrain/Generated/" << y;
    EnsureFolderExists(outputFilePath->str());

    *outputFilePath << "/" << x;
    EnsureFolderExists(outputFilePath->str());

    *outputFilePath << ".png";
}

void TerrainTriangulator::EnsureFolderExists(const std::string& folder) {
    if (_access(folder.c_str(), 0) != 0)
    {
        _mkdir(folder.c_str());
    }
}

bool TerrainTriangulator::FileExists(const std::string& fileName) {
    return _access(fileName.c_str(), 0) == 0;
}