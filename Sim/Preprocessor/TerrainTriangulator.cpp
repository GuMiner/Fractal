#include <direct.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ostream>
#include <stb_image.h>
#include <stb_image_resize.h>
#include <stb_image_write.h>
#include <glm\vec3.hpp>
#include "../Data/Config/PreprocessorConfig.h"
#include "../Data/BinaryModel.h"
#include "TerrainTriangulator.h"

void TerrainTriangulator::GenerateMipMaps() {
    std::ifstream f("Config/preprocessor.json");
    PreprocessorConfig config = json::parse(f).template get<PreprocessorConfig>();

    for (int x = 0; x < config.width; x++) {
        std::cout << "MipMaps: Processing " << x << std::endl;
        for (int y = 0; y < config.height; y++) {
            std::stringstream inputFile;
            inputFile << "Config/Terrain/Source/" << y << "/" << x << ".png";

            std::stringstream outputFilePath;
            SetMipMapOutputFilePath(&outputFilePath, x, y);
            if (!FileExists(outputFilePath.str())) {
                std::cout << "Reducing " << x << ", " << y << std::endl;

                ReduceTile(inputFile.str().c_str(), outputFilePath.str().c_str());
            }
        }
    }
}

void TerrainTriangulator::ReduceTile(const char* inputFile, const char* outputFile)
{
    int width = 0;
    int height = 0;
    int channels = 0;
    unsigned char* imageData = stbi_load(inputFile, &width, &height, &channels, STBI_rgb_alpha);

    // No matter the input (should always be ~1000x1000), go to:
    // 512x512, then 256x256, then 128x128, then 64x64, then 32x32, then 16x16, then 8x8, then 4,4
    // Oriented top to bottom

    // This is somewhat silly, but easy to visually verify for a one-time operation.
    int mipmapWidth = 512;
    int mipmapHeight = 512 + 256 + 128 + 64 + 32 + 16 + 8 + 4;
    unsigned char* mipmapData = new unsigned char[mipmapWidth * mipmapHeight * 4];
    stbir_resize_uint8(
        imageData, width, height, 0,
        mipmapData, 512, 512, 0, 4);

    int fullStride = 512 * 4;
    int offset256 = 512 * fullStride;
    stbir_resize_uint8(
        mipmapData, 512, 512, 0,
        mipmapData + offset256, 256, 256, fullStride, 4);

    int offset128 = offset256 + 256 * fullStride;
    stbir_resize_uint8(
        mipmapData + offset256, 256, 256, fullStride,
        mipmapData + offset128, 128, 128, fullStride, 4);

    int offset64 = offset128 + 128 * fullStride;
    stbir_resize_uint8(
        mipmapData + offset128, 128, 128, fullStride,
        mipmapData + offset64, 64, 64, fullStride, 4);

    int offset32 = offset64 + 64 * fullStride;
    stbir_resize_uint8(
        mipmapData + offset64, 64, 64, fullStride,
        mipmapData + offset32, 32, 32, fullStride, 4);

    int offset16 = offset32 + 32 * fullStride;
    stbir_resize_uint8(
        mipmapData + offset32, 32, 32, fullStride,
        mipmapData + offset16, 16, 16, fullStride, 4);

    int offset8 = offset16 + 16 * fullStride;
    stbir_resize_uint8(
        mipmapData + offset16, 16, 16, fullStride,
        mipmapData + offset8, 8, 8, fullStride, 4);

    int offset4 = offset8 + 8 * fullStride;
    stbir_resize_uint8(
        mipmapData + offset8, 8, 8, fullStride,
        mipmapData + offset4, 4, 4, fullStride, 4);

    stbi_write_png(outputFile, mipmapWidth, mipmapHeight, 4, mipmapData, 0);
}


