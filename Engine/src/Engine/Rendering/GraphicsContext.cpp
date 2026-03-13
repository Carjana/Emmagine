#include "GraphicsContext.h"

#include "Engine/Core.h"

namespace Emma
{
	GraphicsContext::GraphicsContext(SDL_Window *window)
	{
		Window = window;
	}

	void GraphicsContext::Init(GraphicsContextCreateInfo info)
	{
		SwapchainComposition = info.SwapchainComposition;
		PresentMode = info.PresentMode;
#ifdef DEBUG
		Device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, nullptr);
#elifdef RELEASE
		Device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, false, nullptr);
#endif

		CORE_ASSERT_MESSAGE(Device, "GPU Device Creation Failed!", SDL_GetError());
		CORE_ASSERT_FUNC(SDL_ClaimWindowForGPUDevice(Device, Window), "Failed to claim window for GPU Device!", SDL_GetError());

		SDL_SetGPUSwapchainParameters(Device, Window, SwapchainComposition, PresentMode);
	}

	void GraphicsContext::Destroy()
	{
		SDL_WaitForGPUIdle(Device);
		SDL_DestroyGPUDevice(Device);
	}
}
