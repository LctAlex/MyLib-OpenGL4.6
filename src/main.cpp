#include "../utils/SYSTEM.hpp"

#include "../utils/MESH.hpp"
#include "../utils/SHADER.hpp"

//SetAspectRatio()
//ToggleFullscreen()
//SetWindowHint(HINT_WINDOWED_FULLSCREEN)

//A fucking mess ^


int main()
{
    System system;

    system.SetWindowHint(HINT_WINDOWED_FULLSCREEN);

    system.CreateWindow(800, 800, "Hello System");
    system.CreateCamera();

    const float vertices[] =
    {
        -.5f, .5f, 0.f,
        -.5f, -.5f, 0.f,
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

    glm::mat4 model = glm::mat4(1.f);
    glm::mat4 proj = glm::perspective(glm::radians(45.f), 
                    (float)system.window->GetWidth()/system.window->GetHeight(), 0.1f, 100.f);
    
    while(!system.window->ShouldClose())
    {

        if(system.window->IsKeyPressed(GLFW_KEY_SPACE)) system.window->SetResolution(system.window->GetResX()/2,
                                                                                    system.window->GetResY()/2);
        if(system.window->IsKeyPressed(GLFW_KEY_R)) system.window->SetResolution(system.window->GetWidth(),
                                                                                    system.window->GetHeight());
        
        if(system.window->IsKeyPressed(GLFW_KEY_E))
        {
            system.window->DisableCursor();
            system.camera->SetCursorPosCallback(system.window->GetPointer(), true);
        }
        if (system.window->IsKeyPressed(GLFW_KEY_N))
        {
            system.window->EnableCursor();
            system.camera->SetCursorPosCallback(system.window->GetPointer(), false);
        }

        system.window->StartUpdate();
        system.window->ClearColorDepth();

        model = glm::rotate(model, glm::radians(1.f), glm::vec3(0.f, 0.f, 0.5f));

        system.camera->Update(system.window);

        shader.Use();
        shader.SetUniformMat4("model", model);
        shader.SetUniformMat4("projection", proj);
        shader.SetUniformMat4("view", system.camera->GetView());

        mesh.Draw();

        system.window->EndUpdate();
    }

    system.Close();
    return 0;
}