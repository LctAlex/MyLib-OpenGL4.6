#include "Mesh.hpp"

Mesh::Mesh(const float *vertices, const unsigned int *indices, int vertSize, int indSize, const int sizes[], int sizesLength, int stride)
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
    for(int i = 0; i < sizesLength; i++)
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

Mesh::Mesh(const std::vector<float> &vertices, const std::vector<unsigned int> &indices) // positions, normals/colors, texCoords = 8 total
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

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

void Mesh::DrawInstanced(int instances)
{
    glBindVertexArray(VAO);
    glDrawElementsInstanced(GL_TRIANGLES, elements, GL_UNSIGNED_INT, 0, instances);
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
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

Mesh Mesh::createCanvas()
{ 
    const float vertices[] =
    {
        -1.f, -1.f, 0.f,    0.f,
        1.f, 1.f, 1.f,      1.f,
        -1.f, 1.f, 0.f,     1.f,
        1.f, -1.f, 1.f,     0.f
    };
    const unsigned int indices[] = 
    {
        0, 1, 2,
        0, 3, 1
    };

    return Mesh(vertices, indices, 12, 6, (const int[]){3, 2}, 2, 5);
}

Mesh Mesh::createGrassBlade()
{
    const float vertices[] = 
    {
        //x-axis blade
        -.5f, -.5f, 0.f,
        .5f, -.5f, 0.f,
        0.f, 1.f, 0.f,

        //z-axis blade
        0.f, -.5f, -.5f,
        0.f, -.5f, .5f, 
        0.f, 1.f, 0.f
    };
    const unsigned int indices[] =
    {
        0, 1, 2,
        3, 4, 5
    };

    return Mesh(vertices, indices, 18, 6, 3, 3);
}

Mesh Mesh::createQuadColor(glm::vec3 color)
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

//DELETE LATER

Mesh Mesh::createGrassBladeColor(glm::vec3 baseColor, glm::vec3 tipColor)
{
    const float vertices[] = 
    {
        //x-axis blade
        -.05f, 0.f, 0.f,    baseColor.r, baseColor.g, baseColor.b,
        .05f, 0.f, 0.f,     baseColor.r, baseColor.g, baseColor.b,

        //z-axis blade
        0.f, 0.f, -.05f,    baseColor.r, baseColor.g, baseColor.b,
        0.f, 0.f, .05f,     baseColor.r, baseColor.g, baseColor.b,

        //tip
        0.f, 1.f, 0.f,      tipColor.r, tipColor.g, tipColor.b
    };
    const unsigned int indices[] =
    {
        0, 1, 4,
        2, 3, 4
    };

    return Mesh(vertices, indices, 50, 6, 3, 6);
}