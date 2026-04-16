#pragma once
#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

//why start from 0->348 if first GLFW_KEY is 32? Goes into Input class
#define GLFW_KEY_FIRST 32

class Window
{
    private:
    //callbacks cannot be non-static!
    static void glfw_error_callback(int error, const char* description); //encapsulating so nothing can use it outside the class, ONE WINDOW
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void window_size_callback(GLFWwindow* window, int width, int height);
    static void glfw_window_maximize_callback(GLFWwindow* window, int maximized); //alright so this and window_iconify are kinda useless...That's all
    static void glfw_window_iconify_callback(GLFWwindow* window, int iconified);
    static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods); //automatic ESCAPE key checking
    GLFWwindow* window;
    GLFWmonitor* monitor; //I mean do I really need a monitor variable? not really
    int monitorWidth, monitorHeight;
    int initialWidth, initialHeight;
    int windowWidth, windowHeight;
    bool isFullscreen;

    //Framebuffer. I'll have a bet with myself: this will cause trouble in the future
    unsigned int FBO, RBO, TexColorbuffer;
    unsigned int resolutionWidth, resolutionHeight; //by default, will be equal to window size
    unsigned int canvasVAO, canvasVBO, canvasEBO; //maybe "canvas" is not a good name
    unsigned int canvasShader; //god damnit, I would become dependent of one of my classes if I use Shader class. I don't want that (not now)
    void InitFramebufferVAO(); //this could be avoided
    void InitFramebufferShader(); //this could be avoided
    void InitFramebuffer();
    void BindFramebuffer(); //inside StartDrawing();
    void UnbindFramebuffer(); //inside EndDrawing();
    void DrawFramebuffer(); //MUST be paired with UnbindFramebuffer()
    //void ResizeFramebuffer(int width, int height);
    void DeleteFramebuffer(); //will go inside Window::Close()

    typedef struct
    {
        double current;
        double previous;
        double delta;
    }Frames;
    Frames frames;

    void InitKeys();
    void InitButtons();
    bool keyStates[GLFW_KEY_LAST + 1 - GLFW_KEY_FIRST]; //vector to keep track of all key states. 349 is a big number but it's for simplicity
    bool buttonStates[4]; //mouse left, mouse right, mouse middle, mouse middle. GLFW_MOUSE_BUTTON_1 = 0 = GLFW_MOUSE_BUTTON_LEFT = GLFW_MOUSE_BUTTON_FIRST

    public : 
    Window(int width, int height, const char *title); // AspectRatio doesn't work with !resizable

    //Getters
    GLFWwindow* GetPointer();
    GLFWmonitor* GetMonitor();
    float GetFrameTime(); //we'll cast it to float at return
    int GetWidth();
    int GetHeight();

    //Setters
    void SetSwapInterval(bool interval);
    void SetAspectRatio(int numer, int denom); //essentially does nothing in FULLSCREEN
    void SetResolution(int xRes, int yRes); //by default: resolution = window size. CAN'T BE MORE
    void SetBlendingMode(GLint param);//GL_LINEAR (smooth) or GL_NEAREST (sharp)
    void EnableCullFace(GLenum mode);
    void EnableCursor();
    void DisableCursor();
    void ToggleFullscreen();

    //Checkers
    bool ShouldClose();
    bool IsKeyPressed(int GLFW_KEY); //key presses should be a separate thing, inside like a System class or something
    bool IsKeyReleased(int GLFW_KEY);
    bool IsKeyDown(int GLFW_KEY);
    bool IsMouseButtonPressed(int GLFW_MOUSE_BUTTON);
    bool IsMouseButtonReleased(int GLFW_MOUSE_BUTTON);
    bool IsMouseButtonDown(int GLFW_MOUSE_BUTTON);
    void ShowFPS();

    //Managers
    void ClearColor(float r = 0.5f, float g = 0.5f, float b = 0.5f, float a = 1.0f);
    void ClearColorDepth(float r = 0.5f, float g = 0.5f, float b = 0.5f, float a = 1.0f);
    void StartUpdate(); //should make 2D for color only & 3D for color AND depth
    void EndUpdate();
    void Close();
    ~Window();

    //testing
    int GetResX();
    int GetResY();
};