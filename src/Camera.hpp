#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "InputManager.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <functional>
#include <iostream>

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  3000.f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


class Camera
{
public:

    // constructor with vectors
    Camera(GLFWwindow* window, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f))
    , MovementSpeed(SPEED)
    , MouseSensitivity(SENSITIVITY)
    , Zoom(ZOOM)
    , m_window(window)
    , m_lastMousePosition({0, 0})
    , m_mouseGrabbed(false)
    {
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        m_lastMousePosition.x = static_cast<float>(mouseX);
        m_lastMousePosition.y = static_cast<float>(mouseY);

        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // constructor with scalar values
    Camera(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f))
    , MovementSpeed(SPEED)
    , MouseSensitivity(SENSITIVITY)
    , Zoom(ZOOM)
    , m_window(window)
    , m_lastMousePosition({0, 0})
    , m_mouseGrabbed(false)
    {
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        m_lastMousePosition.x = static_cast<float>(mouseX);
        m_lastMousePosition.y = static_cast<float>(mouseY);

        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();

        InputManager::addKeyBinding(GLFW_KEY_ESCAPE, std::bind(&Camera::toggleMouseGrab, this, std::placeholders::_1, std::placeholders::_2));
    }

    GLFWwindow* getWindow() const
    {
        return m_window;
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void move(int key, int action)
    {
        float velocity = MovementSpeed * deltaTime;
        if ((action == GLFW_PRESS || action == GLFW_REPEAT) && key == GLFW_KEY_W)
            Position += Front * velocity;
        if ((action == GLFW_PRESS || action == GLFW_REPEAT) && key == GLFW_KEY_S)
            Position -= Front * velocity;
        if ((action == GLFW_PRESS || action == GLFW_REPEAT) && key == GLFW_KEY_A)
            Position -= Right * velocity;
        if ((action == GLFW_PRESS || action == GLFW_REPEAT) && key == GLFW_KEY_D)
            Position += Right * velocity;
    }

    void toggleMouseGrab(int, int action)
    {
        if (action == GLFW_PRESS)
        {
            m_mouseGrabbed = !m_mouseGrabbed;

            glfwSetInputMode(m_window, GLFW_CURSOR, (m_mouseGrabbed ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL));
        }
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void onMouseMove(float xpos, float ypos)
    {
        float mouseDeltaX = xpos - m_lastMousePosition.x;
        float mouseDeltaY = ypos - m_lastMousePosition.y;
        m_lastMousePosition.x = xpos;
        m_lastMousePosition.y = ypos;

        if (!m_mouseGrabbed)
            return;

        Yaw += mouseDeltaX * MouseSensitivity;
        Pitch += -mouseDeltaY * MouseSensitivity;

        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    void rotateHoriz(float delta)
    {
        Yaw += delta;
        updateCameraVectors();
    }

    void rotateVert(float delta)
    {
        Pitch += delta;
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
        updateCameraVectors();
    }

    void setPosition(glm::vec3 position)
    {
        Position = position;
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void onMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f; 
    }

    bool isMouseGrabbed() const
    {
        return m_mouseGrabbed;
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 getViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    float deltaTime;

    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // euler Angles
    float Yaw;
    float Pitch;

    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

private:

    GLFWwindow* m_window;
    glm::vec2 m_lastMousePosition;
    bool m_mouseGrabbed;

    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};

#endif //CAMERA_HPP
