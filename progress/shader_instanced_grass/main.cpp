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

//     Mesh plane = Mesh::createQuadColor(glm::vec3(0.f, 0.5f, 0.f));
//     plane.model = glm::translate(glm::mat4(1.0f), glm::vec3(0.f));
//     // plane.model = glm::scale(plane.model, glm::vec3(3.f, 1.f, 3.f));
//     plane.model = glm::rotate(plane.model, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
//     Shader planeShader(POSITION_COLOR);

//     Mesh grass = Mesh::createGrassBladeColor(glm::vec3(0.2f, 0.9f, 0.2f), glm::vec3(.9f, 0.7f, 0.2f));
//     grass.model = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f));
//     // grass.model = glm::scale(grass.model, glm::vec3(0.1f, 1.f, 0.1f));
//     //Shader grassShader(POSITION_COLOR);
//     Shader grassShader(POSITION_COLOR_INSTANCED);

//     glm::vec3 grassPositions[50];
//     int index = 0;
//     for(int i = 1; i <= 10; i++)
//     {
//         for(int j = 1; j <= 5; j++)
//         {
//             if(index >= 50) break;
//             grassPositions[index++] = glm::vec3(-1.f + i * 0.2f - 0.1f, 0.f, -1.f + j * 0.4f - 0.2f); // 0.2f = 2/10
//         }
//     }
//     // grassPositions[0] = glm::vec3(-1.f, 0.f, -1.f);
//     // grassPositions[1] = glm::vec3(1.f, 0.f, 1.f);
//     // grassPositions[2] = glm::vec3(0.7f, 0.2f, -1.2f);
    

//     grassShader.Use();
//     //InstanceShader::SetInstancePositions
//     for(int i = 0; i < 50;  i++)
//     {
//         grassShader.SetUniformVec3(("positions["+std::to_string(i)+"]").c_str(), grassPositions[i]);
//     }

//     glEnable(GL_DEPTH_TEST); //find a way to blend this with ClearColor()
//     while(!window.ShouldClose())
//     { 
//         window.StartDrawing();

//         window.ClearColor((GLbitfield)GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, 0.4f, 0.5f, 1.f);

//         camera.ResetVectors(glm::vec3(camera.GetPosition().x, abs(sin(glfwGetTime()/5.f)), 3.f));///**/, glm::vec3(0.f, abs(sin(glfwGetTime())), 0.f), glm::vec3(1.f, 0.f, 0.f));
//         camera.UpdateVectors();

//         //grass.model = glm::rotate(grass.model, glm::radians(0.5f), glm::vec3(0.f, 1.f, 0.f));

//         planeShader.Use();
//         planeShader.SetUniformMat4("view", camera.GetView());
//         planeShader.SetUniformMat4("projection", camera.GetProjection());
//         planeShader.SetUniformMat4("model", plane.model);
//         plane.Draw();
//         grassShader.Use();
//         grassShader.SetUniformMat4("view", camera.GetView());
//         grassShader.SetUniformMat4("projection", camera.GetProjection());
//         grassShader.SetUniformMat4("model", grass.model);
//         //grass.DrawInstanced(50);
//         //grass.Draw();
        
//         window.TitleFPS();
//         window.EndDrawing();
//     }

//     return 0;
// }