#pragma once
#include "Engine/Rendering/GraphicsPipeline.h"
#include "Engine/Rendering/Buffer/IndexBuffer.h"
#include "Engine/Rendering/Buffer/VertexBuffer.h"
#include "Engine/Rendering/EmmaVertex.h"
#include "Engine/ServiceProvider/ServiceProvider.h"

namespace Emma
{

	class Renderer : public IService
	{
	public:
		void CreateBuffersAndPipeline(GraphicsContext *context);

		void Begin(); // TODO: Take in scene parameters
		void EndAndDrawToSwapchain(); // TODO: Submit the batch
		void EndAndDrawToViewport(SDL_GPURenderPass *renderPass, SDL_GPUViewport *viewPort = nullptr);

		void Batch(EmmaVertex *vertices, int nVertices, Uint16 *indices, int nIndices); // TODO: Batch meshes together;
		void Shutdown();

		void BatchTest()
		{
			EmmaVertex vertices_1[] =
			{
				{{-0.5f,-0.5f,0.0f,}, {0,0}, {1,0,0}},
				{{0.0f,0.5f,0.0f,}, {0,0}, {1,0,0}},
				{{0.5f,-0.5f,0.0f,}, {0,0}, {1,0,0}},
			};

			EmmaVertex vertices_2[] =
			{
				{{-1.0f,-1.0f,0.0f,}, {0,0}, {0,0,1}},
				{{-1.0f,1.0f,0.0f,}, {0,0}, {0,0,1}},
				{{0.0f,0.0f,0.0f,}, {0,0}, {0,0,1}},
			};

			Uint16 indices_1[] =
			{
				0, 1, 2
				};

			Batch(vertices_1, ArrayCount(vertices_1), indices_1, ArrayCount(indices_1));
			Batch(vertices_2, ArrayCount(vertices_2), indices_1, ArrayCount(indices_1));
		};

	private:
		GraphicsContext *Context = nullptr;
		GraphicsPipeline *Pipeline = nullptr;

		VertexBuffer* VertexBuffer = nullptr;
		IndexBuffer* IndexBuffer = nullptr;

		EmmaVertex BatchedVertices[1024] = {};
		int nBatchedVertices = 0;

		Uint16 BatchedIndices[1024] = {};
		int nBatchedIndices = 0;

		static constexpr int MaxBatchedObjects = 100;

		int nBatchedObjects = 0;
		int IndicesOffset[MaxBatchedObjects] = {};

		SDL_Window *window = nullptr;
	};
}
