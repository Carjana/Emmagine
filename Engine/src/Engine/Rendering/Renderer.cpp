#include "Renderer.h"

namespace Emma
{
	void Renderer::CreateBuffersAndPipeline(GraphicsContext *context)
	{
		Context = context;
		SDL_ShaderCross_Init();

		Pipeline = new GraphicsPipeline();
		GraphicsPipelineCreateInfo info = {};
		ShaderLoadInfo vertexShaderLoadInfo = {};
		vertexShaderLoadInfo.entryPoint = "main";
		vertexShaderLoadInfo.absolutePath = "D:/OtherProjects/Emmagine/Engine/src/Engine/Rendering/DefaultShader/SimpleColor.vert.hlsl";
		vertexShaderLoadInfo.shaderStage = SDL_SHADERCROSS_SHADERSTAGE_VERTEX;

		ShaderLoadInfo fragmentShaderLoadInfo = {};
		fragmentShaderLoadInfo.entryPoint = "main";
		fragmentShaderLoadInfo.absolutePath = "D:/OtherProjects/Emmagine/Engine/src/Engine/Rendering/DefaultShader/SimpleColor.frag.hlsl";
		fragmentShaderLoadInfo.shaderStage = SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT;

		info.VertexShaderInfo = vertexShaderLoadInfo;
		info.FragmentShaderInfo = fragmentShaderLoadInfo;
		info.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(Context->Device, Context->Window);
		EmmaVertex::FillVertexBufferLayout(info.VertexBufferLayout);
		Pipeline->CreateGraphicsPipeline(Context, &info);

		VertexBuffer = new Emma::VertexBuffer();
		VertexBuffer->Create(Context, sizeof(EmmaVertex) * 1024);

		IndexBuffer = new Emma::IndexBuffer();
		IndexBuffer->Create(Context, sizeof(EmmaVertex) * 1024);
	}

	void Renderer::Begin()
	{
		nBatchedIndices = 0;
		nBatchedVertices = 0;
		nBatchedObjects = 0;
	}

	void Renderer::EndAndDrawToSwapchain()
	{
		if (nBatchedVertices <= 0 || nBatchedIndices <= 0)
			return;

		SDL_GPUCommandBuffer *commandBuffer = SDL_AcquireGPUCommandBuffer(Context->Device);

		SDL_GPUTexture *swapChainTexture;
		CORE_ASSERT_FUNC(SDL_WaitAndAcquireGPUSwapchainTexture(commandBuffer, Context->Window, &swapChainTexture, nullptr, nullptr), SDL_GetError());
		if (swapChainTexture)
		{
			SDL_GPUColorTargetInfo colorTargetInfo {};
			colorTargetInfo.texture = swapChainTexture;
			colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;
			colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
			colorTargetInfo.clear_color = SDL_FColor{0.2f,0.1f,0.3f,1.0f};

			SDL_GPUColorTargetInfo colorTargetInfos[] =
			{
				colorTargetInfo
			};
			CORE_ASSERT_MESSAGE(Pipeline->numColorTargets == ArrayCount(colorTargetInfos), "This must match!");
			SDL_GPURenderPass *renderPass = SDL_BeginGPURenderPass(commandBuffer, colorTargetInfos, Pipeline->numColorTargets, nullptr);

			SDL_GPUViewport viewPort = {};
			viewPort.x = 0;
			viewPort.y = 0;
			viewPort.w = Context->WindowRect.Width;
			viewPort.h = Context->WindowRect.Height;

			EndAndDrawToViewport(renderPass, &viewPort);

			SDL_EndGPURenderPass(renderPass);
		}


		SDL_SubmitGPUCommandBuffer(commandBuffer);
	}

	void Renderer::EndAndDrawToViewport(SDL_GPURenderPass *renderPass, SDL_GPUViewport *viewPort)
	{
		if (nBatchedVertices <= 0 || nBatchedIndices <= 0)
			return;

		VertexBuffer->UploadDataToBuffer(BatchedVertices, sizeof(EmmaVertex) * nBatchedVertices);
		IndexBuffer->UploadDataToBuffer(BatchedIndices, nBatchedIndices);

		if (viewPort)
			SDL_SetGPUViewport(renderPass, viewPort);

		Pipeline->Bind(renderPass);

		VertexBuffer->Bind(renderPass);

		IndexBuffer->Bind(renderPass);

		for (int i = 0;
			i < nBatchedObjects;
			++i)
		{
			int numIndices = 0;
			if (i == nBatchedObjects - 1)
				numIndices = nBatchedIndices - IndicesOffset[i];
			else
				numIndices = IndicesOffset[i + 1] - IndicesOffset[i];

			SDL_DrawGPUIndexedPrimitives(renderPass, numIndices, 1, IndicesOffset[i], 0, 0);

		}
	}

	void Renderer::Batch(EmmaVertex *vertices, int nVertices, Uint16 *indices, int nIndices)
	{
		for (int i = 0;
			i < nVertices;
			++i)
		{
			BatchedVertices[nBatchedVertices + i] = vertices[i];
		}

		for (int i = 0;
			i < nIndices;
			++i)
		{
			BatchedIndices[nBatchedIndices + i] = nBatchedVertices + indices[i];
		}


		IndicesOffset[nBatchedObjects] = nBatchedIndices;

		nBatchedIndices += nIndices;
		nBatchedVertices += nVertices;
		nBatchedObjects++;
	}

	void Renderer::Shutdown()
	{
		VertexBuffer->ReleaseBuffer();
		IndexBuffer->ReleaseBuffer();
		Pipeline->ReleasePipeline();
	}
}
