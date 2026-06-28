#include "../utils/Window.hpp"
#include "../utils/Framebuffer.hpp"
#include "../utils/Shader.hpp"
#include "../utils/Mesh.hpp"
#include "../utils/Camera.hpp"
#include "../utils/Random.hpp"
#include "../utils/Input.hpp"

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"

#include <iostream>

#include "../utils/Grass.hpp"
#define GRASS_POSITION_COLOR_INSTANCED "shaders/grass/grassVert.glsl", "shaders/mesh/fragMVP_PC.glsl"
#define SHADOWED_GRASS_POSITION_COLOR_INSTANCED "shaders/grass/grassShadowVert.glsl", "shaders/grass/grassShadowFrag.glsl"

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

//for shadows:
struct Light
{
    glm::vec3 pos;
    glm::mat4 proj;
    glm::mat4 matrix;
    Light(glm::vec3 position, glm::mat4 lightProjection):pos(position), proj(lightProjection)
    {
        glm::mat4 lightView = glm::lookAt(position,
                                          glm::vec3(0.0f, 0.0f, 0.0f),
                                          glm::vec3(0.0f, 1.0f, 0.0f));
        matrix = lightProjection * lightView;
    }
};

int main()
{
    Window window;
    window.SetHint(HINT_UNRESIZABLE_WINDOW);
    window.Create(1920, 1600, "test");
    //window.Create(1560, 1140, "test");

    glfwSetErrorCallback(glfw_error_callback);
    //next lines are useless because of camera
    glfwSetWindowUserPointer(window.GetHandler(), &window);
    glfwSetWindowPosCallback(window.GetHandler(), glfw_window_pos_callback);

    Camera camera(glm::vec3(-3.f, 2.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
    camera.SetSensitivity(0.018f);
    camera.SetProjectionToPersp(45.f, window.GetWidth(), window.GetHeight(), .1f, 100.f);
    //camera.SetProjectionToOrtho(-10, 10, -10, 10, -10, 10);
    glfwSetWindowUserPointer(window.GetHandler(), &camera);
    glfwSetCursorPosCallback(window.GetHandler(), camera.glfw_mouse_callback);
    glfwSetInputMode(window.GetHandler(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glm::vec3 grassColor = glm::vec3(0.2f, 0.6f, 0.3f);
    Grass grass(grassColor);
    Shader grassShader(SHADOWED_GRASS_POSITION_COLOR_INSTANCED);

    Mesh plane = Mesh::createColoredQuad(grassColor);
    plane.model = glm::translate(glm::mat4(1.0f), glm::vec3(0.f));

    int grassCount = 1000; //
    float planeScale = 2.5f; //

    plane.model = glm::scale(plane.model, glm::vec3(-planeScale, 1.f, -planeScale));
    plane.model = glm::rotate(plane.model, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
    Shader planeShader(POSITION_COLOR_SHADOW);

    Mesh cube = Mesh::createColoredCube(glm::vec3(1.f, 0.f, 0.f));
    cube.model = glm::scale(cube.model, glm::vec3(0.5f));
    cube.model = glm::translate(cube.model, glm::vec3(0.f, 2.5f, 0.f));
    cube.model = glm::rotate(cube.model, glm::radians(45.f), glm::vec3(1.f, 1.f, 1.f));
    //cube also needs a (default) depthShader in order to EMIT shadows
    Shader customDepthShader(GRASS_DEPTH_RENDER_FRAMEBUFFER_SHADER);

    glm::mat4 transformations[grassCount] = {0};
    int index = 0;
    for (int i = 0; i < std::sqrt(grassCount); i++)
    {
        for (int j = 0; j < std::sqrt(grassCount); j++)
        {
            if (index >= grassCount)
            {
                std::cout << "WARNING! Vector index reached : " + std::to_string(index) + '\n';
                break;
            }
            double x = -planeScale + i * ((double)(2 * planeScale) / std::sqrt(grassCount)) + 0.025f * randomFloat(1.f, 4.f);
            double z = -planeScale + j * ((double)(2 * planeScale) / std::sqrt(grassCount)) + 0.025f * randomFloat(1.f, 4.f);
            glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(x, 0.f, z));
            model = glm::scale(model, glm::vec3(1.f, randomFloat(0.3f, 0.7f), 1.f));
            model = glm::rotate(model, glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));
            transformations[index++] = model;
        }
    }

    grass.mesh.SetInstanceTransforms(transformations, grassCount);

    float orthoSize = 3.f;
    Light sun(glm::vec3(-3.f, 2.f, 0.f), glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize * 2, -10.f, 30.f));

    Input input(window.GetHandler());

    //ScreenFramebuffer sFB(window.GetWidth(), window.GetHeight());
    DepthFramebuffer dFB(window.GetWidth(), window.GetHeight());

    while(!window.ShouldClose())
    {
        //camera.ResetVectors(glm::vec3(camera.GetPosition().x, abs(sin(glfwGetTime()/5.f)), 3.f));
        camera.ProcessMovement(input, window.GetDeltaTime());
        camera.UpdateVectors();

        window.StartDrawing();

        // grass.mesh.model = glm::rotate(grass.mesh.model, glm::radians(1.f), glm::vec3(0.f, 1.f, 0.f));
        cube.model = glm::rotate(cube.model, glm::radians(0.5f), glm::vec3(0.f, 1.f, 0.f));

        float currentTime = glfwGetTime() * 0.3f;

        ////generating depth texture (for shadows)
        //in order for the cube to appear inside the depth texture, we must add another shader to it(uses another vertex shader)
        dFB.Bind();
        customDepthShader.Use();
        customDepthShader.SetUniformMat4("lightSpaceMatrix", sun.matrix);
        customDepthShader.SetUniformFloat("time", currentTime);
        grass.mesh.DrawInstanced(grassCount);

        dFB.depthShader->Use();
        dFB.depthShader->SetUniformMat4("lightSpaceMatrix", sun.matrix);
        dFB.depthShader->SetUniformMat4("model", cube.model);
        cube.Draw();
        dFB.Unbind();

        //dFB.DrawToScreen(window.GetWidth(), window.GetHeight());
        ////now we have the depth texture inside dFB.tex.GetHandler();

        glEnable(GL_DEPTH_TEST);
        window.ClearColor((GLbitfield)GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, 0.4f, 0.5f, 0.8f);

        planeShader.Use();
        planeShader.SetUniformMat4("view", camera.GetView());
        planeShader.SetUniformMat4("projection", camera.GetProjection());
        planeShader.SetUniformMat4("model", plane.model);
        planeShader.SetUniformMat4("lightSpaceMatrix", sun.matrix);
        dFB.tex->Bind(0);
        planeShader.SetUniformInt("shadowMap", 0);
        planeShader.SetUniformFloat("shadowDarkness", 0.2f);
        plane.Draw();
        planeShader.SetUniformMat4("model", cube.model);
        cube.Draw();
        dFB.tex->Unbind();

        grassShader.Use();
        grassShader.SetUniformMat4("view", camera.GetView());
        grassShader.SetUniformMat4("projection", camera.GetProjection());
        grassShader.SetUniformFloat("time", currentTime);
        //shadow
        grassShader.SetUniformMat4("lightSpaceMatrix", sun.matrix);
        dFB.tex->Bind(0);
        grassShader.SetUniformInt("shadowMap",0);
        grass.mesh.DrawInstanced(grassCount);
        dFB.tex->Unbind();

        window.TitleFPS();
        window.EndDrawing();
    }

    return 0;
}
