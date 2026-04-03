#pragma once
#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

#define UNRESIZABLE_WINDOW GLFW_RESIZABLE, GLFW_FALSE
#define WINDOWED_FULLSCREEN GLFW_MAXIMIZED, GLFW_TRUE //could find a better name
void SetWindowHint(int hint, int value);//to be called before constructing window! NEEDS glfwInit() before any hint...fuck
struct WindowHint
{
    int hint;
    int value;
    WindowHint(int h, int v):hint(h),value(v){}
};

class Window //designed for ONE single window
{
    private:
    static void glfw_error_callback(int error, const char* description); //encapsulating so nothing can use it outside the class, ONE WINDOW
    static void framebuffer_size_callback(GLFWwindow* window, int wodth, int height);
    static void glfw_window_maximize_callback(GLFWwindow* window, int maximized); //alright so this and window_iconify are kinda useless...That's all
    static void glfw_window_iconify_callback(GLFWwindow* window, int iconified);
    static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods); //automatic ESCAPE key checking
    GLFWwindow* window;
    GLFWmonitor* monitor; //I mean do I really need a monitor variable? I'm interested in it's size only, I think
    int monitorWidth, monitorHeight;
    int initialWidth, initialHeight;
    int windowWidth, windowHeight;
    bool isFullscreen;

    typedef struct //I should use raylib's implementation
    {
        double current;
        double previous;
        double delta;
    }Frames;
    Frames frames;

    bool keyStates[GLFW_KEY_LAST + 1]; //vector to keep track of all key states. 349 is a big number but it's for simplicity
    //int the future, I could define every single available key (I have) and allocate just necessary space to keyStates

    public:
    Window(int width, int height, const char* title); //AspectRatio doesn't work with !resizable
    
    //Getters
    GLFWwindow* GetPointer();
    GLFWmonitor* GetMonitor();
    float GetFrameTime(); //we'll cast it to float at return
    int GetWidth();
    int GetHeight();

    //Setters
    void SetSwapInterval(bool interval);
    void SetAspectRatio(int numer, int denom); //essentially does nothing in FULLSCREEN
    void SetClearColor(float r = 0.f, float g = 0.f, float b = 0.f, float a = 1.f);
    void EnableCullFace(GLenum mode);
    void EnableCursor();
    void DisableCursor();
    void ToggleFullscreen();

    //Checkers
    bool ShouldClose();
    bool IsKeyPressed(int GLFW_KEY);
    bool IsKeyReleased(int GLFW_KEY);
    bool IsKeyDown(int GLFW_KEY);
    void ShowFPS();

    //Managers
    void StartUpdate(); //should make 2D for color only & 3D for color AND depth
    void EndUpdate();
    void Close();
    ~Window();
};

//