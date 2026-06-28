#pragma once
#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"

#include "Input.hpp"

#include <iostream>

//NOTE: Don't forget to UpdateVectors()
class Camera
{
    private:
    bool startMouse;
    float lastX, lastY;
    float yaw, pitch;
    glm::mat4 view, projection;
    glm::vec3 cameraPos, cameraFront, worldUp;
    glm::vec3 cameraRight, cameraUp;
    // variables:
    float speed;
    float sensitivity;
    float zoom;
    struct {int forward = GLFW_KEY_W; int backward = GLFW_KEY_S; int left = GLFW_KEY_A; int right = GLFW_KEY_D;} movementKeys;
    
    public:
    static void glfw_mouse_callback(GLFWwindow *window, double xPos, double yPos);
    //static void glfw_scroll_callback();
    Camera(glm::vec3 pos = glm::vec3(0.f, 0.f, 3.f), glm::vec3 target = glm::vec3(0.f), glm::vec3 up = glm::vec3(0.f, 1.f, 0.f));

    //setters
    void SetSpeed(float newSpeed);
    void SetSensitivity(float newSensitivity);
    void SetProjectionToOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
    void SetProjectionToPersp(float fovy, int width, int height, float near, float far);

    //getters
    glm::mat4 GetView();
    glm::mat4 GetProjection();
    glm::vec3 GetPosition();

    //managers
    void ResetVectors(glm::vec3 newPos = glm::vec3(0.f, 0.f, 3.f), glm::vec3 newTarget = glm::vec3(0.f), glm::vec3 newUp = glm::vec3(0.f, 1.f, 0.f));
    void UpdateVectors(); //goes inside callback? Not necesarily, perhaps we want camera updates even when we don't move it
    void ProcessMovement(Input& input, float deltaTime = 1.0f);
    void Update(Input& input);
    ~Camera();
};