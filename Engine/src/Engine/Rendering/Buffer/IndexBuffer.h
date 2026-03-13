#pragma once
#include "Engine/Rendering/GraphicsContext.h"
#include "SDL3/SDL_gpu.h"

namespace Emma
{
	// TODO: Maybe have a Buffer Base class with static upload methods and only batch data here.
	// TODO: But remember: Maybe also allow upload here, because sometimes I dont want to upload all buffers everytime
	class IndexBuffer
	{
	public:
		void Create(GraphicsContext *context, Uint32 indexBufferSize);

		void UploadDataToBuffer(const Uint16 *indices, Uint32 indicesCount) const;

		void Bind(SDL_GPURenderPass *renderPass) const;

		void ReleaseBuffer() const;

		SDL_GPUBuffer *Buffer = nullptr;
		GraphicsContext *Context  = nullptr;
	};
}
