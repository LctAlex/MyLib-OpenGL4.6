// #include "../utils/Window.hpp"
// //#include "../utils/Framebuffer.hpp"
// #include "../utils/Shader.hpp"
// #include "../utils/Mesh.hpp"
// #include "../utils/Camera.hpp"

// #include "../include/glm/glm.hpp"
// #include "../include/glm/gtc/matrix_transform.hpp"

// #include <iostream>

// void glfw_window_pos_callback(GLFWwindow* window, int xpos, int ypos)
// {
//     Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
//     win->ClearColor(0.f, 0.f, 0.f);
//     glfwSwapBuffers(window);
// }

// void glfw_error_callback(int error_code, const char* description)
// {
//     std::cout << "ERROR: " << description << " (at " << error_code << ")\n";
// }

// int main()
// {
//     Window window;
//     window.Create(1000, 800, "test");

//     //testing
//     glfwSetWindowUserPointer(window.GetHandler(), &window);
//     glfwSetWindowPosCallback(window.GetHandler(), glfw_window_pos_callback);
//     glfwSetErrorCallback(glfw_error_callback);

//     Camera camera(glm::vec3(0.f, 0.f, +3.f), glm::vec3(0.f));
//     camera.SetProjectionToPersp(45.f, window.GetWidth(), window.GetHeight(), .1f, 100.f);
//     camera.UpdateVectors();
    
//     glm::mat4 transformations[50];
//     int index = 0;
//     for (int i = 1; i <= 10; i++)
//     {
//         for (int j = 1; j <= 5; j++)
//         {
//             if (index >= 50)
//             break;
//             glm::mat4 model(1.0f);
//             model = glm::translate(glm::mat4(1.f), glm::vec3(-1.f + i * 0.2f - 0.1f, 0.f, -1.f + j * 0.4f - 0.2f));
//             //model = glm::rotate(model, glm::degrees((float)index), glm::vec3(0.f, 0.f, 1.f));
//             transformations[index++] = model;
//         }
//     }
//     Mesh plane = Mesh::createGrassBladeColor(glm::vec3(0.f, 0.5f, 0.f));
//     plane.SetInstanceTransforms(transformations, 50);
//     plane.model = glm::translate(glm::mat4(1.0f), glm::vec3(0.f));
//     plane.model = glm::rotate(plane.model, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
//     Shader planeShader(POSITION_COLOR);
//     Shader instancedPlaneShader(POSITION_COLOR_INSTANCED);

//     glEnable(GL_DEPTH_TEST); //find a way to blend this with ClearColor()
//     while(!window.ShouldClose())
//     { 
//         //heavy testing: THIS IS INTENSIVE! TODO: make a function to rotate INSIDE THE SHADER via uniforms
//         for(int i = 4; i < 50; i += 3)
//         {
//             glm::mat4 newModel = transformations[i];
//             newModel = glm::rotate(newModel, glm::degrees(window.GetDeltaTime()/10.f), glm::vec3(0.f, 1.f, 0.f));
//             transformations[i] = newModel;
//         }
//         plane.SetInstanceTransforms(transformations, 50);

//         plane.model = glm::rotate(plane.model, glm::degrees(window.GetDeltaTime()/100.f), glm::vec3(0.5f, .7f, 0.5f));

//         window.StartDrawing();

//         window.ClearColor((GLbitfield)GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, 0.4f, 0.5f, 1.f);

//         camera.ResetVectors(glm::vec3(camera.GetPosition().x, abs(sin(glfwGetTime()/5.f)), 3.f));
//         camera.UpdateVectors();

//         // planeShader.Use();
//         // planeShader.SetUniformMat4("view", camera.GetView());
//         // planeShader.SetUniformMat4("projection", camera.GetProjection());
//         // planeShader.SetUniformMat4("model", plane.model);
//         // plane.Draw();
//         instancedPlaneShader.Use();
//         instancedPlaneShader.SetUniformMat4("view", camera.GetView());
//         instancedPlaneShader.SetUniformMat4("projection", camera.GetProjection());
//         instancedPlaneShader.SetUniformMat4("model", plane.model);
//         plane.DrawInstanced(50);

//         //Draw(Shader& shader, Camera& camera)
//         //DrawInstanced(Shader& shader, Camera& camera, int count)

//         //small problems:
//         //Mesh's model is useless with instances. IS IT? IT'S NOT
//         //Mesh can't (really) go back after setting instances

//         window.TitleFPS();
//         window.EndDrawing();
//     }

//     return 0;
// }