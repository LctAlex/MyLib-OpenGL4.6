#include <iostream>
#include <vector>

#include <../include/glad/glad.h>
#include <../include/GLFW/glfw3.h>

#include "../utils/WINDOW.hpp"
#include "../utils/MESH.hpp"
#include "../utils/SHADER.hpp"
#include "../utils/CAMERA.hpp"

int main()
{
    Window window(800, 800, "C S");
    //window.ToggleFullscreen();

    Camera camera(window.GetPointer(), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, -1.f));
    camera.SetSpeed(10.f);

    const float vertices[] = 
    {
        -1.f, 1.f, 0.f,
        -1.f, -1.f, 0.f,
        1.f, 1.f, 0.f,
        1.f, -1.f, 0.f
    };

    // const float vertices[] = 
    // {
    //     -1.f, 1.f, 0.f,     1.0f, 0.0f, 0.0f,
    //     -1.f, -1.f, 0.f,    0.0f, 1.0f, 0.0f,
    //     1.f, 1.f, 0.f,      0.0f, 0.0f, 1.0f,
    //     1.f, -1.f, 0.f,     1.0f, 0.0f, 1.0f
    // };

    const unsigned int indices[] =
    {
        0, 1, 2,
        1, 2, 3
    };

    Mesh mesh(vertices, indices, 12, 6, 3, 3);
    // Mesh mesh(vertices, indices, 24, 6, 3, 6);
    // Shader shader("shaders/vertMVP.glsl", "shaders/Fog.glsl");
    Shader shader("shaders/vertMVP.glsl", "shaders/StaticEffect.glsl");

    glm::mat4 model(1.0f);

    glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)window.GetWidth()/(float)window.GetHeight(), 0.1f, 100.f);

    glm::mat4 view(1.0f);
    
    window.SetClearColor(.5f, .5f, .5f);
    while(!window.ShouldClose())
    {
        window.StartUpdate();
        window.ShowFPS();

        camera.Update(&window);
        view = camera.GetView();

        shader.Use();
        shader.SetUniformMat4("model", model);
        shader.SetUniformMat4("projection", projection);
        shader.SetUniformMat4("view", view);

        shader.SetUniformVec2("windowRes", glm::vec2(window.GetWidth(), window.GetHeight()));
        shader.SetUniformFloat("time", glfwGetTime());

        shader.SetUniformVec3("camPos", camera.GetPosition());
        
        mesh.Draw();

        window.EndUpdate();
    }

    mesh.Delete();
    window.Close();
    return 0;
}