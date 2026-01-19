#pragma once
#include "SDL3/SDL_gpu.h"


namespace Emma
{
	struct GraphicsContextCreateInfo
	{
		SDL_GPUSwapchainComposition SwapchainComposition;
		SDL_GPUPresentMode PresentMode;
	};
	class GraphicsContext
	{
	public:
		GraphicsContext(SDL_Window *window);

		void Init(GraphicsContextCreateInfo info);
		void Destroy();

		SDL_Window *Window;
		SDL_GPUDevice *Device;

		SDL_GPUSwapchainComposition SwapchainComposition;
		SDL_GPUPresentMode PresentMode;
	};
}
