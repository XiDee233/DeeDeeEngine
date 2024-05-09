#pragma once

#include "Entity.h"
namespace DeeDeeEngine {
	class ScriptableEntity {
	public:
		virtual ~ScriptableEntity() {}

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

		virtual void OnCollision(Entity* a, Entity* b) {}
	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnEvent(Event& e) {}
		
	private:
		Entity m_Entity;
		friend class Scene;
	};
}