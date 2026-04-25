#pragma once
#include "../include/glad/glad.h"
#include <vector>

class VBO
{
    private:
    unsigned int ID;
    public:
    VBO(const std::vector<float>& vertices);
    VBO(const float* vertices, int size);
    void Bind();
    void Unbind();
    void Delete();
    ~VBO();
};

class EBO
{
    private:
    unsigned int ID;
    public:
    EBO(const std::vector<unsigned int>& indices);
    EBO(const unsigned int* indices, int size);
    void Bind();
    void Unbind();
    void Delete();
    ~EBO();
};

class BO //General Buffer-Object
{
    private:
    unsigned int ID;
    GLenum target;
    public:
    template <typename T>
    BO(const std::vector<T>& data, GLenum target);
    void Bind();
    void Unbind();
    void Delete();
    ~BO();
};

class VAO
{
    private:
    unsigned int ID;
    public:
    VAO(const std::vector<float>& vertices, int size, int stride, const std::vector<unsigned int>& indices);
    VAO(std::vector<float>& vertices, std::vector<unsigned int>& indices, int size, int stride);
    VAO(const float* vertices, int vertSize, const unsigned int* indices, int indSize, int size, int stride);
    
    VAO();
    void AddData(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, int size, int stride);

    void Bind();
    void Unbind();
    void Delete();
    ~VAO();
    void Draw(int indicesSize);
};