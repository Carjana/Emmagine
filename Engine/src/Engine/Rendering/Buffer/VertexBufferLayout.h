#pragma once

#include "emmapch.h"
#include "Engine/Core.h"
#include "SDL3/SDL_stdinc.h"

namespace Emma
{
	enum class VertexBufferType
	{
		None = 0,
		Float,
		Float2,
		Float3,
		Float4
	};

	struct VertexBufferElement
	{
		std::string Name;
		VertexBufferType Type;
		Uint32 Offset;
		VertexBufferElement(std::string name, const VertexBufferType type, const Uint32 offset) : Name(std::move(name)), Type(type), Offset(offset) {}
	};

	class VertexBufferLayout
	{
	public:
		void AddElement(const std::string &name, const VertexBufferType type)
		{
			Elements.emplace_back(name, type, Size);
			Size += TypeToSize(type);
		}

		Uint32 Size = 0;
		std::vector<VertexBufferElement> Elements;

		static Uint32 TypeToSize(const VertexBufferType type)
		{
			switch (type)
			{
				case VertexBufferType::None:	return 0;
				case VertexBufferType::Float:	return sizeof(float);
				case VertexBufferType::Float2:	return sizeof(float) * 2;
				case VertexBufferType::Float3:	return sizeof(float) * 3;
				case VertexBufferType::Float4:	return sizeof(float) * 4;
				default:
				{
					CORE_ASSERT_MESSAGE(false, "Unknown VertexBufferType");
					return 0;
				}
			}
		}
		static SDL_GPUVertexElementFormat TypeToSDLFormat(const VertexBufferType type)
		{
			switch (type)
			{
				case VertexBufferType::Float:	return SDL_GPU_VERTEXELEMENTFORMAT_FLOAT;
				case VertexBufferType::Float2:	return SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
				case VertexBufferType::Float3:	return SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
				case VertexBufferType::Float4:	return SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4;
				case VertexBufferType::None:
				default:
				{
					CORE_ASSERT_MESSAGE(false, "Unknown VertexBufferType");
					return SDL_GPU_VERTEXELEMENTFORMAT_INVALID;
				}
			}
		}
	};
}
