#include "Texture.h"

Texture::Texture()
{
}

void Texture::Initialize()
{
    glGenTextures(1, &textureId);
}

void Texture::TransferToOpenGl()
{
    glActiveTexture(GL_TEXTURE0 + activeTextureOffset);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA, width, height);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &textureData[0]);
}

int Texture::GetWidth() const
{
    return width;
}

int Texture::GetHeight() const
{
    return height;
}

Texture::~Texture()
{
    glDeleteTextures(1, &textureId);
}
