#include "application.h"

Application::Application(const char* title, int width, int height, int fps, bool enable_depth_buffer, bool cull_face, float camera_speed)
	: m_window{ width, height }, m_fps{ fps }
{
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	auto window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

	m_window.SetWindow(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return;
	}

	if (enable_depth_buffer)
	{
		glEnable(GL_DEPTH_TEST);
	}

	if (cull_face)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glFrontFace(GL_CCW);
	}

	selectedCamera = { width, height, camera_speed, { 0, 0, 3 } };

	m_window.SetCursorPosCallback(posCallback);
	m_window.SetScrollCallback(scrollCallback);

	m_window.CaptureCursor();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Application::Run()
{
	OnStart();

	double targetFrameTime = 1.0 / m_fps;
	double startTime = 0, endTime = 0, deltaTime = 0;

	while (!m_window.ShouldClose())
	{
		deltaTime += endTime - startTime;
		startTime = glfwGetTime();

		while (deltaTime > targetFrameTime)
		{
			m_input_handler.Process(selectedCamera, m_window, targetFrameTime);
			OnUpdate(static_cast<float>(targetFrameTime)); 
			deltaTime -= targetFrameTime;
		}

		OnUpdate();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		OnDraw(selectedCamera, m_scene);
		m_window.SwapBuffers();

		endTime = glfwGetTime();
	}
}

Camera Application::selectedCamera{ 1, 1 };
