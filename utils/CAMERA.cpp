#include "CAMERA.hpp"

void Camera::mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));

    if(cam)
    {
        if(cam->firstMouse)
        {
            cam->lastX = xPos;
            cam->lastY = yPos;
            cam->firstMouse = false;
        }

        float xOffset = xPos - cam->lastX;
        float yOffset = cam->lastY - yPos;

        const float sensitivity = 0.1f;
        xOffset *= sensitivity;
        yOffset *= sensitivity;
        cam->lastX = xPos;
        cam->lastY = yPos;

        cam->yaw += xOffset;
        cam->pitch += yOffset;
    
        if(cam->pitch > 89.f) cam->pitch = 89.f;
        if(cam->pitch < -89.f) cam->pitch = -89.f;
    }
}

Camera::Camera(GLFWwindow* window, glm::vec3 pos, glm::vec3 target, glm::vec3 up)
{
    firstMouse = true;

    cameraPos = pos;
    cameraFront = glm::normalize(target-pos);
    worldUp = up;

    pitch = glm::degrees(asin(cameraFront.y)); //asin(t) = returns how many degrees in t ratio(I think); sin(t) = where t degrees -> reutrns y ratio
    yaw = glm::degrees(atan2(cameraFront.z, cameraFront.x)); //black magic to keep initial target of camera

    speed = 2.5f;
    sensitivity = 0.1f;

    //projection = glm::perspective(glm::radians(45.f), (float)window.GetWidth()/(float)window.GetHeight(), 0.1f, 100.f); //yeah we need a fucking Window member

    glfwSetWindowUserPointer(window, this); //crucial! why? we change *Camera members* inside the callback.
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //Camera::SetCursorPosCallback(window, true);
    glfwSetCursorPosCallback(window, Camera::mouse_callback);
}

void Camera::ResetProjection(PROJ newProj, float newFOV, int windowWidth, int windowHeight, float nearPlane, float farPlane)
{
    if(newProj == PERSPECTIVE_PROJ) projection = glm::perspective(newFOV, (float)windowWidth/(float)windowHeight, nearPlane, farPlane);
    else if (newProj == ORTOGRAPHIC_PROJ) projection = glm::ortho(-1, 1, 1, 1);
}

void Camera::SetSpeed(float newSpeed)
{
    speed = newSpeed;
}

void Camera::SetSensitivity(float newSensitivity)
{
    sensitivity = newSensitivity;
}

void Camera::SetCursorPosCallback(GLFWwindow* window, bool active)
{
    if(active) glfwSetCursorPosCallback(window, Camera::mouse_callback);
    else 
    {
        firstMouse = true; //cool thing! thus we can change this variable and use it IN SetCursorPosCallback(). SO COOL!!!
        glfwSetCursorPosCallback(window, NULL);
    }
}

    void Camera::ChangeControls(int GLFW_KEY_forward, int GLFW_KEY_backward, int GLFW_KEY_left, int GLFW_KEY_right)
{
    inputKeys.FORWARD = GLFW_KEY_forward;
    inputKeys.BACKWARD = GLFW_KEY_backward;
    inputKeys.LEFT = GLFW_KEY_left;
    inputKeys.RIGHT = GLFW_KEY_right;
}

glm::mat4 Camera::GetView()
{
    return glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);
}

glm::mat4 Camera::GetProjection()
{
    return projection;
}

glm::vec3 Camera::GetPosition()
{
    return cameraPos;
}

void Camera::Reset(glm::vec3 newPos, glm::vec3 newTarget)
{
    cameraPos = newPos;
    cameraFront = glm::normalize(newTarget-newPos);
    pitch = glm::degrees(asin(cameraFront.y)); //like in the constructor
    yaw = glm::degrees(atan2(cameraFront.z, cameraFront.x));
}

void Camera::ProcessInput(Window* window)
{
    if(window->IsKeyDown(inputKeys.FORWARD)) cameraPos += cameraFront * speed * window->GetFrameTime();
    if(window->IsKeyDown(inputKeys.BACKWARD)) cameraPos -= cameraFront * speed * window->GetFrameTime();
    if(window->IsKeyDown(inputKeys.LEFT)) cameraPos += cameraRight * speed * window->GetFrameTime();
    if(window->IsKeyDown(inputKeys.RIGHT)) cameraPos -= cameraRight * speed * window->GetFrameTime();
}

void Camera::UpdateVectors()
{
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch)); 

    cameraFront = glm::normalize(direction);
    cameraRight = glm::normalize(glm::cross(cameraFront, worldUp)); //updating these FOR THE INPUT
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}

void Camera::Update(Window* window)
{
    Camera::ProcessInput(window);
    Camera::UpdateVectors();
}
                            
Camera::~Camera(){}