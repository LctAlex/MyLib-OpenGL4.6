#include "VAO.hpp"

VBO::VBO(const std::vector<float>& vertices)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices.data()[0]), vertices.data(), GL_STATIC_DRAW); //need to return size of TOTAL DATA, not amount
}

VBO::VBO(const float* vertices, int size)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, size*sizeof(float), vertices, GL_STATIC_DRAW);
}

void VBO::Bind(){glBindBuffer(GL_ARRAY_BUFFER, ID);}
void VBO::Unbind(){glBindBuffer(GL_ARRAY_BUFFER, 0);}
void VBO::Delete(){glDeleteBuffers(1, &ID);}
VBO::~VBO(){glDeleteBuffers(1, &ID);}

EBO::EBO(const std::vector<unsigned int>& indices)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices.data()[0]), indices.data(), GL_STATIC_DRAW);
}

EBO::EBO(const unsigned int* indices, int size)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}

void EBO::Bind(){glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);}
void EBO::Unbind(){glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);}
void EBO::Delete(){glDeleteBuffers(1, &ID);}
EBO::~EBO(){glDeleteBuffers(1, &ID);}

    
template <typename T>
BO::BO(const std::vector<T>& data, GLenum target)
:target(target)
{
    glGenBuffers(1, &ID);
    glBindBuffer(target, ID);
    glBufferData(target, data.size() * sizeof(data.data()[0]), data.data(), GL_STATIC_DRAW);
}
void BO::Bind(){glBindBuffer(target, ID);}
void BO::Unbind(){glBindBuffer(target, 0);}
void BO::Delete(){glDeleteBuffers(1, &ID);}
BO::~BO(){glDeleteBuffers(1, &ID);}

VAO::VAO(const std::vector<float>& vertices, int size, int stride, const std::vector<unsigned int>& indices) //size = per group \n stride = per line
{
    glGenVertexArrays(1, &ID);
    glBindVertexArray(ID);
    VBO vbo(vertices);
    EBO ebo(indices);
    for(int i = 0; i < stride/size; i++) //invalid if size is inconsistent
    {
        glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, stride*sizeof(float), (void*)(i*size*sizeof(float)));
        glEnableVertexAttribArray(i);
    }
    
    Unbind();
    vbo.Unbind();
    ebo.Unbind();
    vbo.Delete(); //allowed since 3.3 OpenGL?????
    ebo.Delete();
}

VAO::VAO(std::vector<float>& vertices, std::vector<unsigned int>& indices, int size, int stride) //NON-CONSTANTS version
{
    glGenVertexArrays(1, &ID);
    glBindVertexArray(ID);
    VBO vbo(vertices);
    EBO ebo(indices);
    for(int i = 0; i < stride/size; i++)
    {
        glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, stride*sizeof(float), (void*)(i*size*sizeof(float)));
        glEnableVertexAttribArray(i);
    }
    
    Unbind();
    vbo.Unbind();
    ebo.Unbind();
    vbo.Delete();
    ebo.Delete();
}

VAO::VAO(const float* vertices, int vertSize, const unsigned int* indices, int indSize, int size, int stride)
{
    glGenVertexArrays(1, &ID);
    glBindVertexArray(ID);
    VBO vbo(vertices, vertSize);
    EBO ebo(indices, indSize);
    for(int i = 0; i < stride/size; i++)
    {
        glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, stride*sizeof(float), (void*)(i*size*sizeof(float)));
        glEnableVertexAttribArray(i);
    }
    
    Unbind();
    vbo.Unbind();
    ebo.Unbind();
    vbo.Delete();
    ebo.Delete();
}

//should make a version where:
//avoid size and stride params and just take everything in order (pos, normals/colors, texcoords) (recommended for tinyobjloader)

VAO::VAO(){glGenVertexArrays(1, &ID);}

void VAO::AddData(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, int size, int stride)
{
    glBindVertexArray(ID);
    VBO vbo(vertices);
    EBO ebo(indices);
    for(int i = 0; i < stride/size; i++)
    {
        glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, stride*sizeof(float), (void*)(i*size*sizeof(float)));
        glEnableVertexAttribArray(i);
    }
    
    Unbind();
    vbo.Unbind();
    ebo.Unbind();
    vbo.Delete();
    ebo.Delete();
}

void VAO::Bind(){glBindVertexArray(ID);}
void VAO::Unbind(){glBindVertexArray(0);}
void VAO::Delete(){glDeleteVertexArrays(1, &ID);}
VAO::~VAO(){glDeleteVertexArrays(1, &ID);}

void VAO::Draw(int indicesSize)
{
    VAO::Bind();
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
}

//I should NOT delete VBO & EBO while still using!!