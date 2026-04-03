#pragma once
#include <iostream>
#include <vector>
#include "../include/glad/glad.h"

class Mesh
{
    private:
    unsigned int VBO, EBO, VAO;
    int elements;
    public:
    Mesh(const float* vertices, const unsigned int* indices, int vertSize, int indSize, int size, int stride);
    Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, int size, int stride);
    Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices); //positions, normals/colors, texCoords = 8 total
    void Draw();
    void Delete();
    ~Mesh();
};