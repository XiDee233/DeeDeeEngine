#include"deepch.h"
#include"GameMain.h"
#include <Dee.h>
#include <box2d\include\box2d\b2_math.h>
namespace DeeDeeEngine {
	

	class PlayerController;
	
	void GameMain::Init(EditorLayer* layer) {
		m_layer = layer;
		Entity Cam = m_layer->m_ActiveScene->CreateEntity("Cam");
		Cam.AddComponent<CameraComponent>();

		m_player = m_layer->m_ActiveScene->CreateEntity("Test01");
		m_player.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		m_player.AddComponent<NativeScriptComponent>().Bind<PlayerController>();
	    m_player.AddComponent<Rigidbody2DComponent>();
		m_player.GetComponent<Rigidbody2DComponent>().SetDynamic();
		m_player.AddComponent<BoxCollider2DComponent>();

		m_Groud = m_layer->m_ActiveScene->CreateEntity("Groud");
		m_Groud.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
		m_Groud.GetComponent<TransformComponent>().Scale = glm::vec3(10.0f, 1.0f, 1.0f);
		m_Groud.GetComponent<TransformComponent>().Translation = glm::vec3(0, -2, 0);
		m_Groud.AddComponent<Rigidbody2DComponent>();
		m_Groud.AddComponent<BoxCollider2DComponent>();
	}


	class PlayerController :public ScriptableEntity {
	private:
		
		Rigidbody2DComponent* rigidbody;
	public:
		void OnCreate() {
			rigidbody = &GameMain::GetInstance().m_player.GetComponent<Rigidbody2DComponent>();

		}

		void OnUpdate(Timestep ts) {
			if (Input::IsKeyPressed(DEE_KEY_A))
			{
				rigidbody->AddForce(b2Vec2(-10.0f, 0.0f));
				
			}
			if (Input::IsKeyPressed(DEE_KEY_D))
			{
				rigidbody->AddForce(b2Vec2(10.0f, 0.0f));
			}
	
		}

		void OnEvent(Event& e) {
			EventDispatcher dispatcher(e);
			// 使用 std::bind 绑定成员函数到当前实例 this 上
			dispatcher.Dispatch<KeyPressedEvent>(std::bind(&PlayerController::OnKeyPressed, this, std::placeholders::_1));
		}

		bool OnKeyPressed(KeyPressedEvent& e) {

			switch (e.GetKeyCode()) {
			case DEE_KEY_SPACE:
				if (!e.IsRepeat())
				{
					rigidbody->AddForce(b2Vec2(0.0f, 500.0f));
					
				}
				break;
			case DEE_KEY_ENTER:
				if (!e.IsRepeat()) {
					ShootBullets();
				}
				break;
			}
			
			return false;
		}

		void ShootBullets() {

			auto b = GetEntity().GetScene()->CreateEntity("bullet");
			b.AddComponent<NativeScriptComponent>().Bind<Bullet>();
		}

		void OnCollision(Entity* a, Entity* b) {
			std::cout << "Collision between: " << a->GetName() << " and " << b->GetName() << std::endl;
		}

		class Bullet :public ScriptableEntity {
			void OnCreate() {
 				auto& transform = GetComponent<TransformComponent>();
				transform.Translation.x = GameMain::GetInstance().m_player.GetComponent<TransformComponent>().Translation.x + 0.5f;
				AddComponent<Rigidbody2DComponent>();
				AddComponent<BoxCollider2DComponent>();
				auto& pic = AddComponent<SpriteRendererComponent>();
				transform.Scale = glm::vec3(0.2f, 0.2f, 1.0f);
				auto& rigidbody = GetComponent<Rigidbody2DComponent>();
				rigidbody.AddForce(b2Vec2(500, 0));
				rigidbody.SetDynamic();
			}
		};
	};

	

	
}