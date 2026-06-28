#pragma once
#include "Mesh.hpp"
//This class will NOT remain inside the main /utils, but it may remain in an /examples folder
class Grass
{
    private:
    Mesh generateMesh(glm::vec3 baseColor, glm::vec3 tipColor);

    public:
    Mesh mesh;
    Grass(glm::vec3 baseColor = glm::vec3(0.3f, 0.8f, 0.2f), glm::vec3 tipColor = glm::vec3(0.2f, 0.5f, 0.1f));
    ~Grass();
};