#include "Mesh.hpp"

Mesh::Mesh(const float *vertices, const unsigned int *indices, int vertSize, int indSize, const int sizes[], int sizesAmount, int stride)
{
    elements = indSize;
    model = glm::mat4(1.0f);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertSize * sizeof(float), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indSize * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    int offset = 0;
    for(int i = 0; i < sizesAmount; i++)
    {
        glVertexAttribPointer(i, sizes[i], GL_FLOAT, GL_FALSE, stride * sizeof(float), (void *)(offset * sizeof(float)));
        glEnableVertexAttribArray(i);
        offset += sizes[i];
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Mesh::Mesh(const float *vertices, const unsigned int *indices, int vertSize, int indSize, int size, int stride)
{
    elements = indSize;
    model = glm::mat4(1.0f);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertSize * sizeof(float), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indSize * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    for (int i = 0; i < stride / size; i++)
    {
        glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void *)(i * size * sizeof(float)));
        glEnableVertexAttribArray(i);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Mesh::Mesh(const std::vector<float> &vertices, const std::vector<unsigned int> &indices, const std::vector<int> sizes, int stride)
{
    elements = indices.size();
    model = glm::mat4(1.0f);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    for (int i = 0; i < sizes.size(); i++)
    {
        glVertexAttribPointer(i, sizes[i], GL_FLOAT, GL_FALSE, stride * sizeof(float), (void *)(i * sizes[i] * sizeof(float)));
        glEnableVertexAttribArray(i);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Mesh::Mesh(const std::vector<float> &vertices, const std::vector<unsigned int> &indices, int size, int stride)
{
    elements = indices.size();
    model = glm::mat4(1.0f);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    for (int i = 0; i < stride / size; i++)
    {
        glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void *)(i * size * sizeof(float)));
        glEnableVertexAttribArray(i);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Mesh::Mesh(const std::vector<float> &vertices, const std::vector<unsigned int> &indices) // positions, normals, texCoords = 8 total
{
    elements = indices.size();
    model = glm::mat4(1.0f);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(POSITION_LAYOUT, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(0));
    glEnableVertexAttribArray(POSITION_LAYOUT);
    glVertexAttribPointer(NORMAL_LAYOUT, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(NORMAL_LAYOUT);
    glVertexAttribPointer(TEXCOORD_LAYOUT, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(TEXCOORD_LAYOUT);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::Draw()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glDeleteBuffers(1, &instanceVBO);
}

Mesh Mesh::createQuad()
{
    const float vertices[] =
    {
        -1.f, -1.f, 0.f,
        1.f, 1.f, 0.f,
        -1.f, 1.f, 0.f,
        1.f, -1.f, 0.f
    };
    const unsigned int indices[] = 
    {
        0, 1, 2,
        0, 3, 1
    };
    return Mesh(vertices, indices, 12, 6, 3, 3);
}

// Mesh Mesh::createCube(){}

Mesh Mesh::createCanvas()
{ 
    const float vertices[] =
    {
        //X Y           //TexCoords
        -1.f, -1.f,      0.f, 0.f,
        1.f, 1.f,        1.f, 1.f,
        -1.f, 1.f,       0.f, 1.f,
        1.f, -1.f,       1.f, 0.f
    };
    const unsigned int indices[] = 
    {
        0, 1, 2,
        0, 3, 1
    };
    return Mesh(vertices, indices, 16, 6, (const int[]){2, 2}, 2, 4);
}

Mesh Mesh::createColoredQuad(glm::vec3 color)
{
    const float vertices[] =
    {
        -1.f, -1.f, 0.f,    color.r, color.g, color.b, 
        1.f, 1.f, 0.f,      color.r, color.g, color.b, 
        -1.f, 1.f, 0.f,     color.r, color.g, color.b, 
        1.f, -1.f, 0.f,      color.r, color.g, color.b,
    };
    const unsigned int indices[] = 
    {
        0, 1, 2,
        0, 3, 1
    };
    return Mesh(vertices, indices, 24, 6, 3, 6);
}

Mesh Mesh::createColoredCube(glm::vec3 color)
{
    const float vertices[] = 
    {
        -1.f, -1.f, 1.f,    color.r, color.g, color.b,
        1.f, -1.f, 1.f,     color.r, color.g, color.b,
        1.f, 1.f, 1.f,      color.r, color.g, color.b,
        -1.f, 1.f, 1.f,     color.r, color.g, color.b,

        -1.f, -1.f, -1.f,    color.r, color.g, color.b,
        1.f, -1.f, -1.f,     color.r, color.g, color.b,
        1.f, 1.f, -1.f,      color.r, color.g, color.b,
        -1.f, 1.f, -1.f,     color.r, color.g, color.b,
    };

    const unsigned int indices[] = 
    {
        0, 1, 2,    2, 3, 0,
        1, 5, 6,    6, 2, 1,
        5, 4, 7,    7, 6, 5,
        4, 0, 3,    3, 7, 4,
        3, 2, 6,    6, 7, 3,
        0, 1, 5,    5, 4, 0
    };

    // const float vertices[] =
    // {
    //     -0.5f, -0.5f,  0.5f,    color.r, color.g, color.b,
    //     0.5f, -0.5f,  0.5f,     color.r, color.g, color.b,
    //     -0.5f,  0.5f,  0.5f,    color.r, color.g, color.b,
    //     0.5f,  0.5f,  0.5f,     color.r, color.g, color.b,

    //     -0.5f, -0.5f, -0.5f,    color.r, color.g, color.b,
    //     0.5f, -0.5f, -0.5f,     color.r, color.g, color.b,
    //     -0.5f,  0.5f, -0.5f,    color.r, color.g, color.b,
    //     0.5f,  0.5f, -0.5f,     color.r, color.g, color.b,
    // };
    // const unsigned int indices[] =
    // {
    //     0, 1, 3,
    //     3, 2, 0,

    //     4, 0, 2,
    //     2, 6, 4,

    //     5, 4, 7,
    //     7, 6, 5,

    //     1, 5, 7,
    //     7, 3, 1,

    //     3, 2, 7,
    //     7, 6, 3,

    //     4, 5, 1,
    //     1, 0, 4
    // };

    // std::vector<unsigned int> indices = {
    //     // Front
    //     0, 2, 1, 2, 3, 1,
    //     // Back
    //     4, 5, 6, 5, 7, 6,
    //     // Left
    //     0, 4, 2, 4, 6, 2,
    //     // Right
    //     1, 5, 3, 5, 7, 3,
    //     // Top
    //     2, 6, 3, 6, 7, 3,
    //     // Bottom
    //     0, 1, 4, 1, 5, 4};

    return Mesh(vertices, indices, 48, 36, 3, 6);
}

//Instancing:
//This is expensive! Use it only for instance initialization!
void Mesh::SetInstanceTransforms(std::vector<glm::mat4>& instanceTransforms)
{
    instanceCount = instanceTransforms.size();

    glBindVertexArray(VAO);

    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * instanceTransforms.size(), &instanceTransforms[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    for (int i = 0; i < 4; i++)
    {
        glVertexAttribPointer(INSTANCE_LAYOUT + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(i * sizeof(glm::vec4)));
        glEnableVertexAttribArray(INSTANCE_LAYOUT + i);
    }

    for (int i = 0; i < 4; i++)
    {
        glVertexAttribDivisor(INSTANCE_LAYOUT + i, 1);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::SetInstanceTransforms(glm::mat4 *instanceTransforms, int instanceCount)
{
    this->instanceCount = instanceCount; //could check for nullptr inside the transforms here (instanceCount goes out of bounds)
    //(if (!transformations[instanceCount]) {...} , or some way to tell garbage values)

    glBindVertexArray(VAO);

    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * instanceCount, &instanceTransforms[0], GL_STATIC_DRAW);
    
    //set data at INSTANCE_LAYOUT
    for(int i = 0; i < 4; i++)
    {
        glVertexAttribPointer(INSTANCE_LAYOUT + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * sizeof(glm::vec4)));
        glEnableVertexAttribArray(INSTANCE_LAYOUT + i);
    }

    for(int i = 0; i < 4; i++)
    {
        glVertexAttribDivisor(INSTANCE_LAYOUT + i, 1); //"For attribute index (n+i)(4 for example), advance to the next value once per instance instead of once per vertex"
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::DrawInstanced(int instances)
{
    glBindVertexArray(VAO);
    glDrawElementsInstanced(GL_TRIANGLES, elements, GL_UNSIGNED_INT, 0, instances);
}