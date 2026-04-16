#pragma once
#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

#include "WINDOW.hpp"
#include "CAMERA.hpp"

#include <vector>

#define HINT_UNRESIZABLE_WINDOW GLFW_RESIZABLE, GLFW_FALSE
#define HINT_WINDOWED_FULLSCREEN GLFW_MAXIMIZED, GLFW_TRUE

class System
{
    private:
    struct WindowHint
    {
        int hint;
        int value;
        WindowHint(int h, int v);
    };
    std::vector<WindowHint> userWindowHints;
    void InitGLFW();
    void ActivateWindowHints();
    void ActivateUserHints();
    void ClearUserHints();
    
    public : 
    //members
    Window *window;
    Camera* camera;
    
    //Constructor
    System();

    //Setters
    void SetWindowHint(int hint, int value);
    
    //Managers
    void CreateWindow(int width, int height, const char* title);
    void CreateCamera(glm::vec3 pos = glm::vec3(0.f, 0.f, 3.f), glm::vec3 target = glm::vec3(0.f), glm::vec3 up = glm::vec3(0.f, 1.f, 0.f));
    
    void Close();

    ~System();
};