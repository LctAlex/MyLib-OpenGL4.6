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

    ~Mesh();

    //static functions: do not DEPEND on a class' instance!
    static Mesh createQuad();
    static Mesh createCube();
    static Mesh createGrassBlade();
    static Mesh createCanvas();

    //fun
    static Mesh createQuadColor(glm::vec3 color);
    static Mesh createGrassBladeColor(glm::vec3 baseColor, glm::vec3 tipColor = glm::vec3(1.f));

    // Instancing:
    private:
    unsigned int instanceVBO; //data will be on layout = 4 
    int instanceCount;

    public:
    //folk I'm adding too many approaches lol
    void SetInstanceTransforms(std::vector<glm::mat4>& instanceTransforms);
    void SetInstanceTransforms(glm::mat4* instanceTransforms, int instanceCount); // could take ( std::vector<glm::mat4>.data(), std::vector<glm::mat4>.size() )

    void DrawInstanced(int instances = 1);
};

#define POSITION_LAYOUT 0
#define NORMAL_LAYOUT 1
#define TEXCOORD_LAYOUT 2
#define COLOR_LAYOUT 3
#define INSTANCE_LAYOUT 4