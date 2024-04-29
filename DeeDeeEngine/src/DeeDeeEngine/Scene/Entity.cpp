#include "deepch.h"
#include "Entity.h"
namespace DeeDeeEngine {
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}
}