#pragma once
#include "../include/glad/glad.h"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

#include "ShaderData.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

class Shader
{
    public:
    unsigned int ID;
    Shader(const char* vertexFileName, const char* fragmentFileName);

    //void ReloadShader();
    void Use();
    void SetUniformInt(const GLchar* uniformName, int i);
    void SetUniformFloat(const GLchar* uniformName, float f);
    void SetUniformVec2(const GLchar* uniformName, glm::vec2 vec2); //floats
    void SetUniformVec3(const GLchar* uniformName, glm::vec3 vec3); //floats
    void SetUniformVec4(const GLchar* uniformName, glm::vec4 vec4); //floats
    void SetUniformMat4(const GLchar* uniformName, glm::mat4 mat4);
    ~Shader();
};

#define POSITION_COLOR "shaders/mesh/vertMVP_PC.glsl","shaders/mesh/fragMVP_PC.glsl"
#define POSITION_COLOR_INSTANCED "shaders/mesh/instanced/vertMVP_PCI.glsl", "shaders/mesh/fragMVP_PC.glsl"