#include "VertexBuffer.h"
#include "Engine/Core.h"

namespace Emma
{
	void VertexBuffer::Create(GraphicsContext *context, const Uint32 vertexBufferSize)
	{
		Context = context;

		SDL_GPUBufferCreateInfo vertexBufferCreateInfo {};
		vertexBufferCreateInfo.size = vertexBufferSize;
		vertexBufferCreateInfo.usage = SDL_GPU_BUFFERUSAGE_VERTEX;

		Buffer = SDL_CreateGPUBuffer(Context->Device, &vertexBufferCreateInfo);
		CORE_ASSERT_MESSAGE(Buffer, SDL_GetError());
		SDL_SetGPUBufferName(Context->Device, Buffer, "VertexBuffer");
	}

	void VertexBuffer::UploadDataToBuffer(const void *vertices, const Uint32 verticesSize) const
	{
		SDL_GPUTransferBufferCreateInfo vertexTransferBufferCreateInfo {};
		vertexTransferBufferCreateInfo.size = verticesSize;
		vertexTransferBufferCreateInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;

		// create vertex transferbuffer and map memory to it
		SDL_GPUTransferBuffer *vertexTransferBuffer = SDL_CreateGPUTransferBuffer(Context->Device, &vertexTransferBufferCreateInfo);
		CORE_ASSERT_MESSAGE(vertexTransferBuffer, SDL_GetError());
		Uint8 *mappedMemory = (Uint8 *)SDL_MapGPUTransferBuffer(Context->Device, vertexTransferBuffer, false);
		CORE_ASSERT_MESSAGE(mappedMemory != nullptr, SDL_GetError());

		SDL_memcpy(mappedMemory, vertices, verticesSize);

		SDL_UnmapGPUTransferBuffer(Context->Device, vertexTransferBuffer);

		// upload buffer to GPU
		SDL_GPUCommandBuffer *cmdBuffer = SDL_AcquireGPUCommandBuffer(Context->Device);
		CORE_ASSERT_MESSAGE(cmdBuffer, SDL_GetError());

		SDL_GPUCopyPass *copyPass = SDL_BeginGPUCopyPass(cmdBuffer);

		SDL_GPUTransferBufferLocation vertexBufferLocation {};
		vertexBufferLocation.offset = 0;
		vertexBufferLocation.transfer_buffer = vertexTransferBuffer;

		SDL_GPUBufferRegion vertexGPUBufferRegion {};
		vertexGPUBufferRegion.buffer = Buffer;
		vertexGPUBufferRegion.offset = 0;
		vertexGPUBufferRegion.size = verticesSize;

		SDL_UploadToGPUBuffer(copyPass, &vertexBufferLocation, &vertexGPUBufferRegion, false);

		SDL_EndGPUCopyPass(copyPass);

		CORE_ASSERT_FUNC(SDL_SubmitGPUCommandBuffer(cmdBuffer), SDL_GetError());

		SDL_ReleaseGPUTransferBuffer(Context->Device, vertexTransferBuffer);
	}

	void VertexBuffer::Bind(SDL_GPURenderPass *renderPass) const
	{
		SDL_GPUBufferBinding bufferBindings[]
		{
			{Buffer, 0}
		};

		SDL_BindGPUVertexBuffers(renderPass, 0, bufferBindings, ArrayCount(bufferBindings));
	}

	void VertexBuffer::ReleaseBuffer() const
	{
		SDL_ReleaseGPUBuffer(Context->Device, Buffer);
	}
}
