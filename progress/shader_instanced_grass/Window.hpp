#pragma once
#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"

#include <iostream>

#define HINT_UNRESIZABLE_WINDOW GLFW_RESIZABLE, GLFW_FALSE

class Window
{
    private:
    static void glfw_error_callback(int error, const char* description);

    unsigned int width, height;
    struct {float previous, current, delta;} frames;

    GLFWwindow *handler;

public:
    Window();
    void Create(unsigned int width, unsigned int height, const char* title);

    //setters
    void SetHint(int hint, int value);
    void SetVsync(bool active);

    //getters
    GLFWwindow *GetHandler();
    float GetDeltaTime();
    unsigned int GetWidth();
    unsigned int GetHeight();

    //checkers
    bool ShouldClose();

    //managers
    void ClearColor(float r = .5f, float g = .5f, float b = .5f, float a = 1.f);
    void ClearColor(GLbitfield buffers = GL_COLOR_BUFFER_BIT, float r = .5f, float g = .5f, float b = .5f, float a = 1.f);
    void StartDrawing();
    void EndDrawing();

    ~Window();

    //uncertain
    void TitleFPS();
};