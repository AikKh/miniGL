#pragma once

#include <map>
#include <array>

#include "window.h"
#include "camera.h"

class InputHandler {
public:
	void Process(Camera& camera, const Window& window, double deltaTime)
	{
        // Continuous
        /*for (auto& [key, func] : m_window_press)
        {
            if (glfwGetKey(window.GetPtr(), key) == GLFW_PRESS)
            {
                func(window);
            }
        }*/

        for (auto& [key, func] : m_camera_press)
        {
            if (glfwGetKey(window.GetPtr(), key) == GLFW_PRESS)
            {
                func(camera, deltaTime);
            }
        }

        /*for (auto& [key, func] : m_window_release)
        {
            if (glfwGetKey(window.GetPtr(), key) == GLFW_RELEASE)
            {
                func(window);
            }
        }*/

        // Single
        for (auto& [key, func] : m_window_single_press)
        {
            if (!m_pressed_table[key] && glfwGetKey(window.GetPtr(), key) == GLFW_PRESS)
            {
                func(window);
                m_pressed_table[key] = true;
            }
        }

        /*for (auto& [key, func] : m_camera_single_press)
        {
            if (!m_pressed_table[key] && glfwGetKey(window.GetPtr(), key) == GLFW_PRESS)
            {
                func(camera, deltaTime);
                m_pressed_table[key] = true;
            }
        }*/

        for (auto& [key, func] : m_window_single_release)
        {
            if (m_pressed_table[key] && glfwGetKey(window.GetPtr(), key) == GLFW_RELEASE)
            {
                func(window);
                m_pressed_table[key] = false;
            }
        }
	}

    // Continuous
    /*void OnPressOf(unsigned int key, std::function<void(const Window&)> func)
    {
        m_window_press[key] = std::move(func);
    }*/

    void OnPressOf(unsigned int key, std::function<void(Camera&, double)> func)
    {
        m_camera_press[key] = std::move(func);
    }

    /*void OnReleaseOf(unsigned int key, std::function<void(const Window&)> func)
    {
        m_window_release[key] = std::move(func);
    }*/

    // Single
    void OnSinglePressOf(unsigned int key, std::function<void(const Window&)> func)
    {
        m_window_single_press[key] = std::move(func);
    }

    /*void OnSinglePressOf(unsigned int key, std::function<void(Camera&, double)> func)
    {
        m_camera_single_press[key] = std::move(func);
    }*/

    void OnSingleReleaseOf(unsigned int key, std::function<void(const Window&)> func)
    {
        m_window_single_release[key] = std::move(func);
    }

private:
    std::map<int, std::function<void(const Window&)>> m_window_single_press;
    //std::map<int, std::function<void(const Window&)>> m_window_press;

    //std::map<int, std::function<void(Camera&, double)>> m_camera_single_press;
    std::map<int, std::function<void(Camera&, double)>> m_camera_press;

    std::map<int, std::function<void(const Window&)>> m_window_single_release;
    //std::map<int, std::function<void(const Window&)>> m_window_release;

    std::array<bool, GLFW_KEY_LAST> m_pressed_table = { false };
};