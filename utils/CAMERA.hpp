#pragma once
#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"

#include "WINDOW.hpp"

#include <iostream>

class Camera
{
    private:
    static void mouse_callback(GLFWwindow* window, double xPos, double yPos);
    struct KEYS{ int FORWARD = GLFW_KEY_W; int BACKWARD = GLFW_KEY_S; int LEFT = GLFW_KEY_D; int RIGHT = GLFW_KEY_A;};
    enum PROJ{PERSPECTIVE_PROJ, ORTOGRAPHIC_PROJ};
    bool firstMouse;
    float lastX, lastY;
    float yaw, pitch;
    glm::mat4 view, projection;
    glm::vec3 cameraPos, cameraFront, worldUp;
    glm::vec3 cameraRight, cameraUp;
    //variables:
    float speed;
    float sensitivity;
    float zoom; //thinking about this
    KEYS inputKeys;

public:
    Camera(GLFWwindow* window, glm::vec3 pos = glm::vec3(0.f, 0.f, 3.f), glm::vec3 target = glm::vec3(0.f), glm::vec3 up = glm::vec3(0.f, 1.f, 0.f));

    //Setters
    void ResetProjection(PROJ newProj, float newFOV = 45.f, int windowWidth = 1, int windowHeight = 1, float nearPlane = 0.1f, float farPlane = 100.f);
    //void SetPerspProj()
    //void SetOrthoProj()
    void SetSpeed(float newSpeed);
    void SetSensitivity(float newSensitivity);

    void ToggleScrolling(float limit); //this will need a new callback...I need to implement projection here as well

    void SetCursorPosCallback(GLFWwindow* window, bool active); //another reason to try and have GLFWwindow* as a variable
    void ChangeControls(int GLFW_KEY_forward, int GLFW_KEY_backward, int GLFW_KEY_left, int GLFW_KEY_right); //NOT the most necesarry function, but it's fun

    //Getters
    glm::mat4 GetView(); //view needs projection too
    glm::mat4 GetProjection();
    glm::vec3 GetPosition();

    //Managers
    void Reset(glm::vec3 newPos = glm::vec3(0.f, 0.f, 3.f), glm::vec3 newTarget = glm::vec3(0.f));
    void ProcessInput(Window* window);
    void UpdateVectors();
    void Update(Window* window); //has UpdateVectors and ProcessInput, atp we should have a window pointer as member
    ~Camera();
};