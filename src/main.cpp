#include "../utils/Window.hpp"
#include "../utils/Framebuffer.hpp"
#include "../utils/Shader.hpp"
#include "../utils/Shape.hpp"
#include "../utils/Camera.hpp"

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"

#include <iostream>

void glfw_window_pos_callback(GLFWwindow* window, int xpos, int ypos)
{
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    win->ClearColor(0.f, 0.f, 0.f);
    glfwSwapBuffers(window);
}

int main()
{
    Window window; //glfwInit() + set basic hints;
    window.SetHint(HINT_UNRESIZABLE_WINDOW);
    window.Create(1000, 800, "test");

    //testing
    glfwSetWindowUserPointer(window.GetHandler(), &window);
    glfwSetWindowPosCallback(window.GetHandler(), glfw_window_pos_callback);

    const float v[] = {
        -.5f, -.5f, 0.f,
        .5f, -.5f, 0.f,
        .5f, .5f, 0.f,
        -.5f, .5f, 0.f};

    const unsigned int i[] = {
        0, 1, 2,
        2, 3, 0};

    Shape shape(v, i, 12, 6, 3, 3);
    Shader shapeShader("shaders/vertMVP.glsl", "shaders/frag.glsl");

    Camera camera(glm::vec3(0.f, 0.f, -3.f), glm::vec3(0.f));
    camera.SetProjectionToPersp(45.f, window.GetWidth(), window.GetHeight(), .1f, 100.f);

    ScreenFramebuffer screenFB(window.GetWidth(), window.GetHeight());
    bool resized = false;

    while(!window.ShouldClose())
    { 
        if(glfwGetTime()>3.f && !resized)
        {
            screenFB.Resize(100, 50);
        }

        window.StartDrawing();
        
        screenFB.Bind();
        
        glViewport(0, 0, screenFB.GetWidth(), screenFB.GetHeight()); //IMPORTANT
        glEnable(GL_DEPTH_TEST);

        window.ClearColor((unsigned int)GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, 1.f);

        shape.model = glm::rotate(shape.model, glm::radians(0.25f) * window.GetDeltaTime() * 10, glm::vec3(0.f, 0.f, 1.f));
        camera.UpdateVectors();

        shapeShader.Use();
        shapeShader.SetUniformMat4("model", shape.model);
        shapeShader.SetUniformMat4("view", camera.GetView());
        shapeShader.SetUniformMat4("projection", camera.GetProjection());

        shape.Draw();

        screenFB.Unbind();

        glViewport(0, 0, window.GetWidth(), window.GetHeight()); //IMPORTANT 2
        glDisable(GL_DEPTH_TEST);

        window.ClearColor(1.f, 1.f, 1.f);
        screenFB.Draw();

        window.TitleFPS();
        window.EndDrawing();
    }

    return 0;
}