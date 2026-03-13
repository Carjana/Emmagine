#include "IndexBuffer.h"

#include "Engine/Core.h"
#include "SDL3/SDL_assert.h"

namespace Emma
{
	void IndexBuffer::Create(GraphicsContext *context, const Uint32 indexBufferSize)
	{
		Context = context;
		SDL_GPUBufferCreateInfo indexBufferCreateInfo {};
		indexBufferCreateInfo.size = indexBufferSize;
		indexBufferCreateInfo.usage = SDL_GPU_BUFFERUSAGE_INDEX;

		Buffer = SDL_CreateGPUBuffer(Context->Device, &indexBufferCreateInfo);
		CORE_ASSERT_MESSAGE(Buffer, SDL_GetError());
		SDL_SetGPUBufferName(Context->Device, Buffer, "IndexBuffer");
	}

	void IndexBuffer::UploadDataToBuffer(const Uint16 *indices, const Uint32 indicesCount) const
	{
		SDL_GPUTransferBufferCreateInfo indexTransferBufferCreateInfo {};
		indexTransferBufferCreateInfo.size = sizeof(Uint16) * indicesCount;
		indexTransferBufferCreateInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;

		// create index transferbuffer and map memory to it
		SDL_GPUTransferBuffer *indexTransferBuffer = SDL_CreateGPUTransferBuffer(Context->Device, &indexTransferBufferCreateInfo);
		CORE_ASSERT_MESSAGE(indexTransferBuffer, SDL_GetError());
		Uint8 *mappedMemory = (Uint8 *)SDL_MapGPUTransferBuffer(Context->Device, indexTransferBuffer, false);
		CORE_ASSERT_MESSAGE(mappedMemory != nullptr, SDL_GetError());

		SDL_memcpy(mappedMemory, indices, sizeof(Uint16) * indicesCount);

		SDL_UnmapGPUTransferBuffer(Context->Device, indexTransferBuffer);

		// upload buffer to GPU
		SDL_GPUCommandBuffer *cmdBuffer = SDL_AcquireGPUCommandBuffer(Context->Device);
		CORE_ASSERT_MESSAGE(cmdBuffer, SDL_GetError());

		SDL_GPUCopyPass *copyPass = SDL_BeginGPUCopyPass(cmdBuffer);

		SDL_GPUTransferBufferLocation indexBufferLocation {};
		indexBufferLocation.offset = 0;
		indexBufferLocation.transfer_buffer = indexTransferBuffer;

		SDL_GPUBufferRegion indexGPUBufferRegion {};
		indexGPUBufferRegion.buffer = Buffer;
		indexGPUBufferRegion.offset = 0;
		indexGPUBufferRegion.size = sizeof(Uint16) * indicesCount;

		SDL_UploadToGPUBuffer(copyPass, &indexBufferLocation, &indexGPUBufferRegion, false);

		SDL_EndGPUCopyPass(copyPass);

		CORE_ASSERT_FUNC(SDL_SubmitGPUCommandBuffer(cmdBuffer), SDL_GetError());

		SDL_ReleaseGPUTransferBuffer(Context->Device, indexTransferBuffer);
	}

	void IndexBuffer::Bind(SDL_GPURenderPass *renderPass) const
	{
		CORE_ASSERT(renderPass);
		SDL_GPUBufferBinding indexBufferBinding;
		indexBufferBinding.buffer = Buffer;
		indexBufferBinding.offset = 0;

		SDL_BindGPUIndexBuffer(renderPass, &indexBufferBinding, SDL_GPU_INDEXELEMENTSIZE_16BIT);
	}

	void IndexBuffer::ReleaseBuffer() const
	{
		SDL_ReleaseGPUBuffer(Context->Device, Buffer);
	}
}
