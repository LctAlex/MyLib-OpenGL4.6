#pragma once
#include "Mesh.hpp"
//Warning!
//This class is just an example of how the programmer can use the Framework!
//It will NOT remain inside the main project, but it may remain in the /examples folder
class Grass
{
    private:
    Mesh generateMesh(glm::vec3 baseColor, glm::vec3 tipColor);

    public:
    Mesh mesh;
    Grass(glm::vec3 baseColor = glm::vec3(0.3f, 0.8f, 0.2f), glm::vec3 tipColor = glm::vec3(0.2f, 0.5f, 0.1f));
    ~Grass();
};