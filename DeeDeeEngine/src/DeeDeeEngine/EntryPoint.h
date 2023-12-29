#pragma once
#ifdef DEE_PLATFORM_WINDOWS

extern DeeDeeEngine::Application* DeeDeeEngine::CreateApplication();

int main(int argc,char** argv) {
	printf("DeeDeeEngine!\n");
	auto app = DeeDeeEngine::CreateApplication();
	app->Run();
	delete app;

}
#endif // 
