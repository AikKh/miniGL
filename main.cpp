#include "application.h"

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

class App : public Application {
public:
	App() : Application{ "Test", SCR_WIDTH , SCR_HEIGHT } {}

	void OnStart() override
	{
		auto source_path = "resources\\monkey.obj";
		Loader loader;

		auto transform = std::make_shared<Transform>();
		auto mesh = std::make_shared<Mesh>(loader.LoadOBJ(source_path));
		auto renderer = std::make_shared<Renderer>("shader.vert", "shader.frag");

		renderer->SetVec3("color", 0.4, 0, 0.6);
		
		AddObject({ transform, mesh, renderer });

		SetLight({ { 0, 0, 3 } });
	}

	void OnUpdate() override
	{
		for (const auto& object : m_scene)
		{
			object.Transform->Rotate({ 0, glfwGetTime() * 50, 0 });
		}
	}
};

int main()
{	
	App app{};
	app.Run();
}
