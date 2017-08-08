#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

class Texture
{
public:
    int activeTextureOffset;
    GLuint textureId;

    std::vector<unsigned char> textureData;
    int width;
    int height;

    Texture();
    void Initialize();
    void TransferToOpenGl();
    ~Texture();
};

