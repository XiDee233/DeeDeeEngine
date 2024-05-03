#pragma once
#include "DeeDeeEngine/Renderer/EditorCamera.h"
#include "DeeDeeEngine\Core\UUID.h"
#include "entt.hpp"
#include "DeeDeeEngine/Core/Timestep.h"

class b2World;
namespace DeeDeeEngine {
	//ǰ��������Forward Declaration����һ�������ࡢ����������ķ�ʽ�������ṩ�䶨�塣��DeeDeeEngine�����ռ��е�class Entity;����һ��ǰ��������
	//�����߱�����Entity��һ���࣬�������Ķ��彫�ں����������������Ŀ����Ϊ�˽��ѭ���������⣬������Ϊ�˼��ٱ��������ԣ��Ӷ��ӿ�����ٶȡ�
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