#include <Dee.h>
#include "DeeDeeEngine/Core/EntryPoint.h"


#include "EditorLayer.h"


class DeeEditor :public DeeDeeEngine::Application {
public:
	DeeEditor()
		:Application("Dee Editor")
	{
		
		//PushLayer(new GameLayer());
		PushLayer(new DeeDeeEngine::EditorLayer());
	}
	~DeeEditor() {

	}
};
DeeDeeEngine::Application* DeeDeeEngine::CreateApplication() {
	return new DeeEditor();
}