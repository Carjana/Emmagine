#include "ViewPortLayer.h"

namespace EmmaEditor
{
	void ViewPortLayer::OnAttach()
	{
		graphicsContext = Emma::EmmaApplication::GetInstance()->mainWindow->Context;
	}

	void ViewPortLayer::OnRenderImGui()
	{
		if (renderer == nullptr)
			renderer = GetService<Emma::Renderer>();

		ImGui::Begin("ViewPort");
		renderer->Begin();
		renderer->BatchTest();

		ImVec2 contentSize = ImGui::GetContentRegionAvail();

		if (contentSize.x <= 0.0f || contentSize.y <= 0.0f)
		{
			ImGui::End();
			return;
		}

		SDL_GPUTextureCreateInfo textureCreateInfo {};
		textureCreateInfo.type = SDL_GPU_TEXTURETYPE_2D;
		textureCreateInfo.format = SDL_GetGPUSwapchainTextureFormat(graphicsContext->Device, graphicsContext->Window);
		textureCreateInfo.width = (Uint32)contentSize.x;
		textureCreateInfo.height = (Uint32)contentSize.y;
		textureCreateInfo.layer_count_or_depth = 1;
		textureCreateInfo.num_levels = 1;
		textureCreateInfo.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER | SDL_GPU_TEXTUREUSAGE_COLOR_TARGET;

		testTexture = SDL_CreateGPUTexture(graphicsContext->Device, &textureCreateInfo);

		SDL_GPUCommandBuffer *CMDBuffer = SDL_AcquireGPUCommandBuffer(graphicsContext->Device);

		SDL_GPUColorTargetInfo targetInfo = {};
		targetInfo.texture = testTexture;
		targetInfo.clear_color = SDL_FColor { 0.45f, 0.55f, 0.60f, 1.00f};
		targetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
		targetInfo.store_op = SDL_GPU_STOREOP_STORE;
		targetInfo.mip_level = 0;
		targetInfo.layer_or_depth_plane = 0;
		targetInfo.cycle = false;


		SDL_GPURenderPass *renderPass = SDL_BeginGPURenderPass(CMDBuffer, &targetInfo, 1, nullptr);

		renderer->EndAndDrawToViewport(renderPass);

		SDL_EndGPURenderPass(renderPass);

		SDL_GPUFence *fence = SDL_SubmitGPUCommandBufferAndAcquireFence(CMDBuffer);

		SDL_WaitForGPUFences(graphicsContext->Device, true, &fence, 1);
		SDL_ReleaseGPUFence(graphicsContext->Device, fence);

		if (testTexture != nullptr)
		{
			SDL_GPUTexture *texture = testTexture;

			ImGui::Image(texture, ImVec2(contentSize.x, contentSize.y));
		}

		ImGui::End();
	}

	void ViewPortLayer::PostRenderImGui()
	{
		SDL_ReleaseGPUTexture(graphicsContext->Device, testTexture);
	}
}
