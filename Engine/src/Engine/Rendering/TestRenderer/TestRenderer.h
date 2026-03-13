#pragma once
#include "Engine/EmmaApplication.h"
#include "Engine/Rendering/GraphicsPipeline.h"
#include "Engine/Rendering/Buffer/IndexBuffer.h"
#include "Engine/Rendering/Buffer/VertexBuffer.h"

namespace Emma
{
	class TestRenderer
	{
	public:
		void Init();
		void Run();

		void Shutdown();

	private:
		EmmaApplication *Instance = nullptr;
		GraphicsContext *Context = nullptr;

		GraphicsPipeline *Pipeline = nullptr;
		VertexBuffer *VertexBuffer = nullptr;
		IndexBuffer *IndexBuffer = nullptr;
		SDL_GPUTexture *Texture = nullptr;
		SDL_GPUSampler *Samplers[6] = {};
	};
}
