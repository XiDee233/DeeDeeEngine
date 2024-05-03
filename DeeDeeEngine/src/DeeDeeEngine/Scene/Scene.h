#pragma once
#include "DeeDeeEngine/Renderer/EditorCamera.h"
#include "DeeDeeEngine\Core\UUID.h"
#include "entt.hpp"
#include "DeeDeeEngine/Core/Timestep.h"

class b2World;
namespace DeeDeeEngine {
	//前向声明（Forward Declaration）是一种声明类、函数或变量的方式，而不提供其定义。在DeeDeeEngine命名空间中的class Entity;就是一个前向声明。
	//它告诉编译器Entity是一个类，但是它的定义将在后面给出。这样做的目的是为了解决循环依赖问题，或者是为了减少编译依赖性，从而加快编译速度。
	class Entity;
	class Scene {
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnViewportResize(uint32_t width, uint32_t height);
		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);

		Entity GetPrimaryCameraEntity();
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		b2World* m_PhysicsWorld = nullptr;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
	
}