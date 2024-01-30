#pragma once
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <spdlog/fmt/ostr.h>
namespace DeeDeeEngine {
	class DEE_API Log
	{
	public:
		static void Init();
		//inline ��������������ʱ�����ֱ�Ӳ��뵽���õط��������Ǻ�������
		// &��ʾ����ֵ��һ�����ã���һ��������������һ���������������Ա��ⲻ��Ҫ�ĸ��Ʋ�����������ܡ�
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}


#define DEE_CORE_ERROR(...)    ::DeeDeeEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define DEE_CORE_WARN(...)     ::DeeDeeEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define DEE_CORE_INFO(...)     ::DeeDeeEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define DEE_CORE_TRACE(...)    ::DeeDeeEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define DEE_CORE_FATAL(...)    ::DeeDeeEngine::Log::GetCoreLogger()->fatal(__VA_ARGS__)


#define DEE_ERROR(...)    ::DeeDeeEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define DEE_WARN(...)     ::DeeDeeEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define DEE_INFO(...)     ::DeeDeeEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define DEE_TRACE(...)    ::DeeDeeEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define DEE_FATAL(...)    ::DeeDeeEngine::Log::GetClientLogger()->fatal(__VA_ARGS__)