#include "GraphicsPipeline.h"

#include "EmmaVertex.h"
#include "Engine/Logger.h"
#include "SDL_shadercross/include/SDL3_shadercross/SDL_shadercross.h"

namespace Emma
{
	void GraphicsPipeline::CreateGraphicsPipeline(GraphicsContext *context, GraphicsPipelineCreateInfo *createInfo)
	{
		Context = context;

		SDL_GPUShader *vertexShader = LoadShader(&createInfo->VertexShaderInfo);
		SDL_GPUShader *fragmentShader = LoadShader(&createInfo->FragmentShaderInfo);

		SDL_GPUColorTargetDescription colorTargetDescription = {};
		colorTargetDescription.format = createInfo->ColorTargetFormat;
		SDL_GPUColorTargetDescription colorTargetDescriptions[] = {colorTargetDescription};

		numColorTargets = ArrayCount(colorTargetDescriptions);

		SDL_GPUGraphicsPipelineTargetInfo pipelineTargetInfo = {};
		pipelineTargetInfo.color_target_descriptions = colorTargetDescriptions;
		pipelineTargetInfo.num_color_targets = numColorTargets;

		std::vector<SDL_GPUVertexAttribute> vertexAttributes(createInfo->VertexBufferLayout.Elements.size());
		for (Uint32 i = 0; i < createInfo->VertexBufferLayout.Elements.size(); ++i)
		{
			vertexAttributes[i] = {i, 0, VertexBufferLayout::TypeToSDLFormat(createInfo->VertexBufferLayout.Elements[i].Type), createInfo->VertexBufferLayout.Elements[i].Offset};
		}

		SDL_GPUVertexBufferDescription vertexBufferDescriptions[] =
		{
			{0, createInfo->VertexBufferLayout.Size, SDL_GPU_VERTEXINPUTRATE_VERTEX}
		};

		SDL_GPUVertexInputState vertexInputState = {};
		vertexInputState.vertex_attributes = vertexAttributes.data();
		vertexInputState.num_vertex_attributes = (Uint32)vertexAttributes.size();
		vertexInputState.vertex_buffer_descriptions = vertexBufferDescriptions;
		vertexInputState.num_vertex_buffers = ArrayCount(vertexBufferDescriptions);

		SDL_GPUGraphicsPipelineCreateInfo pipelineInfo {};
		pipelineInfo.vertex_shader = vertexShader;
		pipelineInfo.fragment_shader = fragmentShader;
		pipelineInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;

		pipelineInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
		pipelineInfo.rasterizer_state.cull_mode = SDL_GPU_CULLMODE_NONE;

		pipelineInfo.target_info = pipelineTargetInfo;

		pipelineInfo.vertex_input_state = vertexInputState;

		Pipeline = SDL_CreateGPUGraphicsPipeline(Context->Device, &pipelineInfo);
		CORE_ASSERT_MESSAGE(Pipeline, SDL_GetError());

		SDL_ReleaseGPUShader(Context->Device, vertexShader);
		SDL_ReleaseGPUShader(Context->Device, fragmentShader);
	}

	void GraphicsPipeline::Bind(SDL_GPURenderPass *renderPass) const
	{
		SDL_BindGPUGraphicsPipeline(renderPass, Pipeline);
	}

	SDL_GPUShader * GraphicsPipeline::LoadShader(ShaderLoadInfo *info) const
	{
		// TODO: save compiled Code and check if it exists before compiling again
		size_t hlslSize;
		void* hlslCode = SDL_LoadFile(info->absolutePath, &hlslSize);
		SDL_ShaderCross_HLSL_Info hlslInfo {};
		hlslInfo.source = (const char*)hlslCode;
		hlslInfo.entrypoint = info->entryPoint;
		hlslInfo.shader_stage = info->shaderStage;

		size_t codeSize;
		void* spirvCode = SDL_ShaderCross_CompileSPIRVFromHLSL(&hlslInfo, &codeSize);
		if (spirvCode == NULL)
			LOG_CORE_ERROR(SDL_GetError());

		SDL_ShaderCross_SPIRV_Info spirvInfo = {};
		spirvInfo.bytecode = (const Uint8*)spirvCode;
		spirvInfo.bytecode_size = codeSize;
		spirvInfo.entrypoint = info->entryPoint;
		spirvInfo.shader_stage = info->shaderStage;

		SDL_PropertiesID propertiesId {};
		SDL_ShaderCross_GraphicsShaderResourceInfo resourceInfo {};
		SDL_ShaderCross_GraphicsShaderMetadata *metaData = SDL_ShaderCross_ReflectGraphicsSPIRV((Uint8*)spirvCode, codeSize, propertiesId);
		resourceInfo = metaData->resource_info;

		SDL_GPUShader *vertexShader = SDL_ShaderCross_CompileGraphicsShaderFromSPIRV(Context->Device, &spirvInfo, &resourceInfo, propertiesId);

		SDL_free(metaData);
		SDL_free(spirvCode);
		SDL_free(hlslCode);

		CORE_ASSERT_MESSAGE(vertexShader, SDL_GetError());

		return vertexShader;
	}

	void GraphicsPipeline::ReleasePipeline() const
	{
		SDL_ReleaseGPUGraphicsPipeline(Context->Device, Pipeline);
	}
}
