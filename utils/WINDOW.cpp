#include "WINDOW.hpp"

std::vector<WindowHint> totalWindowHints; //THIS IS GLOBAL! MAKE IT A MEMBER! could go inside the System class! 

void SetWindowHint(int hint, int value)
{
    if(totalWindowHints.size() % 5 == 0) 
    totalWindowHints.reserve(totalWindowHints.size()+5); //reserve size for 5 more hints for each 5 hints
    totalWindowHints.emplace_back(hint, value);
}

void ActivateWindowHints(std::vector<WindowHint> &windowHintsVec) // will be parameterless inside System
{
    for (int i = 0; i < windowHintsVec.size(); i++)
    {
        glfwWindowHint(windowHintsVec[i].hint, windowHintsVec[i].value);
    }
}

void ClearWindowHints(std::vector<WindowHint>& windowHintsVec) //will be parameterless inside System
{
    std::vector<WindowHint>().swap(windowHintsVec);
}

//callbacks
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
        //this doesn't update in out main class
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

//framebuffer
void Window::InitFramebufferVAO()
{
    float canvasVertices[] = {
        -1.f, -1.f,     0.f, 0.f,
        1.f, 1.f,       1.f, 1.f,
        -1.f, 1.f,      0.f, 1.f,
        1.f, -1.f,      1.f, 0.f
    };
    unsigned int canvasIndices[] = {
        0, 1, 2,
        0, 3, 1
    };

    glGenVertexArrays(1, &canvasVAO); //this fucker must be bound BEFORE VBO/EBO
    glBindVertexArray(canvasVAO);
    
    glGenBuffers(1, &canvasVBO);
    glBindBuffer(GL_ARRAY_BUFFER, canvasVBO);
    glBufferData(GL_ARRAY_BUFFER, 16*sizeof(float), canvasVertices, GL_STATIC_DRAW);
    
    glGenBuffers(1, &canvasEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, canvasEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(unsigned int), canvasIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(0));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Window::InitFramebufferShader()
{
    unsigned int vShader, fShader;
    const char* vSourceCode;
    const char* fSourceCode;

    bool foundShaders = true; //obviously this will rely on a Shader class' program compilation
    if(foundShaders) //just for testing
    {
        vSourceCode = "#version 460 core\n"
                      "layout (location = 0) in vec2 aPos;\n"
                      "layout(location = 1) in vec2 aTexCoords;\n"
                      "out vec2 TexCoords;\n"
                      "void main()\n"
                      "{\n"
                      "gl_Position = vec4(aPos.xy, 0.0, 1.0);\n"
                      "TexCoords = aTexCoords;\n"
                      "}\0";

        fSourceCode = "#version 460 core\n"
                      "out vec4 FragColor;\n"
                      "in vec2 TexCoords;\n"
                      "uniform sampler2D screenTex;\n"
                      "const float offset = 1.0/300.0;\n"
                      "void main()\n"
                      "{\n"
                      "vec2 offsets[9] = vec2[](\n"
                      "vec2(-offset,  offset),\n"
                      "vec2( 0.0f,    offset),\n"
                      "vec2( offset,  offset),\n"
                      "vec2(-offset,  0.0f),\n"
                      "vec2( 0.0f,    0.0f),\n"
                      "vec2( offset,  0.0f),\n"
                      "vec2(-offset, -offset),\n"
                      "vec2( 0.0f,   -offset),\n"
                      "vec2( offset, -offset));\n"

                      //"float kernel[9] = float[](1, 1, 1, 1, -7, 1, 1, 1, 1);\n"
                      //"float kernel[9] = float[](-1, -1, -1, -1, 9, -1, -1, -1, -1);\n"
                      //"float kernel[9] = float[](-1, -1, -1, -1, 8, -1, -1, -1, -1);\n"
                      "float kernel[9] = float[](-1, 1, -1, 1, 1, 1, -1, 1, -1);\n"

                      "vec3 sampleTex[9];\n"
                      "for(int i = 0; i < 9; i++) {sampleTex[i] = vec3(texture(screenTex, TexCoords.st + offsets[i]));}\n"

                      "vec3 color = vec3(0.0);\n"
                      "for(int i = 0; i < 9; i++) {color += sampleTex[i] * kernel[i];}\n"

                      "FragColor = vec4(color, 1.0);\n"
                      "}\n";
    } //more like this will happen inside try{} and else inside catch{}
    else //hardcoded source codes
    {
        vSourceCode = "#version 460 core\n"
                      "layout (location = 0) in vec2 aPos;\n"
                      "layout(location = 1) in vec2 aTexCoords;\n"
                      "out vec2 TexCoords;\n"
                      "void main()\n"
                      "{\n"
                      "gl_Position = vec4(aPos.xy, 0.0, 1.0);\n"
                      "TexCoords = aTexCoords;\n"
                      "}\0";
        fSourceCode = "#version 460 core\n"
                      "out vec4 FragColor;\n"
                      "in vec2 TexCoords;\n"
                      "uniform sampler2D screenTex;\n"
                      "void main()\n"
                      "{\n"
                      "FragColor = texture(screenTex, TexCoords);\n"
                    //   "vec4 color = texture(screenTex, TexCoords);\n"
                    //   "float average = (color.r*5 + color.g + color.b) / 7.0;\n"
                    //   "FragColor = vec4(vec3(average), 1.0);\n"
                      "}\n";
    }

    try
    {
        int success = 1;
        char infoLog[256];

        vShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vShader, 1, &vSourceCode, NULL);
        glCompileShader(vShader);
        glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vShader, 256, NULL, infoLog);
            std::cout << infoLog;
            throw std::runtime_error("ERROR::FRAMEBUFFER::SHADER::VERTEX::COMPILATION_ERROR\n");
        }

        fShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fShader, 1, &fSourceCode, NULL);
        glCompileShader(fShader);
        glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fShader, 256, NULL, infoLog);
            std::cout << infoLog;
            throw std::runtime_error("ERROR::FRAMEBUFFER::SHADER::FRAGMENT::COMPILATION_ERROR\n");
        }

        canvasShader = glCreateProgram();
        glAttachShader(canvasShader, vShader);
        glAttachShader(canvasShader, fShader);
        glLinkProgram(canvasShader);
        glGetProgramiv(canvasShader, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(canvasShader, 256, NULL, infoLog);
            std::cout << infoLog;
            throw std::runtime_error("ERROR::FRAMEBUFFER::SHADER::LINKING_ERROR");
        }
    }
    catch(std::exception &e)
    {
        std::cerr << e.what();
        exit(EXIT_FAILURE);
    }

    glDeleteShader(fShader);
    glDeleteShader(vShader);
}

