#pragma once
#ifdef DEE_PLATFORM_WINDOWS

extern DeeDeeEngine::Application* DeeDeeEngine::CreateApplication();

int main(int argc,char** argv) {
	DeeDeeEngine::Log::Init();
	DEE_CORE_WARN("Init Log!");
	int a = 5;
	DEE_INFO("Hello! Var={0}",a);
	printf("DeeDeeEngine!\n");
	auto app = DeeDeeEngine::CreateApplication();
	app->Run();
	delete app;

}
#endif // 
