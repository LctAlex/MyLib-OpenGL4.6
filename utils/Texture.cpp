#include "Texture.hpp"

Texture2D::Texture2D(unsigned int width, unsigned int height):width(width),height(height)
{
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind(int activeTex)
{
    glActiveTexture(GL_TEXTURE0 + activeTex);
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture2D::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int Texture2D::GetHandler()
{
    return ID;
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &ID);
}