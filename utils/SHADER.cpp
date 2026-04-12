#include "SHADER.hpp"

Shader::Shader(const char* vertexFileName, const char* fragmentFileName)
{
    //these will be the raw shader codes
    const char* vertexSourceCode;
    const char* fragmentSourceCode;

    //these will be the opened files
    std::ifstream vertexShaderFile;
    std::ifstream fragmentShaderFile;

    //for convenience, we'll put everything from the files into string streams
    std::stringstream vStream, fStream;
    //can't safely convert from sstream to C string directly, we'll put streams into strings
    std::string vString, fString;

    try
    {
        vertexShaderFile.open(vertexFileName);
        fragmentShaderFile.open(fragmentFileName);
        if(!vertexShaderFile.is_open()) throw std::runtime_error("ERROR::SHADER::VERTEX::OPEN_FAILURE");
        if(!fragmentShaderFile.is_open()) throw std::runtime_error("ERROR::FRAGMENT::VERTEX::OPEN_FAILURE");
    
        vStream << vertexShaderFile.rdbuf();
        fStream << fragmentShaderFile.rdbuf();
    
        vertexShaderFile.close();
        fragmentShaderFile.close();
    
        //converting streams into C strings
        vString = vStream.str();
        fString = fStream.str();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        exit(EXIT_FAILURE);
    }
    
    vertexSourceCode = vString.c_str();
    fragmentSourceCode = fString.c_str();

    int success = 1;
    char infoLog[256];

    unsigned int vShader;
    try
    {
        vShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vShader, 1, &vertexSourceCode, NULL);
        glCompileShader(vShader);
    
        glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vShader, 256, NULL, infoLog);
            std::cout << infoLog;
            throw std::runtime_error("ERROR::SHADER::VERTEX::COMPILATION_ERROR");
        }
    }
    catch(const std::exception &e)
    {
        std::cout << e.what() << '\n';
        exit(EXIT_FAILURE);
    }

    unsigned int fShader;
    try
    {
        fShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fShader, 1, &fragmentSourceCode, NULL);
        glCompileShader(fShader);
    
        glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(fShader, 256, NULL, infoLog);
            std::cout << infoLog;
            throw std::runtime_error("ERROR::SHADER::FRAGMENT::COMPILATION_ERROR");
        }
    }
    catch(const std::exception &e)
    {
        std::cout << e.what() << '\n';
        exit(EXIT_FAILURE);
    }

    try
    {
        ID = glCreateProgram();
        glAttachShader(ID, vShader);
        glAttachShader(ID, fShader);
        glLinkProgram(ID);
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(ID, 256, NULL, infoLog);
            std::cout << infoLog;
            throw std::runtime_error("ERROR::SHADER::LINKING_ERROR");
        }
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << '\n';
        exit(EXIT_FAILURE);
    }

    glDeleteShader(vShader);
    glDeleteShader(fShader);
}

void Shader::Use()
{
    glUseProgram(ID);
}

void Shader::Delete()
{
    glDeleteProgram(ID);
}

Shader::~Shader(){}

// I NEED A VECTOR TO HOLD ALL unfiromNames AND locations, so I dont call glGetUniformName everytime

void Shader::SetUniformInt(const GLchar* uniformName, int i)
{
    int loc = glGetUniformLocation(ID, uniformName);
    glUniform1i(loc, i);
}

void Shader::SetUniformFloat(const GLchar* uniformName, float f)
{
    int loc = glGetUniformLocation(ID, uniformName);
    glUniform1f(loc, f);
}

void Shader::SetUniformVec2(const GLchar* uniformName, glm::vec2 vec2)
{
    int loc = glGetUniformLocation(ID, uniformName);
    glUniform2fv(loc, 1, glm::value_ptr(vec2));
}

void Shader::SetUniformVec3(const GLchar* uniformName, glm::vec3 vec3)
{
    int loc = glGetUniformLocation(ID, uniformName);
    glUniform3fv(loc, 1, glm::value_ptr(vec3));
}

void Shader::SetUniformVec4(const GLchar* uniformName, glm::vec4 vec4)
{
    int loc = glGetUniformLocation(ID, uniformName);
    glUniform3fv(loc, 1, glm::value_ptr(vec4));
}

void Shader::SetUniformMat4(const GLchar* uniformName, glm::mat4 mat4)
{
    int loc = glGetUniformLocation(ID, uniformName);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat4));
}