#pragma once

#ifdef PLATFORM_WINDOWS

#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)
#define DEBUG_BREAK __debugbreak();

#else
	#error "Platform not supported!"
#endif

#ifdef EMMA_BUILD_DLL
	#define EMMA_API DLL_EXPORT
#else
	#define EMMA_API DLL_IMPORT
#endif

#define BIT(x) (1 << x)

#define ArrayCount(array) (sizeof(array) / sizeof(array[0]))


#if DEBUG
	#define ASSERT(condition, ...) if(!condition) {LOG_INFO("Assertion failed! {0}", __VA_ARGS__); DEBUG_BREAK}
#else
	#define assert(condition, ...)
#endif