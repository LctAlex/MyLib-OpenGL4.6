#pragma once
#include "../include/GLFW/glfw3.h"

#define GLFW_KEY_FIRST GLFW_KEY_SPACE
//{#define KEY_0 GLFW_KEY_0 ...}

class Input
{
    private:
    GLFWwindow* window; //window handler
    static void glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    bool keyStates[GLFW_KEY_LAST - GLFW_KEY_FIRST + 1]; // 348 - 32 = 316
    bool mouseButtonStates[3]; //3 = LEFT, RIGHT, MIDDLE (I'll add the rest later) This could lowkey be a byte
    void InitKeys();
    void InitMouseButtons();

    public:
    Input(GLFWwindow* window);
    //void SetListener(/*lambda?*/);

    //should these be static??? (so no Input initialization is needed)

    bool IsKeyPressed(int KEY);
    bool IsKeyReleased(int KEY);
    bool IsKeyDown(int KEY);
    bool IsKeyUp(int KEY);

    bool IsMouseButtonPressed(int MOUSE_BUTTON);
    bool IsMouseButtonReleased(int MOUSE_BUTTON);
    bool IsMouseButtonDown(int MOUSE_BUTTON);
    bool IsMouseButtonUp(int MOUSE_BUTTON);

    ~Input();
};
