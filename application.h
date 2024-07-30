#pragma once

#include "camera.h"
#include "window.h"
#include "loader.h"

class Application {
public:
	Application(const char* title, int width, int height, int fps = 60, bool enable_depth_buffer = true, bool cull_face = true);

	Application(const Application&) = delete;

	virtual void OnStart() = 0;

	virtual void OnUpdate() = 0;

	virtual void OnUpdate(float deltaTime) {};

public:
	void SetClearColor(int r, int g, int b, int alpha = 255)
	{
		glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, alpha / 255.0f);
	}

	void SetLight(Light light)
	{
		m_light = light;
	}

	void AddObject(Object object)
	{
		m_scene.Add(std::move(object));
	}

	void Run();

private:
	static void posCallback(GLFWwindow* window, double xposIn, double yposIn)
	{
		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);

		float xoffset = xpos - selectedCamera.m_last_x;
		float yoffset = selectedCamera.m_last_y - ypos; // reversed since y-coordinates go from bottom to top

		selectedCamera.m_last_x = xpos;
		selectedCamera.m_last_y = ypos;

		selectedCamera.ProcessMouseMovement(xoffset, yoffset);
	}

	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		selectedCamera.ProcessMouseScroll(static_cast<float>(yoffset));
	}

private:
	int m_fps;

protected:
	Window m_window;

	Scene m_scene;
	Light m_light;
	static Camera selectedCamera;
};