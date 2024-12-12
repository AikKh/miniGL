#pragma once
//#include "includes.h"
#include <iostream>
#include <vector>
#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "scene.h"
#include "light.h"

class Window {
public:
	Window(int width, int height) : 
		m_width{ width }, m_height{ height }, m_window {}, m_cursor_callback{ nullptr }
	{
		glfwMakeContextCurrent(m_window);
		glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);

		// Capturing mouse
		glfwSetCursorPos(m_window, width / 2.0, height / 2.0);
	}

	Window(const Window&) = delete;
	Window(Window&&) = default;

public:
	int GetWidth() const
	{
		return m_width;
	}

	int GetHeight() const
	{
		return m_height;
	}

	void SetCursorPosCallback(GLFWcursorposfun func)
	{
		m_cursor_callback = func;
	}

	void SetScrollCallback(GLFWscrollfun func) const
	{
		glfwSetScrollCallback(m_window, func);
	}

	void SetWindow(GLFWwindow* window)
	{
		m_window = window;
		glfwMakeContextCurrent(m_window);
	}

	bool ShouldClose() const
	{
		return glfwWindowShouldClose(m_window);
	}

	void SwapBuffers()
	{
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	void Exit() const
	{
		glfwSetWindowShouldClose(m_window, true);
	}

	void CaptureCursor() const
	{
		glfwSetCursorPosCallback(m_window, m_cursor_callback);
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void ReleaseCursor() const
	{
		glfwSetCursorPosCallback(m_window, nullptr);
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	GLFWwindow* GetPtr() const
	{
		return m_window;
	}

private:
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

private:
	int m_width;
	int m_height;
	GLFWwindow* m_window;

	GLFWcursorposfun m_cursor_callback;
};