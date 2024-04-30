#pragma once

#include "entt.hpp"
#include "DeeDeeEngine/Core/Timestep.h"
namespace DeeDeeEngine {
	//ǰ��������Forward Declaration����һ�������ࡢ����������ķ�ʽ�������ṩ�䶨�塣��DeeDeeEngine�����ռ��е�class Entity;����һ��ǰ��������
	//�����߱�����Entity��һ���࣬�������Ķ��彫�ں����������������Ŀ����Ϊ�˽��ѭ���������⣬������Ϊ�˼��ٱ��������ԣ��Ӷ��ӿ�����ٶȡ�
	class Entity;
	class Scene {
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());

		void OnViewportResize(uint32_t width, uint32_t height);
		void OnUpdate(Timestep ts);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		friend class Entity;
	};
}