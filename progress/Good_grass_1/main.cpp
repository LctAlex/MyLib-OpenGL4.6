#include "../utils/Window.hpp"
//#include "../utils/Framebuffer.hpp"
#include "../utils/Shader.hpp"
#include "../utils/Mesh.hpp"
#include "../utils/Camera.hpp"
#include "../utils/Random.hpp"

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"

#include <iostream>

#include "../utils/Grass.hpp"
#define GRASS_POSITION_COLOR_INSTANCED "shaders/grass/grassVert.glsl", "shaders/mesh/fragMVP_PC.glsl"

void glfw_window_pos_callback(GLFWwindow* window, int xpos, int ypos)
{
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    win->ClearColor(0.f, 0.f, 0.f);
    glfwSwapBuffers(window);
}

void glfw_error_callback(int error_code, const char* description)
{
    std::cout << "ERROR: " << description << " (at " << error_code << ")\n";
}

int main()
{
    Window window;
    window.Create(1560, 1140, "test");

    //testing
    glfwSetWindowUserPointer(window.GetHandler(), &window);
    glfwSetWindowPosCallback(window.GetHandler(), glfw_window_pos_callback);
    glfwSetErrorCallback(glfw_error_callback);

    Camera camera(glm::vec3(0.f, 2.5f, 5.5f), glm::vec3(0.f));
    camera.SetProjectionToPersp(45.f, window.GetWidth(), window.GetHeight(), .1f, 100.f);
    camera.UpdateVectors();

    glm::vec3 grassColor = glm::vec3(0.2f, 0.6f, 0.3f);
    Mesh plane = Mesh::createQuadColored(grassColor);
    plane.model = glm::translate(glm::mat4(1.0f), glm::vec3(0.f));

    float planeScale = 2.5f; //
    
    plane.model = glm::scale(plane.model, glm::vec3(planeScale, 1.f, planeScale));
    plane.model = glm::rotate(plane.model, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
    Shader planeShader(POSITION_COLOR);

    Grass grass(grassColor);
    Shader grassShader(GRASS_POSITION_COLOR_INSTANCED);

    int grassCount = 10000; //

    glm::mat4 transformations[grassCount] = {0};
    int index = 0;
    for (int i = 0; i < std::sqrt(grassCount); i++) // God bless kofybrek for this masterpiece
    {
        for (int j = 0; j < std::sqrt(grassCount); j++)
        {
            if (index >= grassCount)
            {
                std::cout << "WARNING! Vector index reached : " + std::to_string(index) + '\n'; //this shit gets called for some reason
                break;
            }
            double x = -planeScale + i * ((double)(2 * planeScale) / std::sqrt(grassCount)) + 0.025f * randomFloat(1.f, 4.f);
            double z = -planeScale + j * ((double)(2 * planeScale) / std::sqrt(grassCount)) + 0.025f * randomFloat(1.f, 4.f);
            glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(x, 0.f, z));
            model = glm::scale(model, glm::vec3(1.f, randomFloat(0.5f, 1.f), 1.f));
            transformations[index++] = model;
        }
    } // I need a Random class for this

    grass.mesh.SetInstanceTransforms(transformations, grassCount);

    glEnable(GL_DEPTH_TEST); //find a way to blend this with ClearColor()
    while(!window.ShouldClose())
    { 
        window.StartDrawing();

        window.ClearColor((GLbitfield)GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, 0.4f, 0.5f, 0.8f);

        // camera.ResetVectors(glm::vec3(camera.GetPosition().x, abs(sin(glfwGetTime()/5.f)), 3.f));
        // camera.UpdateVectors();

        planeShader.Use();
        planeShader.SetUniformMat4("view", camera.GetView());
        planeShader.SetUniformMat4("projection", camera.GetProjection());
        planeShader.SetUniformMat4("model", plane.model);
        plane.Draw();

        grassShader.Use();
        grassShader.SetUniformMat4("view", camera.GetView());
        grassShader.SetUniformMat4("projection", camera.GetProjection());
        grassShader.SetUniformFloat("time", glfwGetTime());
        grass.mesh.DrawInstanced(grassCount);

        //Draw(Shader& shader, Camera& camera)
        //DrawInstanced(Shader& shader, Camera& camera, int count)

        window.TitleFPS();
        window.EndDrawing();
    }

    return 0;
}