#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <vector>

#include "Shader.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    UP,
    DOWN,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 10.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Prevents the camera from jumping around when first clicking left click
    bool firstClick = true;

    // Adjust the speed of the camera and it's sensitivity when looking around
    float speed = 0.1f;
    float sensitivity = 100.0f;

    Camera();

    // camera Attributes
    glm::vec3 Front;
    glm::vec3 Right;

    // camera options
    float MovementSpeed;

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();

    glm::mat4 GetProjectionMatrix();

    glm::vec3 GetPosition(void) { return m_position_coords; }

    void Orbit(float x_offset, float y_offset);

    void Zoom(double y_offset);

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

private:

    float m_fov = 45.0f;
    float m_distance = 10.0;
    float m_position_xangle = 90.0;
    float m_position_yangle = 30.0;
    glm::vec3 m_position_coords = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 m_up_vec = glm::vec3(0.0, 1.0, 0.0);
    const glm::vec3 m_WORLD_UP_VEC = glm::vec3(0.0, 1.0, 0.0);

    // Stores the width and height of the window
    unsigned int width = 800;
    unsigned int height = 800;

    // Sensitivities
    float m_mouse_sensitivity = 0.3;
    float m_zoom_sensitivity = 0.5;
    float m_fov_sensitivity = 0.4;

    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
};
#endif