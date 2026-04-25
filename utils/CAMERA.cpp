#include "Camera.hpp"

Camera::Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 up)
{
    startMouse = true;

    cameraPos = pos;
    cameraFront = glm::normalize(target - pos);
    worldUp = up;

    pitch = glm::degrees(asin(cameraFront.y));
    yaw = glm::degrees(atan2(cameraFront.z, cameraFront.x));

    speed = 2.5f;
    sensitivity = 0.1f;

    projection = glm::ortho(-1.f, 1.f, -1.f, 1.f);
}

void Camera::SetSpeed(float newSpeed)
{
    speed = newSpeed;
}

void Camera::SetSensitivity(float newSensitivity)
{
    sensitivity = newSensitivity;
}

void Camera::SetProjectionToOrtho(float left, float right, float bottom, float top)
{
    projection = glm::ortho(left, right, bottom, top);
}

void Camera::SetProjectionToPersp(float fovy, int width, int height, float near, float far)
{
    projection = glm::perspective(glm::radians(fovy), float(width)/(float)(height), near, far);
}

glm::mat4 Camera::GetView()
{
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

glm::mat4 Camera::GetProjection()
{
    return projection;
}

glm::vec3 Camera::GetPosition()
{
    return cameraPos;
}

void Camera::ResetVectors(glm::vec3 newPos, glm::vec3 newTarget, glm::vec3 newUp)
{
    cameraPos = newPos;
    cameraFront = glm::normalize(newPos-newTarget);
    cameraUp = newUp;
}

void Camera::UpdateVectors()
{
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));

    cameraFront = glm::normalize(direction);
    cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}

Camera::~Camera(){};