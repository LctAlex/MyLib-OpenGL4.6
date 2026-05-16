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
    Mesh(const float *vertices, const unsigned int *indices, int vertSize, int indSize, const int sizes[], int sizesAmount, int stride); //(const int[]){...}, size
    Mesh(const float *vertices, const unsigned int *indices, int vertSize, int indSize, int size, int stride);
    Mesh(const std::vector<float> &vertices, const std::vector<unsigned int> &indices, const std::vector<int> sizes, int stride); //const std::vector<int>{...}
    Mesh(const std::vector<float> &vertices, const std::vector<unsigned int> &indices, int size, int stride);
    Mesh(const std::vector<float> &vertices, const std::vector<unsigned int> &indices); // positions, normals/colors, texCoords = 8 total
    void Draw();

    ~Mesh();

    //static functions: do not DEPEND on a class' instance!
    static Mesh createQuad();
    static Mesh createCube();
    static Mesh createCanvas();

    //fun
    static Mesh createQuadColored(glm::vec3 color);

    // Instancing:
    private:
    unsigned int instanceVBO; //data will be on layout = 4 
    int instanceCount;

    public:
    //SetInstanceTransforms() is intensive, should be set ONCE (make sure it is set once somehow)
    void SetInstanceTransforms(std::vector<glm::mat4>& instanceTransforms);
    void SetInstanceTransforms(glm::mat4* instanceTransforms, int instanceCount); // could take ( std::vector<glm::mat4>.data(), std::vector<glm::mat4>.size() )
    //for just moving the instances (4x more lightweight)
    // void SetInstanceTranslations(std::vector<glm::vec3>& instanceTranslations);
    // void SetInstanceTranslations(glm::vec3* instanceTranslations, int instanceCount);

    void DrawInstanced(int instances = 1);
};

#define POSITION_LAYOUT 0
#define NORMAL_LAYOUT 1
#define TEXCOORD_LAYOUT 2
#define COLOR_LAYOUT 3
#define INSTANCE_LAYOUT 4