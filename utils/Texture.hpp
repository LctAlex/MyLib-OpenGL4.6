#pragma once
#include "../include/glad/glad.h"

class Texture2D
{
    private:
    unsigned int ID;
    
    public:
    unsigned int width, height;
    Texture2D(unsigned int width, unsigned int height);
    void Bind(int activeTex = 0);
    void Unbind();
    unsigned int GetHandler();

    ~Texture2D();
};