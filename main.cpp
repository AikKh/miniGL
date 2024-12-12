#include "application.h"

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

class App : public Application {
public:
	App() : Application{ "Test", SCR_WIDTH , SCR_HEIGHT }, m_light{ { 0, 0, 3 } } {}

	void OnStart() override
	{
		Loader loader;

		auto monkey_transform = std::make_shared<Transform>();
		auto monkey_mesh = std::make_shared<Mesh>(loader.Load("resources\\iso.obj", { BufferDataType::Vertex, BufferDataType::Vertex }, true));
		auto monkey_renderer = std::make_shared<Renderer>("shader.vert", "shader.frag");

		// It should be before loading second shader. This is very important
		monkey_renderer->SetVec3("color", 0.4, 0, 0.6);

		auto light_transform = std::make_shared<Transform>();
		auto light_mesh = std::make_shared<Mesh>(loader.Load("resources\\iso.obj", { BufferDataType::Vertex }, true));
		auto light_renderer = std::make_shared<Renderer>("light_shader.vert", "light_shader.frag"); // This line causes everything


		AddObject({ monkey_transform, monkey_mesh, monkey_renderer, [this](const Object& obj)
 {
obj.Renderer->SetMatrix4f("model", obj.Transform->GetModelMatrix());
obj.Renderer->SetMatrix4f("view", selectedCamera.GetViewMatrix());
obj.Renderer->SetMatrix4f("projection", selectedCamera.GetProjectionMatrix());
obj.Renderer->SetMatrix3f("transposedModel", obj.Transform->GetTransposeModelMatrix());

// Fragmet
obj.Renderer->SetVec3("lightPos", m_light.GetPosition());
} });

		AddObject({ light_transform, light_mesh, light_renderer, [this](const Object& obj)
 {
obj.Renderer->SetMatrix4f("model", obj.Transform->GetModelMatrix());
obj.Renderer->SetMatrix4f("view", selectedCamera.GetViewMatrix());
obj.Renderer->SetMatrix4f("projection", selectedCamera.GetProjectionMatrix());

obj.Transform->Translate(m_light.GetPosition());
} });

		// Register keys
		m_input_handler.OnSinglePressOf(GLFW_KEY_ESCAPE, [](const Window& w) { w.Exit(); });

		m_input_handler.OnSinglePressOf(GLFW_KEY_C, [](const Window& w) mutable { w.ReleaseCursor(); });
		m_input_handler.OnSingleReleaseOf(GLFW_KEY_C, [](const Window& w) mutable { w.CaptureCursor(); selectedCamera.SetFirstMove(true); });

		m_input_handler.OnPressOf(GLFW_KEY_W, [](Camera& c, double d) { c.ProcessDirection(CameraDirection::FORWARD, d); });
		m_input_handler.OnPressOf(GLFW_KEY_S, [](Camera& c, double d) { c.ProcessDirection(CameraDirection::BACKWARD, d); });
		m_input_handler.OnPressOf(GLFW_KEY_A, [](Camera& c, double d) { c.ProcessDirection(CameraDirection::LEFT, d); });
		m_input_handler.OnPressOf(GLFW_KEY_D, [](Camera& c, double d) { c.ProcessDirection(CameraDirection::RIGHT, d); });
		m_input_handler.OnPressOf(GLFW_KEY_Q, [](Camera& c, double d) { c.ProcessDirection(CameraDirection::UP, d); });
		m_input_handler.OnPressOf(GLFW_KEY_E, [](Camera& c, double d) { c.ProcessDirection(CameraDirection::DOWN, d); });
	}

	void OnUpdate() override
	{
		auto time = glfwGetTime();

		//double scaler = (glm::sin(time) + 1) / 10 + 0.9;

		const float radius = 10.0f;
		float camX = sin(time) * radius;
		float camZ = cos(time) * radius;

		m_light.SetPosition(camX, 0.0, camZ);

		//for (const auto& object : m_scene)
		//{
		//	object.Transform->Rotate({ 0, glfwGetTime() * 100, 0 });
		//	//object.Transform->Scale({ scaler, scaler, scaler });
		//}
	}

	void OnDraw(const Camera& camera, const Scene& scene) override
	{
		for (const auto& object : scene)
		{
			object.Select();

			object.CallUpdater();

			DrawObject(object);
		}
	}

private:
	Light m_light;
};

int main()
{
	App app{};
	app.Run();
}
