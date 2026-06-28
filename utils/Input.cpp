#include "Input.hpp"

void Input::glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE) 
        glfwSetWindowShouldClose(window, true);//don't know how safe this is but yeah
}

void Input::InitKeys()
{
    for (int i = GLFW_KEY_FIRST; i <= GLFW_KEY_LAST; i++)
    {
        keyStates[i - GLFW_KEY_FIRST] = false;
    }
}

void Input::InitMouseButtons()
{
    for(int i = 0; i < 3; i++)
    {
        mouseButtonStates[i] = false;
    }
}

Input::Input(GLFWwindow *window):window(window)
{
    InitKeys();
    InitMouseButtons();
    glfwSetKeyCallback(window, glfw_key_callback);
}

bool Input::IsKeyPressed(int KEY)
{
    int keyIndex = KEY - GLFW_KEY_FIRST;
    bool isPressed = (glfwGetKey(window, KEY) == GLFW_PRESS);
    bool result = (!keyStates[keyIndex] && isPressed);
    keyStates[keyIndex] = isPressed;
    return result;
}
bool Input::IsKeyReleased(int KEY)
{
    int keyIndex = KEY - GLFW_KEY_FIRST;
    bool isPressed = (glfwGetKey(window, KEY) == GLFW_PRESS);
    bool result = (keyStates[keyIndex] && !isPressed);
    keyStates[keyIndex] = isPressed;
    return result;
}
bool Input::IsKeyDown(int KEY)
{
    return (glfwGetKey(window, KEY) == GLFW_PRESS);
}
bool Input::IsKeyUp(int KEY)
{
    return (glfwGetKey(window, KEY) == GLFW_RELEASE);
}

bool Input::IsMouseButtonPressed(int MOUSE_BUTTON)
{
    bool isPressed = (glfwGetMouseButton(window, MOUSE_BUTTON) == GLFW_PRESS);
    bool result = (!mouseButtonStates[MOUSE_BUTTON] && isPressed);
    mouseButtonStates[MOUSE_BUTTON] = isPressed;
    return result;
}
bool Input::IsMouseButtonReleased(int MOUSE_BUTTON)
{
    bool isPressed = (glfwGetMouseButton(window, MOUSE_BUTTON) == GLFW_PRESS);
    bool result = (mouseButtonStates[MOUSE_BUTTON] && !isPressed);
    mouseButtonStates[MOUSE_BUTTON] = isPressed;
    return result;
}
bool Input::IsMouseButtonDown(int MOUSE_BUTTON)
{
    return (glfwGetMouseButton(window, MOUSE_BUTTON) == GLFW_PRESS);
}
bool Input::IsMouseButtonUp(int MOUSE_BUTTON)
{
    return (glfwGetMouseButton(window, MOUSE_BUTTON) == GLFW_RELEASE);
}

Input::~Input(){/*nothing really to clear, eh?*/}