#include <Dee.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "imgui/imgui.h"
glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
{
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);
	glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	return Projection * View * Model;
}

class ExampleLayer :public DeeDeeEngine::Layer {
public:
	ExampleLayer()
		:Layer("Example") {
		auto cam = camera(5.0f, { 0.5f,0.5f });
	}
	void ExampleLayer:: OnUpdate() override {
		if (DeeDeeEngine::Input::IsKeyPressed(DEE_KEY_TAB)) {
			DEE_TRACE("Tab key is pressed!");
		}
	}
	void ExampleLayer::OnEvent(DeeDeeEngine::Event& event)override {
		if (event.GetEventType() == DeeDeeEngine::EventType::KeyPressed)
		{
			DeeDeeEngine::KeyPressedEvent& e = (DeeDeeEngine::KeyPressedEvent&)event;
			DEE_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
	void ExampleLayer::OnImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::Text("HelloWorld");
		ImGui::End();
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