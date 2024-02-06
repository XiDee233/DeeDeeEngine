#include <Dee.h>

class ExampleLayer :public DeeDeeEngine::Layer {
public:
	ExampleLayer() 
		:Layer("Example") {

		}
	void OnUpdate() override {
		DEE_INFO("ExampleLayer::Update");
		}
	void OnEvent(DeeDeeEngine::Event& event)override {
		DEE_INFO("{0}", event);
	}
	
};
class Sandbox :public DeeDeeEngine::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {

	}
};
DeeDeeEngine::Application* DeeDeeEngine::CreateApplication() {
	return new Sandbox();
}