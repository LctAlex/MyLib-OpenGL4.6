#include "Window.hpp"

void Window::glfw_error_callback(int error, const char *description)
{
    std::cerr << "[GLFW] ERROR: " << description << " (code: " << error << ")\n";
}

Window::Window() //basic start of window initialization
{
    if(!glfwInit())
    {
        std::cerr << "ERROR::GLFW::FAILED_INITIALIZATION\n";
        exit(EXIT_FAILURE);
    } //iniside a bigger class, everything will be inside a try-catch!

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Window::Create(unsigned int width, unsigned int height, const char* title)
{
    this->width = width;
    this->height = height;
    handler = glfwCreateWindow(width, height, title, nullptr, nullptr);
    glfwMakeContextCurrent(handler);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "ERROR::GLAD::FAILED_LOADING\n";
    }

    frames.current = glfwGetTime();
    frames.previous = frames.current;
    frames.delta = 0.f;

    Window::SetVsync(true);

    glfwSetWindowUserPointer(handler, this);
    glfwSetErrorCallback(glfw_error_callback);

    glViewport(0, 0, width , height);
}

//setters
void Window::SetHint(int hint, int value)
{
    glfwWindowHint(hint, value);
}

void Window::SetVsync(bool active)
{
    glfwSwapInterval(active);
}

//getters
GLFWwindow* Window::GetHandler()
{
    return handler;
}

float Window::GetDeltaTime()
{
    return frames.delta;
}

unsigned int Window::GetWidth()
{
    return width;
}

unsigned int Window::GetHeight()
{
    return height;
}

//checkers
bool Window::ShouldClose()
{
    return glfwWindowShouldClose(handler);
}


//managers
void Window::ClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}
// Usage example: GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT
void Window::ClearColor(unsigned int buffers, float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(buffers);
}

void Window::StartDrawing()
{
    
}

void Window::EndDrawing()
{
    frames.current = glfwGetTime();
    frames.delta = frames.current - frames.previous;
    frames.previous = frames.current;
    glfwPollEvents();
    glfwSwapBuffers(handler);
}

Window::~Window()
{
    if (handler)
    {
        glfwDestroyWindow(handler);
    }
    glfwTerminate();
}

void Window::TitleFPS()
{
    double currentTime = glfwGetTime(); // to set frames once per second
    static double lastTime = 0;

    if ((currentTime - lastTime) > 1.f)
    {
        char line[10];
        int len = snprintf(line, 10, "%.1f FPS", 1.f / (float)frames.delta);
        if (len < 10)
            glfwSetWindowTitle(handler, line);
        lastTime = currentTime;
    }
}