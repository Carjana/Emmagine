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

		//Device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL, true, nullptr);
		Device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, nullptr);
		CORE_ASSERT_MESSAGE(Device, "GPU Device Creation Failed!", SDL_GetError());
		CORE_ASSERT_FUNC(SDL_ClaimWindowForGPUDevice(Device, Window), "Failed to claim window for GPU Device!", SDL_GetError());

		SDL_SetGPUSwapchainParameters(Device, Window, SwapchainComposition, PresentMode);
	}

	void GraphicsContext::Destroy()
	{

	}
}
