#pragma once
#include "../include/glad/glad.h"

#include "../include/glm/glm.hpp"

#include <iostream>
#include <vector>

class Mesh
{
    private:
    unsigned int VBO, EBO, VAO;
    int elements;
    
    public:
    glm::mat4 model;
    //every parameter could be const
    Mesh(const float *vertices, const unsigned int *indices, int vertSize, int indSize, const int sizes[], int sizesLength, int stride); //(const int[]){...}, size
    Mesh(const float *vertices, const unsigned int *indices, int vertSize, int indSize, int size, int stride);
    Mesh(const std::vector<float> &vertices, const std::vector<unsigned int> &indices, const std::vector<int> sizes, int stride); //const std::vector<int>{...}
    Mesh(const std::vector<float> &vertices, const std::vector<unsigned int> &indices, int size, int stride);
    Mesh(const std::vector<float> &vertices, const std::vector<unsigned int> &indices); // positions, normals/colors, texCoords = 8 total
    void Draw();
    void DrawInstanced(int instances = 1);

    ~Mesh();

    //static functions: do not DEPEND on a class' instance!
    static Mesh createQuad();
    static Mesh createCube();
    static Mesh createGrassBlade();
    static Mesh createCanvas();

    static Mesh createQuadColor(glm::vec3 color);
    //fun
    static Mesh createGrassBladeColor(glm::vec3 baseColor, glm::vec3 tipColor = glm::vec3(1.f));
};