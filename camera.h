#pragma once

#include "includes.h"

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

namespace
{
    const float YAW = -90.0f;
    const float PITCH = 0.0f;
    const float SPEED = 10.0f;
    const float SENSITIVITY = 0.08f;
    const float ZOOM = 45.0f;
}

class Camera final {
public:
    Camera(int width, int height, glm::vec3 position = glm::vec3(0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) :
        m_position{ position }, m_world_up{ up }, m_yaw{ yaw }, m_pitch{ pitch },
        m_speed{ SPEED }, m_mouse_sensitivity{ SENSITIVITY }, m_zoom{ ZOOM },
        m_width{ width }, m_height{ height },
        m_last_x{ width / 2.0f }, m_last_y{ height / 2.0f }
    {
        UpdateCameraVectors();

        UpdateViewMatrix();
        UpdateProjectionMatrix();
    }

    friend class Application;

    const glm::mat4& GetViewMatrix() const&
    {
        return m_view_matrix;
    }

    void UpdateViewMatrix()
    {
        m_view_matrix = glm::lookAt(m_position, m_position + m_front, m_up);
    }

    const glm::mat4& GetProjectionMatrix() const&
    {
        return m_projection_matrix;
    }

    void UpdateProjectionMatrix(float range = 100.0f)
    {
        m_projection_matrix = glm::perspective(glm::radians(m_zoom), (float)m_width / (float)m_height, 0.1f, range);
    }

    float GetZoom() const
    {
        return m_zoom;
    }

    void ProcessKeyboard(CameraMovement direction, float deltaTime)
    {
        float velocity = m_speed * deltaTime;
        if (direction == FORWARD)
            m_position += m_front * velocity;
        if (direction == BACKWARD)
            m_position -= m_front * velocity;
        if (direction == LEFT)
            m_position -= m_right * velocity;
        if (direction == RIGHT)
            m_position += m_right * velocity;
    }

    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= m_mouse_sensitivity;
        yoffset *= m_mouse_sensitivity;

        m_yaw += xoffset;
        m_pitch += yoffset;

        if (constrainPitch)
        {
            if (m_pitch > 89.0f)
                m_pitch = 89.0f;
            if (m_pitch < -89.0f)
                m_pitch = -89.0f;
        }

        UpdateCameraVectors();

        UpdateViewMatrix();
        UpdateProjectionMatrix();
    }

    void ProcessMouseScroll(float yoffset)
    {
        m_zoom -= (float)yoffset;
        if (m_zoom < 1.0f)
            m_zoom = 1.0f;
        if (m_zoom > 45.0f)
            m_zoom = 45.0f;
    }

    std::string ToString() const
    {
        return "X: " + std::to_string(m_position.x) + ", " +
               "Y: " + std::to_string(m_position.y) + ", " +
               "Z: " + std::to_string(m_position.z);
    }

private:
    void UpdateCameraVectors()
    {
        m_front =
        {
            cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)),
            sin(glm::radians(m_pitch)),
            sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch))
        };
        m_front = glm::normalize(m_front);

        m_right = glm::normalize(glm::cross(m_front, m_world_up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        m_up = glm::normalize(glm::cross(m_right, m_front));
    }

private:
#pragma region Fields

    glm::mat4 m_view_matrix;
    glm::mat4 m_projection_matrix;

    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_world_up;

    int m_width;
    int m_height;

    float m_yaw;
    float m_pitch;

    float m_speed;
    float m_mouse_sensitivity;
    float m_zoom;

    float m_last_x;
    float m_last_y;

    //bool m_view_updated;
    //bool m_projection_updated;

#pragma endregion
};