#include "Grass.hpp"

// Mesh Grass::generateMesh(glm::vec3 baseColor, glm::vec3 tipColor)
// {
//     const float vertices[] = 
//     {
//         //x-axis blade      //colors                                    //swaying force
//         -.025f, 0.f, 0.f,    baseColor.r, baseColor.g, baseColor.b,      0.f,
//         .025f, 0.f, 0.f,     baseColor.r, baseColor.g, baseColor.b,      0.f,

//         //z-axis blade
//         0.f, 0.f, -.025f,    baseColor.r, baseColor.g, baseColor.b,      0.f,
//         0.f, 0.f, .025f,     baseColor.r, baseColor.g, baseColor.b,      0.f,

//         //tip
//         0.f, 1.f, 0.f,      tipColor.r, tipColor.g, tipColor.b,         1.f
//     };
//     const unsigned int indices[] =
//     {
//         0, 1, 4,
//         2, 3, 4
//     };
//     return Mesh(vertices, indices, 35, 6, (const int[]){3, 3, 1}, 3, 7);
// }

Mesh Grass::generateMesh(glm::vec3 baseColor, glm::vec3 tipColor)
{
    const float vertices[] =
        {
            // x-axis blade      //colors                                    //swaying force
            -.025f, 0.f, 0.f, baseColor.r, baseColor.g, baseColor.b,         0.f,
            .025f, 0.f, 0.f, baseColor.r, baseColor.g, baseColor.b,          0.f,

            // tip
            0.f, 1.f, 0.f, tipColor.r, tipColor.g, tipColor.b,               1.f
        };
    const unsigned int indices[] =
        {
            0, 1, 2
        };
    return Mesh(vertices, indices, 21, 3, (const int[]){3, 3, 1}, 3, 7);
}

Grass::Grass(glm::vec3 baseColor, glm::vec3 tipColor):mesh(generateMesh(baseColor, tipColor)){}

Grass::~Grass(){} //I guess ~Mesh() is getting called here