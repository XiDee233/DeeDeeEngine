#include <Dee.h>
#include "DeeDeeEngine/Core/EntryPoint.h"


#include "EditorLayer.h"
#include "../GameMain.h"


class DeeEditor :public DeeDeeEngine::Application {
private:
	DeeDeeEngine::EditorLayer* m_EditorLayer;
public:
	DeeEditor()
		:Application("Dee Editor")
	{
		
		//PushLayer(new GameLayer());
		m_EditorLayer = new DeeDeeEngine::EditorLayer();
		PushLayer(m_EditorLayer);
		DeeDeeEngine::GameMain::Init(m_EditorLayer);

	}
	~DeeEditor() {

	}
};
DeeDeeEngine::Application* DeeDeeEngine::CreateApplication() {	return new DeeEditor();
}