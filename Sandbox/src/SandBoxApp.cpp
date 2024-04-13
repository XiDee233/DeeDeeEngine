#include <Dee.h>
#include "DeeDeeEngine/Core/EntryPoint.h"
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "imgui/imgui.h"
#include <Platform\OpenGL\OpenGLShader.h>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

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
		:Layer("Example"), m_CameraController(1270.0f/720.0f,true)
	{

		m_SquareVA = (DeeDeeEngine::VertexArray::Create());


		float vertices2[5 * 4] = {
			-0.5f, -0.5f, 0.0f,0.0f,0.0f,
			 0.5f, -0.5f, 0.0f,1.0f,0.0f,
			 0.5f,  0.5f, 0.0f,1.0f,1.0f,
			-0.5f,  0.5f, 0.0f,0.0f,1.0f

		};
		DeeDeeEngine::Ref<DeeDeeEngine::VertexBuffer> squareVB;
		squareVB.reset(DeeDeeEngine::VertexBuffer::Create(vertices2, sizeof(vertices2)));
		DeeDeeEngine::BufferLayout _layout2 = {
			{DeeDeeEngine::ShaderDataType::Float3,"a_Position"},
			{DeeDeeEngine::ShaderDataType::Float2,"a_TexCoord"}
		};
		squareVB->SetLayout(_layout2);

		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t indices2[6] = { 0,1,2,2,3,0 };
		DeeDeeEngine::Ref<DeeDeeEngine::IndexBuffer> squareIB;
		squareIB.reset(DeeDeeEngine::IndexBuffer::Create(indices2, sizeof(indices2) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);
		std::string vertexSrc = R"(
         #version 330 core
         layout(location = 0)in vec3 a_Position;
layout(location =1) in vec4 a_Color;

 uniform mat4 u_ViewProjection;
 uniform mat4 u_Transform;


out vec3 v_Position;
out vec4 v_Color;
         
         void main(){
v_Position = a_Position;
v_Color =a_Color;
             gl_Position =u_ViewProjection*u_Transform* vec4(a_Position,1.0);
}
         )";
		std::string fragmentSrc = R"(
  #version 330 core
         layout(location = 0)out vec4 color;

uniform vec4 u_Color;

in vec3 v_Position;
in vec4 v_Color;
         
         void main(){
             color = vec4(v_Position+0.5,1.0);
color = v_Color;
}           
         )";

		m_Shader =(DeeDeeEngine::Shader::Create("Triangle",vertexSrc, fragmentSrc));

		std::string flatShaderVertexSrc2 = R"(
         #version 330 core
         layout(location = 0)in vec3 a_Position;

 uniform mat4 u_ViewProjection;
 uniform mat4 u_Transform;


out vec3 v_Position;
         
         void main(){
v_Position = a_Position;
             gl_Position = u_ViewProjection *u_Transform* vec4(a_Position,1.0);
}
         )";
		std::string flatShaderFragmentSrc2 = R"(
  #version 330 core
         layout(location = 0)out vec4 color;
in vec3 v_Position;
uniform vec3 u_Color;
         
         void main(){
             color = vec4(u_Color,1.0);
}           
         )";

		m_FlatColorShader=(DeeDeeEngine::Shader::Create("Flat",flatShaderVertexSrc2, flatShaderFragmentSrc2));

	
		auto textureShader =m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
		m_Texture=(DeeDeeEngine::Texture2D::Create("./assets/textures/cjy.png"));
		m_DeeLogoTexture = (DeeDeeEngine::Texture2D::Create("./assets/textures/logo.png"));

		std::dynamic_pointer_cast<DeeDeeEngine::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<DeeDeeEngine::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}
	void ExampleLayer::OnUpdate(DeeDeeEngine::Timestep ts) override {
		
		
		m_CameraController.OnUpdate(ts);

		DeeDeeEngine::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
		DeeDeeEngine::RenderCommand::Clear();
		

		DeeDeeEngine::Renderer::BeginScene(m_CameraController.GetCamera());
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));



		/*	DeeDeeEngine::MaterialRef material = new DeeDeeEngine::Material(m_FlatColorShader);
			DeeDeeEngine::MaterialInstanceRef mi = new DeeDeeEngine::MaterialInstance(material);
			mi->SetValue("u_Color", redColor);
			mi->SetTexture("u_AlbedoMap", texture);
			squareMesh->SetMaterial(mi);*/

		std::dynamic_pointer_cast<DeeDeeEngine::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<DeeDeeEngine::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;


				DeeDeeEngine::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);

			}
		}
		//glm::mat4 transform = glm::translate(glm::mat4(1.0f),m_SquarePosition);
		auto textureShader = m_ShaderLibrary.Get("Texture");
		m_Texture->Bind();
		DeeDeeEngine::Renderer::Submit(textureShader, m_SquareVA, glm::translate(glm::mat4(1.0f), glm::vec3(-0.5, 0, 0)));
		m_DeeLogoTexture->Bind();
		DeeDeeEngine::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		DeeDeeEngine::Renderer::EndScene();
	}
	void ExampleLayer::OnEvent(DeeDeeEngine::Event& event)override {
		/*	if (event.GetEventType() == DeeDeeEngine::EventType::KeyPressed)
			{ 
				DeeDeeEngine::KeyPressedEvent& e = (DeeDeeEngine::KeyPressedEvent&)event;
				DEE_TRACE("{0}", (char)e.GetKeyCode());
			}*/

		m_CameraController.OnEvent(event);

		DeeDeeEngine::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<DeeDeeEngine::KeyPressedEvent>(DEE_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));

	}

	bool OnKeyPressedEvent(DeeDeeEngine::KeyPressedEvent& event) {

		return false;
	}
	void ExampleLayer::OnImGuiRender() override {
	/*	ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();*/
	}
private:

	DeeDeeEngine::ShaderLibrary m_ShaderLibrary;
	DeeDeeEngine::Ref<DeeDeeEngine::Shader> m_Shader;
	DeeDeeEngine::Ref<DeeDeeEngine::VertexArray> m_VertexArray;


	DeeDeeEngine::Ref<DeeDeeEngine::Shader> m_FlatColorShader;
	DeeDeeEngine::Ref<DeeDeeEngine::VertexArray> m_SquareVA;

	DeeDeeEngine::Ref<DeeDeeEngine::Texture2D> m_Texture,m_DeeLogoTexture;

	DeeDeeEngine::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquarePosition;
	glm::vec3 m_SquareColor = { 0.2f,0.3f,0.4f };
};
class Sandbox :public DeeDeeEngine::Application {
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}
	~Sandbox() {

	}
};
DeeDeeEngine::Application* DeeDeeEngine::CreateApplication() {
	return new Sandbox();
}