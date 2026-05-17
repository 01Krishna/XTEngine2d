#pragma once

#include <memory>


#include "Core.h"
#include "spdlog/spdlog.h"


namespace XTEngine2d
{
	class XT_API Log
	{
		Log();
		~Log();	

	public:

		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

//Core Log macros

#define XT_CORE_ERROR(...) ::XTEngine2d::Log::GetCoreLogger()->error(__VA_ARGS__)
#define XT_CORE_WARN(...) ::XTEngine2d::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define XT_CORE_INFO(...) ::XTEngine2d::Log::GetCoreLogger()->info(__VA_ARGS__)
#define XT_CORE_TRACE(...) ::XTEngine2d::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define XT_CORE_CRITICAL(...) ::XTEngine2d::Log::GetCoreLogger()->critical(__VA_ARGS__)


//Client Log macros

#define XT_ERROR(...) ::XTEngine2d::Log::GetClientLogger()->error(__VA_ARGS__)
#define XT_WARN(...) ::XTEngine2d::Log::GetClientLogger()->warn(__VA_ARGS__)
#define XT_INFO(...) ::XTEngine2d::Log::GetClientLogger()->info(__VA_ARGS__)	
#define XT_TRACE(...) ::XTEngine2d::Log::GetClientLogger()->trace(__VA_ARGS__)
#define XT_CRITICAL(...) ::XTEngine2d::Log::GetClientLogger()->critical(__VA_ARGS__)


