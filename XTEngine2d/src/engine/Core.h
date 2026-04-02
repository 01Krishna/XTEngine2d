#pragma once

#ifdef XT_PLATFORM_WINDOWS
	#ifdef XT_BUILD_DLL
		#define XT_API __declspec(dllexport)
	#else
		#define XT_API __declspec(dllimport)
	#endif // XT_BUILD_DLL

#else
	#error XTEngine2d only supports Windows!
#endif // XT_PLATFORM_WINDOWS
