#pragma once
#include <iostream> //may not need this if I use try and catch inside
#include <fstream>
#include <sstream>

#include "../include/glad/glad.h"

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

class Shader
{
    public:
    unsigned int ID;
    Shader(const char* vertexFileName, const char* fragmentFileName);

    void Use();
    void SetUniformInt(const GLchar* uniformName, int i);
    void SetUniformFloat(const GLchar* uniformName, float f);
    void SetUniformVec2(const GLchar* uniformName, glm::vec2 vec2); //floats
    void SetUniformVec3(const GLchar* uniformName, glm::vec3 vec3); //floats
    void SetUniformVec4(const GLchar* uniformName, glm::vec4 vec4); //floats
    void SetUniformMat4(const GLchar* uniformName, glm::mat4 mat4);
    void Delete();
    ~Shader();
};