void TerrainTriangulator::TriangulateTerrain() {
    std::ifstream f("Config/preprocessor.json");
    PreprocessorConfig config = json::parse(f).template get<PreprocessorConfig>();

    // TODO swap back once testing is done
    for (int x = 0; x < config.width; x++) {
        std::cout << "Triangulation: Processing " << x << std::endl;
        for (int y = 0; y < config.height; y++) {
            std::stringstream inputFile;
            inputFile << "Config/Terrain/Generated/" << y << "/" << x << "-mips.png";

            std::stringstream outputFilePath;
            SetOutputFilePath(&outputFilePath, x, y, 8); // TODO for now, just make the 8x8 mips files
            if (true || !FileExists(outputFilePath.str())) {

                std::cout << "Triangulating " << x << ", " << y << std::endl;
                TriangulateTile(inputFile.str().c_str(), outputFilePath.str(), 8);
                //return; // Only do the starter tile to test the generation  
            }
        }
    }
}

void TerrainTriangulator::TriangulateTile(const char* inputFile, std::string outputFile, int mipsLevel) {
    int width = 0;
    int height = 0;
    int channels = 0;
    unsigned char* imageData = stbi_load(inputFile, &width, &height, &channels, STBI_rgb_alpha);

    if (width != 512) {
        std::cout << "This should be a mip-mapped tile, erroring out" << std::endl;
        return;
    }

    std::vector<glm::vec3> vertices;
    std::vector<glm::ivec3> faces;

    for (int x = 0; x < mipsLevel; x++) {
        for (int y = 0; y < mipsLevel; y++) {
            int xEffective = x; // TODO don't assume mips level 8 here
            int mipsOffset = 512 + 256 + 128 + 64 + 32 + 16;
            int yEffective = y + mipsOffset; // TODO have a mips class to do this sort of calculation

            int imageCoord = (xEffective + yEffective * width) * 4;
            int height = imageData[imageCoord] + ((int)imageData[imageCoord + 1] << 8); // TODO might need modifications
            
            // TODO needs to be able to load other images and get edges from that. 
            // This probably could be done by re-mip-mapping the images, or done here
            int xPlus1 = x == mipsLevel - 1 ? x : x + 1;
            int yPlus1 = (y == mipsLevel - 1 ? y : y + 1) + mipsOffset;

            int imageCoordX = (xPlus1 + yEffective * width) * 4;
            int imageCoordY = (xEffective + yPlus1 * width) * 4;
            int imageCoordXY = (xPlus1 + yPlus1 * width) * 4;
            int heightX = imageData[imageCoordX] + ((int)imageData[imageCoordX + 1] << 8);
            int heightY = imageData[imageCoordY] + ((int)imageData[imageCoordY + 1] << 8);
            int heightXY = imageData[imageCoordXY] + ((int)imageData[imageCoordXY + 1] << 8);

            // V1 -- just flat plane, connectors TBD
            int vs = vertices.size();
            faces.push_back(glm::ivec3(vs, vs + 1, vs + 2)); // CW faces of the top plane
            faces.push_back(glm::ivec3(vs, vs + 2, vs + 3));
            vertices.push_back(glm::vec3(x, y, height));
            vertices.push_back(glm::vec3(x, y + 1, heightY));
            vertices.push_back(glm::vec3(x + 1, y + 1, heightXY));
            vertices.push_back(glm::vec3(x + 1, y, heightX));
        }
    }

    if (!BinaryModel::Save(outputFile, vertices, faces)) {
        std::cout << "Unable to save " << outputFile << "!" << std::endl;
    }

    stbi_image_free(imageData);
}

void TerrainTriangulator::SetOutputFilePath(std::stringstream* outputFilePath, int x, int y, int mipsLevel) {
    *outputFilePath << "Config/Terrain/Generated/" << y;
    EnsureFolderExists(outputFilePath->str());

    *outputFilePath << "/" << x << "-" << mipsLevel << ".off";
}

void TerrainTriangulator::SetMipMapOutputFilePath(std::stringstream* outputFilePath, int x, int y) {
    *outputFilePath << "Config/Terrain/Generated/" << y;
    EnsureFolderExists(outputFilePath->str());

    *outputFilePath << "/" << x << "-mips.png";
}

void TerrainTriangulator::EnsureFolderExists(const std::string& folder) {
    if (_access(folder.c_str(), 0) != 0)
    {
        _mkdir(folder.c_str());
    }
}

bool TerrainTriangulator::FileExists(const std::string& fileName) {
    std::ifstream infile(fileName);
    return infile.good();
}