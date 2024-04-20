#include <Dee.h>
#include "DeeDeeEngine/Core/EntryPoint.h"


#include "Sandbox2D.h"
#include "GameLayer.h"


class Sandbox :public DeeDeeEngine::Application {
public:
	Sandbox()
	{
		
		//PushLayer(new GameLayer());
		PushLayer(new Sandbox2D());

	}
	~Sandbox() {

	}
};
DeeDeeEngine::Application* DeeDeeEngine::CreateApplication() {
	return new Sandbox();
}