#include "WINDOW.hpp"

std::vector<WindowHint> totalWindowHints;

void SetWindowHint(int hint, int value)
{
    if(totalWindowHints.size() % 5 == 0) 
        totalWindowHints.reserve(totalWindowHints.size()+5); //reserve size for 5 more hints for each 5 hints
    totalWindowHints.emplace_back(hint, value);
}

void Window::glfw_error_callback(int error, const char* description)
{
    std::cerr << "[GLFW] ERROR: " << description << " (code: " << error << ")\n";
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window)); //I need to study this one
    if(win)
    {
        win->windowWidth = width;
        win->windowHeight = height;
    }
}

void Window::glfw_window_maximize_callback(GLFWwindow* window, int maximized) //useless
{
    if(maximized)
        glfwMaximizeWindow(window);
    else
        glfwRestoreWindow(window);
}

void Window::glfw_window_iconify_callback(GLFWwindow* window, int iconified) //useless
{
    if(iconified)
        glfwIconifyWindow(window);
    else 
        glfwRestoreWindow(window);
}

void Window::glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

//I need WindowSetConfig() functions
Window::Window(int width, int height, const char* title)
{
    glfwSetErrorCallback(glfw_error_callback); //main purpose: logging and diagnosing
    try
    {
        if(!glfwInit()) 
        throw std::runtime_error("ERROR::GLFW::FAILED_INITIALIZATION\n");
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        for(int i = 0; i < totalWindowHints.size(); i++)
        {
            glfwWindowHint(totalWindowHints[i].hint, totalWindowHints[i].value);
        }
        std::vector<WindowHint>().swap(totalWindowHints); //"freeing" the hint vector

        for(int i = 0; i <= GLFW_KEY_LAST; i++)
        {
            keyStates[i] = false;
        } // setting all keys as NOT pressed
        
        monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* vidMode = glfwGetVideoMode(monitor);
        monitorWidth = vidMode->width;
        monitorHeight = vidMode->height;

        initialWidth = width;
        initialHeight = height;

        windowWidth = width;
        windowHeight = height;

        window = glfwCreateWindow(width, height, title, NULL, NULL);
        glfwSetWindowUserPointer(window, this);

        std::cout << "Window created: " << Window::GetWidth() << " : " << Window::GetHeight() << '\n';

        if(window == NULL)
        {
            glfwTerminate();
            throw std::runtime_error("ERROR::GLFW::FAILED_WINDOW_CREATION\n");
        }
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetWindowMaximizeCallback(window, glfw_window_maximize_callback);
        glfwSetWindowIconifyCallback(window, glfw_window_iconify_callback);
        glfwSetKeyCallback(window, glfw_key_callback);

        glfwMakeContextCurrent(window);
        
        //loading glad
        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw std::runtime_error("ERROR::GLAD::FAILED_LOADING\n");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        exit(EXIT_FAILURE);
    }

    Window::SetSwapInterval(1);

    frames.delta = 0.f;
    frames.current = glfwGetTime();
    frames.previous = frames.current;

    isFullscreen = false;
    glViewport(0, 0, windowWidth, windowHeight);
}

//Getters
GLFWwindow* Window::GetPointer()
{
    return window;
}

GLFWmonitor* Window::GetMonitor()
{
    return monitor;
}

float Window::GetFrameTime()
{
    return (float)frames.delta;
}

int Window::GetWidth()
{
    return windowWidth;
}

int Window::GetHeight()
{
    return windowHeight;
}

//Setters
void Window::SetSwapInterval(bool interval) //1 or 0. 1 by default.
{
    glfwSwapInterval(interval);
}

void Window::SetAspectRatio(int numer, int denom)
{
    glfwSetWindowAspectRatio(window, numer, denom);
}

void Window::EnableCullFace(GLenum mode)
{
    glEnable(GL_CULL_FACE);
    glCullFace(mode);
}

void Window::EnableCursor()
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::DisableCursor()
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::ToggleFullscreen() //Attention! If window is already fullcreen, the window will resize to the first 2 parameters from constructor
{
    if(!isFullscreen) 
    {
        isFullscreen = !isFullscreen;
        glfwSetWindowMonitor(window, monitor, 0, 0, monitorWidth, monitorHeight, GLFW_DONT_CARE);
    }
    else
    {
        isFullscreen = !isFullscreen;
        glfwSetWindowMonitor(window, NULL, 100, 100, initialWidth, initialHeight, GLFW_DONT_CARE); //well it's kinda hardcoded but whatever
    }
}

//Checkers
bool Window::ShouldClose()
{
    return glfwWindowShouldClose(window);
}

//NOTE: I shouldn't use more functions on a single KEY
bool Window::IsKeyPressed(int GLFW_KEY)
{
    bool isPressed = (glfwGetKey(window, GLFW_KEY) == GLFW_PRESS);
    bool result = (!keyStates[GLFW_KEY] && isPressed);
    keyStates[GLFW_KEY] = isPressed;
    return result;
}

bool Window::IsKeyReleased(int GLFW_KEY)
{
    bool isPressed = (glfwGetKey(window, GLFW_KEY) == GLFW_PRESS);
    bool result = (keyStates[GLFW_KEY] && !isPressed);
    keyStates[GLFW_KEY] = isPressed;
    return result;
}

bool Window::IsKeyDown(int GLFW_KEY)
{
    return (glfwGetKey(window, GLFW_KEY) == GLFW_PRESS);
}

void Window::ShowFPS()
{
    double currentTime = glfwGetTime(); //to set frames once per second
    static double lastTime = 0;

    if((currentTime - lastTime) > 1.f)
    {
        char line[10];
        int len = snprintf(line, 10, "%.1f FPS", 1.f/(float)frames.delta);
        if(len < 10)glfwSetWindowTitle(window, line);
        lastTime = currentTime;
    }
}

//Managers
void Window::ClearBackground(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

    void Window::StartUpdate()
{
    //glClear(GL_COLOR_BUFFER_BIT); // | GL_DEPTH_BUFFER_BIT);
    //HERE WE'LL BIND THE FRAMEBUFFER DRAWING, i guess?
}

void Window::EndUpdate()
{
    glfwSwapBuffers(window);
    frames.current = glfwGetTime();
    frames.delta = frames.current - frames.previous;
    frames.previous = frames.current;
    glfwPollEvents();
}

void Window::Close()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

Window::~Window(){}