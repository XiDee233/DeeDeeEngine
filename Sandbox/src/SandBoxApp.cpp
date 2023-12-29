#include <Dee.h>
class Sandbox :public DeeDeeEngine::Application {
public:
	Sandbox() {

	}
	~Sandbox() {

	}
};
DeeDeeEngine::Application* DeeDeeEngine::CreateApplication() {
	return new Sandbox();
}