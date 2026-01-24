#pragma once
#include "Engine/EmmaApplication.h"

namespace Emma
{
	struct TempVertex
	{
		vec3 position;
		vec2 uv;
	};
	class TestRenderer
	{
	public:
		void Init();
		void Run();
	private:
		EmmaApplication *Instance = nullptr;
		GraphicsContext *Context = nullptr;

		SDL_GPUGraphicsPipeline *Pipeline = nullptr;
		SDL_GPUBuffer *VertexBuffer = nullptr;
		SDL_GPUBuffer *IndexBuffer = nullptr;
		SDL_GPUTexture *Texture = nullptr;
		SDL_GPUSampler *Samplers[6] = {};
	};
}