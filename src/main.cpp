#include <iostream>
#include <vector>

#include <../include/glad/glad.h>
#include <../include/GLFW/glfw3.h>

#include "../utils/WINDOW.hpp"
#include "../utils/MESH.hpp"
#include "../utils/SHADER.hpp"
#include "../utils/CAMERA.hpp"
#include "../utils/VAO.hpp"

struct WindowUserPointer //this would kinda force Camera and Window classes to depend on each other
{
    Window* window;
    Camera* camera;
};
//Thus a System (or some name) class would combine everything...A System class would: process inputs, create window, handle camera

int main()
{
    //SetWindowHint(UNRESIZABLE_WINDOW);
    Window window(800, 800, "Framebuffer");

    //Camera camera(window.GetPointer(), glm::vec3(0.f, 0.f, 3.f));
    window.EnableCursor();

    // WindowUserPointer data;
    // glfwSetWindowUserPointer(window.GetPointer(), &data);

    const float vertices[] =
    {
        -0.5f, 0.5f, 0.f,
        -0.5f, -0.5f, 0.f,
        .5f, .5f, 0.f,
        .5f, -.5f, 0.f
    };

    const unsigned int indices[] =
    {
        0, 1, 2,
        2, 1, 3
    };

    Mesh mesh(vertices, indices, 12, 6, 3, 3);
    Shader shader("shaders/vertMVPpc.glsl", "shaders/fragpc.glsl");

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.f, 0.f, -3.f));
    glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)window.GetWidth() / (float)window.GetHeight(), 0.1f, 100.f);
    
    while (!window.ShouldClose())
    {
        if(window.IsKeyPressed(GLFW_KEY_SPACE)) window.SetResolution(window.GetResX()/2, window.GetResY()/2);
        if(window.IsKeyPressed(GLFW_KEY_R)) window.SetResolution(window.GetWidth(), window.GetHeight());
        if(window.IsKeyPressed(GLFW_KEY_ENTER)) window.SetResolution(window.GetWidth()+1, window.GetHeight()+1);
        if(window.IsKeyPressed(GLFW_KEY_I)) std::cout << "Window resolution: " 
                                                    << window.GetWidth() << ':' << window.GetHeight() << '\n';
        if(window.IsKeyPressed(GLFW_KEY_H)) window.SetBlendingMode(GL_NEAREST);
        if(window.IsKeyPressed(GLFW_KEY_M)) window.SetBlendingMode(GL_LINEAR);
        window.StartUpdate();
        window.ClearColorDepth(0.3f, 0.3f, 0.3f);
        //camera.Update(&window);
        
        model = glm::rotate(model, glm::radians(1.f), glm::vec3(0.1f, 0.f, 1.f));
        shader.Use();
        //shader.SetUniformMat4("view", camera.GetView());
        shader.SetUniformMat4("view", glm::mat4(1.f));
        shader.SetUniformMat4("model", model);
        shader.SetUniformMat4("projection", projection);
        mesh.Draw();

        window.ShowFPS();
        window.EndUpdate();
    }

    window.Close();
    shader.Delete();
    mesh.Delete();
    return 0;
}

//NOTE: when setting resolution/resizing window, the viewport must also be adjusted for the framebuffer, otherwise weird things happen
//TASK: Implement the framebuffer into the Window now. FUCK I NEED 2D and 3D windows / methods!!!