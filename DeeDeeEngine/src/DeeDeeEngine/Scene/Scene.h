#pragma once

#include "entt.hpp"
#include "DeeDeeEngine/Core/Timestep.h"
namespace DeeDeeEngine {
	//前向声明（Forward Declaration）是一种声明类、函数或变量的方式，而不提供其定义。在DeeDeeEngine命名空间中的class Entity;就是一个前向声明。
	//它告诉编译器Entity是一个类，但是它的定义将在后面给出。这样做的目的是为了解决循环依赖问题，或者是为了减少编译依赖性，从而加快编译速度。
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