#pragma once

#ifdef PLATFORM_WINDOWS

#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)

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