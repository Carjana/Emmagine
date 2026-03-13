#pragma once
#include "GraphicsContext.h"
#include "Buffer/VertexBufferLayout.h"
#include "SDL3/SDL_gpu.h"
#include "SDL_shadercross/include/SDL3_shadercross/SDL_shadercross.h"

namespace Emma
{
	struct ShaderLoadInfo
	{
		const char *absolutePath;
		const char* entryPoint;
		SDL_ShaderCross_ShaderStage shaderStage;
	};

	struct GraphicsPipelineCreateInfo
	{
		ShaderLoadInfo VertexShaderInfo;
		ShaderLoadInfo FragmentShaderInfo;

		SDL_GPUTextureFormat ColorTargetFormat;
		VertexBufferLayout VertexBufferLayout;
	};

	class GraphicsPipeline
	{
	public:
		void CreateGraphicsPipeline(GraphicsContext *context, GraphicsPipelineCreateInfo *createInfo);

		void Bind(SDL_GPURenderPass *renderPass) const;

		void ReleasePipeline() const;

		SDL_GPUGraphicsPipeline *Pipeline = nullptr;
		int numColorTargets = 0;
	private:

		SDL_GPUShader *LoadShader(ShaderLoadInfo *info) const;

		GraphicsContext *Context = nullptr;
	};
}