void Window::InitFramebuffer()
{
    Window::InitFramebufferVAO();
    Window::InitFramebufferShader();
    resolutionWidth = windowWidth;
    resolutionHeight = windowHeight;
    try
    {
        glGenFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        glGenTextures(1, &TexColorbuffer);
        glBindTexture(GL_TEXTURE_2D, TexColorbuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, resolutionWidth, resolutionHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TexColorbuffer, 0);

        glGenRenderbuffers(1, &RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, resolutionWidth, resolutionHeight);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            throw std::runtime_error("ERROR::FRAMEBUFFER::INCOMPLETE");
    }
    catch(std::exception &e)
    {
        std::cerr << e.what();
        exit(EXIT_FAILURE);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Window::BindFramebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, resolutionWidth, resolutionHeight);
}

void Window::UnbindFramebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glViewport(0, 0, windowWidth, windowHeight);
    Window::ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Window::DrawFramebuffer()
{
    glUseProgram(canvasShader);
    glUniform1i(glGetUniformLocation(canvasShader, "screenTex"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TexColorbuffer);
    glBindVertexArray(canvasVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Window::DeleteFramebuffer()
{
    glDeleteBuffers(1, &canvasVBO);
    glDeleteBuffers(1, &canvasEBO);
    glDeleteVertexArrays(1, &canvasVAO);
    glDeleteProgram(canvasShader);
    glDeleteRenderbuffers(1, &RBO);
}

//input
void Window::InitKeys()
{
    for (int i = GLFW_KEY_FIRST; i <= GLFW_KEY_LAST; i++)
    {
        keyStates[i - GLFW_KEY_FIRST] = false;
    } // setting all keys as NOT pressed
}

void Window::InitButtons()
{
    for (int i = 0; i < 4; i++)
    {
        buttonStates[i] = false;
    }
}

// constructor
Window::Window(int width, int height, const char *title)
{
    glfwSetErrorCallback(glfw_error_callback); //main purpose: logging and diagnosing
    try
    {
        if(!glfwInit()) 
        throw std::runtime_error("ERROR::GLFW::FAILED_INITIALIZATION\n");
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        ActivateWindowHints(totalWindowHints);
        ClearWindowHints(totalWindowHints); //"freeing" the hint vector

            // setting up button/mouse input info
            Window::InitKeys();
        Window::InitButtons();
        
        monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* vidMode = glfwGetVideoMode(monitor);
        monitorWidth = vidMode->width;
        monitorHeight = vidMode->height;

        initialWidth = width;
        initialHeight = height;

        windowWidth = width;
        windowHeight = height;

        window = glfwCreateWindow(width, height, title, NULL, NULL);
        
        std::cout << "Window created: " << Window::GetWidth() << " : " << Window::GetHeight() << '\n';
        
        if(window == NULL)
        {
            glfwTerminate();
            throw std::runtime_error("ERROR::GLFW::FAILED_WINDOW_CREATION\n");
        }
        glfwSetWindowUserPointer(window, this); //in order to use glfwGetWindowUserPointer() elsewhere
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

    Window::InitFramebuffer();
    Window::SetSwapInterval(1);

    frames.delta = 0.f;
    frames.current = glfwGetTime();
    frames.previous = frames.current;

    isFullscreen = false;

    glClearColor(0.f, 0.f, 0.f, 1.f); //default Color Clearing
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

float Window::GetFrameTime() //GetDeltaTime
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

void Window::SetResolution(int xRes, int yRes)
{
    if(windowWidth < xRes || windowHeight < yRes || xRes <= 0 || yRes <= 0)
    {
        std::cout << "Resolution (" << xRes << ':' << yRes
                << ") is incompatible with window size (" << windowWidth << ':' << windowHeight << ")\n";
        return; 
    }
    resolutionWidth = xRes;
    resolutionHeight = yRes;
    try
    {
        //resetting texture's resolution
        glBindTexture(GL_TEXTURE_2D, TexColorbuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, resolutionWidth, resolutionHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TexColorbuffer, 0);
    
        //resetting renderbuffer's resolution
        glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, resolutionWidth, resolutionHeight);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            throw std::runtime_error("ERROR::FRAMEBUFFER::INCOMPLETE_RESOLUTION");
    }
    catch (std::exception &e)
    {
        std::cerr << e.what();
        exit(EXIT_FAILURE);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Window::SetBlendingMode(GLint param)
{
    glBindTexture(GL_TEXTURE_2D, TexColorbuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TexColorbuffer, 0);
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
    bool result = (!keyStates[GLFW_KEY - GLFW_KEY_FIRST] && isPressed);
    keyStates[GLFW_KEY - GLFW_KEY_FIRST] = isPressed;
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

bool Window::IsMouseButtonPressed(int GLFW_MOUSE_BUTTON)
{
    bool isPressed = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON) == GLFW_PRESS);
    bool result = (!buttonStates[GLFW_MOUSE_BUTTON] && isPressed);
    buttonStates[GLFW_MOUSE_BUTTON] = isPressed;
    return result;
}

bool Window::IsMouseButtonReleased(int GLFW_MOUSE_BUTTON)
{
    bool isPressed = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON) == GLFW_PRESS);
    bool result = (buttonStates[GLFW_MOUSE_BUTTON] && !isPressed);
    buttonStates[GLFW_MOUSE_BUTTON] = isPressed;
    return result;
}

bool Window::IsMouseButtonDown(int GLFW_MOUSE_BUTTON)
{
    return (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON));
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
void Window::ClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::ClearColorDepth(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void Window::StartUpdate()
{
    Window::BindFramebuffer();
}

void Window::EndUpdate()
{
    Window::UnbindFramebuffer();
    Window::DrawFramebuffer();
    glfwSwapBuffers(window);
    frames.current = glfwGetTime();
    frames.delta = frames.current - frames.previous;
    frames.previous = frames.current;
    glfwPollEvents();
}

void Window::Close()
{
    glfwDestroyWindow(window);
    Window::DeleteFramebuffer();
    glfwTerminate();
}

Window::~Window(){}

//testing
int Window::GetResX()
{
    return resolutionWidth;
}
int Window::GetResY()
{
    return resolutionHeight;
}