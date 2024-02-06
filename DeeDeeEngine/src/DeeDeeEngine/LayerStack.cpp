#include "deepch.h"
#include "LayerStack.h"
namespace DeeDeeEngine {
	LayerStack::LayerStack() {
		m_LayerInsert = m_Layers.begin();
	}

	LayerStack::~LayerStack() {
		for (Layer* layer : m_Layers)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer) {
		m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
	}
	void LayerStack::PushOverlay(Layer* overlay) {
		m_Layers.emplace_back(overlay);
	}
	void LayerStack::PopLayer(Layer* layer) {
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end()) {
			m_Layers.erase(it);
			m_LayerInsert--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay) {
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);//在 m_Layers 容器中查找指定的覆盖层 overlay
		if (it != m_Layers.end())
			m_Layers.erase(it);//移除
	}
}