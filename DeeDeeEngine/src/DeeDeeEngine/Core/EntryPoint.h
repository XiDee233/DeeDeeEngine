#pragma once
#ifdef DEE_PLATFORM_WINDOWS

extern DeeDeeEngine::Application* DeeDeeEngine::CreateApplication();

int main(int argc, char** argv) {
	DeeDeeEngine::Log::Init();
	printf("DeeDeeEngine!\n");
	DEE_PROFILE_BEGIN_SESSION("Startup", "DeeDeeProfile_Startup.json");
	auto app = DeeDeeEngine::CreateApplication();
	DEE_PROFILE_END_SESSION();

	DEE_PROFILE_BEGIN_SESSION("Startup", "DeeDeeProfile_Runtime.json");
	app->Run();
	DEE_PROFILE_END_SESSION();

	DEE_PROFILE_BEGIN_SESSION("Startup", "DeeDeeProfile_Shutdown.json");
	delete app;
	DEE_PROFILE_END_SESSION();
}
#endif // 
