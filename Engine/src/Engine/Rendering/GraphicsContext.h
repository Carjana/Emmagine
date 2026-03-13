#pragma once
#include "SDL3/SDL_gpu.h"
#include "SDL3/SDL_render.h"


namespace Emma
{
	struct GraphicsContextCreateInfo
	{
		SDL_GPUSwapchainComposition SwapchainComposition;
		SDL_GPUPresentMode PresentMode;
	};

	struct WindowRect
	{
		unsigned int X;
		unsigned int Y;
		unsigned int Width;
		unsigned int Height;
	};

	class GraphicsContext
	{
	public:
		GraphicsContext(SDL_Window *window);

		void Init(GraphicsContextCreateInfo info);
		void Destroy();

		SDL_Window *Window;
		SDL_GPUDevice *Device;

		WindowRect WindowRect;

		SDL_GPUSwapchainComposition SwapchainComposition;
		SDL_GPUPresentMode PresentMode;
	};
}
