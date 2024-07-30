#include "application.h"

Application::Application(const char* title, int width, int height, int fps, bool enable_depth_buffer, bool cull_face)
	: m_window{ width, height }, m_fps{ fps }, m_light{ {0, 0, 0} }
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
		/*glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glFrontFace(GL_CCW);*/
	}

	selectedCamera = { width, height, { 0, 0, 3 } };

	//m_window.SetCursorPosCallback(posCallback);
	m_window.SetScrollCallback(scrollCallback);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Application::Run()
{
	OnStart();

	double targetFrameTime = 1.0 / m_fps;
	double startTime = 0, endTime = 0, deltaTime = 0;

#pragma region Test3

	//Renderer cube_renderer{ "shader.vert", "shader.frag" };
	/*auto renderer = std::make_shared<Renderer>("shader.vert", "shader.frag");

	Loader loader;
	auto mesh = std::make_shared<Mesh>(loader.LoadOBJ("C:\\Projects\\C++ Projects\\FirstOpenGL\\Objects\\monkey.obj"));

	auto transform = std::make_shared<Transform>();

	Object object{ transform, mesh, renderer };
	object.Renderer->SetVec3("color", 1, 1, 1);

	Scene scene;
	scene.Add(std::move(object));*/

#pragma endregion

	while (!m_window.ShouldClose())
	{
		deltaTime += endTime - startTime;
		startTime = glfwGetTime();

		while (deltaTime > targetFrameTime)
		{
			// process_input(window, timeSinceLastUpdate); // TODO: encapsulate into InputHandler
			OnUpdate(static_cast<float>(targetFrameTime)); 
			deltaTime -= targetFrameTime;
		}

		OnUpdate();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#pragma region Test3
		//auto time = glfwGetTime() * 50 + 5.0;

		//for (const auto& object : scene)
		//{
		//	object.Select();

		//	object.Transform->Rotate({ 0, 0, 0 });

		//	object.Renderer->SetMatrix4f("model", object.Transform->GetModelMatrix());
		//	object.Renderer->SetMatrix4f("view", selectedCamera.GetViewMatrix());
		//	object.Renderer->SetMatrix4f("projection", selectedCamera.GetProjectionMatrix());

		//	glDrawArrays(GL_TRIANGLES, 0, object.Mesh->FaceCount * 3); // Without EBO
		//}
		

#pragma endregion

		m_window.Draw(selectedCamera, m_scene, m_light);
		m_window.SwapBuffers();

		endTime = glfwGetTime();
	}
}

Camera Application::selectedCamera{ 1, 1 };
