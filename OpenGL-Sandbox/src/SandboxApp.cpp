#include "GLCore.h"
#include "SandboxLayer.h"

using namespace GLCore;

class Sandbox : public Application
{
public:
	Sandbox()
	{
		PushLayer(new SandboxLayer());
	}
};

int main()
{
	//GLCore::Log::Init();

	GLCORE_PROFILE_BEGIN_SESSION("Startup", "CoreProfile-Startup.json");
	std::unique_ptr<Sandbox> app = std::make_unique<Sandbox>();
	GLCORE_PROFILE_END_SESSION();

	GLCORE_PROFILE_BEGIN_SESSION("Runtime", "CoreProfile-Runtime.json");
	app->Run();
	GLCORE_PROFILE_END_SESSION();
}