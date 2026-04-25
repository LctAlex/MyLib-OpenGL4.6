#pragma once
#include "../include/glad/glad.h"

#include "../include/glm/glm.hpp"

#include <iostream>
#include <vector>

class Shape
{
private:
    unsigned int VBO, EBO, VAO;
    int elements;
    
    public:
    glm::mat4 model;
    Shape(const float *vertices, const unsigned int *indices, int vertSize, int indSize, int size, int stride);
    Shape(const std::vector<float> &vertices, const std::vector<unsigned int> &indices, int size, int stride);
    Shape(const std::vector<float> &vertices, const std::vector<unsigned int> &indices); // positions, normals/colors, texCoords = 8 total
    void Draw();
    ~Shape();
};