#pragma once

#ifdef PLATFORM_WINDOWS

#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)
#define DEBUG_BREAK __debugbreak();

#else
	#error "Platform not supported!"
#endif

#ifdef EMMA_DYNAMIC_LIBRARY
	#ifdef EMMA_BUILD_DLL
		#define EMMA_API DLL_EXPORT
	#else
		#define EMMA_API DLL_IMPORT
	#endif
#else
	#define EMMA_API
#endif

#if DEBUG
	#include "Engine/Logger.h"
	#define CORE_ASSERT_MESSAGE(condition, ...) if(!(condition)) {LOG_CORE_ERROR("Assertion failed! {0}", __VA_ARGS__) DEBUG_BREAK}
	#define CORE_ASSERT(condition) if(!(condition)) {LOG_CORE_ERROR("Assertion failed!") DEBUG_BREAK}
	#define CORE_ASSERT_FUNC(function, ...) CORE_ASSERT_MESSAGE((function), __VA_ARGS__)
	#define ASSERT_MESSAGE(condition, ...) if(!(condition)) {LOG_APP_ERROR("Assertion failed! {0}",__VA_ARGS__) DEBUG_BREAK}
	#define ASSERT(condition) if(!(condition)) {LOG_APP_ERROR("Assertion failed!") DEBUG_BREAK}
	#define ASSERT_FUNC(function, ...) ASSERT((function), __VA_ARGS__)
#else
	#define CORE_ASSERT_MESSAGE(condition, ...)
	#define CORE_ASSERT(condition, ...)
	#define CORE_ASSERT_FUNC(function, ...) function
	#define ASSERT_MESSAGE(condition, ...)
	#define ASSERT(condition, ...)
	#define ASSERT_FUNC(function, ...) function
#endif

#define BIT(x) (1 << x)

#define ArrayCount(array) (sizeof(array) / sizeof(array[0]))

#define BIND_EVENT(fn) std::bind(&fn, this, std::placeholders::_1)

// Custom fmt formatter to format type. Use "input" to refer to the input variable.
#define CUSTOM_FORMAT(type, ...)\
template <>\
struct fmt::formatter<type> {\
	constexpr auto parse(format_parse_context& ctx) {\
		return ctx.begin();\
	}\
\
	template <typename FormatContext>\
	auto format(const type& input, FormatContext& ctx) const {\
		return fmt::format_to(ctx.out(), __VA_ARGS__);\
	}\
};

#include "spdlog/fmt/bundled/base.h"
#include "Math/Mathdefines.h"