#include "SYSTEM.hpp"

System::WindowHint::WindowHint(int h, int v):hint(h), value(v){}

void System::InitGLFW()
{
    if (!glfwInit())
        throw std::runtime_error("ERROR::GLFW::FAILED_INITIALIZATION\n");
}
void System::ActivateWindowHints()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}
void System::ActivateUserHints()
{
    for (int i = 0; i < userWindowHints.size(); i++)
    {
        glfwWindowHint(userWindowHints[i].hint, userWindowHints[i].value);
    }
}
void System::ClearUserHints()
{
    std::vector<WindowHint>().swap(userWindowHints);
}


//Constructor
System::System()
{
    static int systemCount = 0;
    if (systemCount != 0)
    {
        std::cout << "ERROR::SYSTEM::TOO_MANY_INITIALIZATIONS\n";
        exit(EXIT_FAILURE);
    }

    systemCount++;
    window = NULL;
    camera = NULL;
    // construct *input;
}


//Setters
void System::SetWindowHint(int hint, int value)
{
    if (userWindowHints.size() % 5 == 0)
        userWindowHints.reserve(userWindowHints.size() + 5); // reserve size for 5 more hints for each 5 hints
    userWindowHints.emplace_back(hint, value);
}


//Managers
void System::CreateWindow(int width, int height, const char *title)
{
    try
    {
        System::InitGLFW();
        System::ActivateWindowHints();
        System::ActivateUserHints();
        System::ClearUserHints();
        window = new Window(width, height, title);
        glfwSetWindowUserPointer(window->GetPointer(), this);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        exit(EXIT_FAILURE);
    }
}

void System::CreateCamera(glm::vec3 pos, glm::vec3 target, glm::vec3 up)
{
    if(window == NULL)
    {
        std::cout << "ERROR::SYSTEM::CAMERA::NO_WINDOW_TO_ASSIGN_TO\n";
        exit(EXIT_FAILURE);
    }
    camera = new Camera(window->GetPointer(), pos, target, up);
}

void System::Close()
{
    window->Close();
    delete window;
    delete camera;
}

System::~System(){}