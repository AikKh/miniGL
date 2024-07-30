#pragma once

#include "includes.h"
#include "camera.h"
#include "scene.h"
#include "light.h"

class Window {
public:
	Window(int width, int height) : 
		m_width{ width }, m_height{ height }, m_window {}
	{
		/*glfwMakeContextCurrent(m_window);*/
		glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);

		// Capturing mouse
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

	void SetCursorPosCallback(GLFWcursorposfun func) const
	{
		glfwSetCursorPosCallback(m_window, func);
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

	void Draw(const Camera& camera, const Scene& scene, const Light& light) const
	{
		for (const auto& object : scene)
		{
			object.Select();

			// Vertex
			object.Renderer->SetMatrix4f("model", object.Transform->GetModelMatrix());
			object.Renderer->SetMatrix4f("view", camera.GetViewMatrix());
			object.Renderer->SetMatrix4f("projection", camera.GetProjectionMatrix());
			object.Renderer->SetMatrix3f("transposedModel", object.Transform->GetTransposeModelMatrix());

			// Fragmet
			object.Renderer->SetVec3("lightPos", light.GetPosition());

			glDrawArrays(GL_TRIANGLES, 0, object.Mesh->FaceCount * 3); // Without EBO
			// glDrawElements(GL_TRIANGLES, object.Mesh->FaceCount * 3, GL_UNSIGNED_INT, 0); // With EBO
		}
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
};