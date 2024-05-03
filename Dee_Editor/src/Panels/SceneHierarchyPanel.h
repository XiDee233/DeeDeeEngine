#pragma once

#include "DeeDeeEngine/Core/Core.h"
#include "DeeDeeEngine/Core/Log.h"
#include "DeeDeeEngine/Scene/Scene.h"
#include "DeeDeeEngine/Scene/Entity.h"

namespace DeeDeeEngine {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();

		Entity GetSelectedEntity() const { return m_SelectionContext; }
		void SetSelectedEntity(Entity entity);
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};

}