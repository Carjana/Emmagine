#pragma once
#include "Engine/Rendering/GraphicsContext.h"

namespace Emma
{
	// TODO: Maybe have a Buffer Base class with static upload methods and only batch data here.
	// TODO: But remember: Maybe also allow upload here, because sometimes I dont want to upload all buffers everytime
	class VertexBuffer
	{
	public:
		void Create(GraphicsContext *context, Uint32 vertexBufferSize);

		void UploadDataToBuffer(const void *vertices, Uint32 verticesSize) const;

		void Bind(SDL_GPURenderPass *renderPass) const;

		void ReleaseBuffer() const;

		SDL_GPUBuffer *Buffer = nullptr;
		GraphicsContext *Context  = nullptr;
	};
}
