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
		template<typename T>
		T& AddComponent() {
			return m_Entity.AddComponent<T>();
		}
		virtual Entity& GetEntity() { return m_Entity; }

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