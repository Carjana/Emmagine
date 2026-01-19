#include "ImGuiLayer.h"

#include "Emmagine.h"

#include "imgui.h"

#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlgpu3.h"

namespace Emma
{
	void ImGuiLayer::OnAttach()
	{
		const EmmaApplication *app = EmmaApplication::GetInstance();
		context = app->mainWindow->Context;

		LOG_CORE_INFO("Creating ImGui Context")
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		const float mainScale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());

		ImGuiStyle &style = ImGui::GetStyle();
		style.ScaleAllSizes(mainScale);
		style.FontScaleDpi = mainScale;
		io.ConfigDpiScaleFonts = true;
		io.ConfigDpiScaleViewports = true;

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplSDL3_InitForSDLGPU(context->Window);
		ImGui_ImplSDLGPU3_InitInfo initInfo = {};

		initInfo.Device = context->Device;
		initInfo.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(context->Device, context->Window);
		initInfo.MSAASamples = SDL_GPU_SAMPLECOUNT_1;
		initInfo.SwapchainComposition = context->SwapchainComposition;
		initInfo.PresentMode = context->PresentMode;
		ImGui_ImplSDLGPU3_Init(&initInfo);
		LOG_CORE_INFO("Im Gui Context created")

		io.DisplaySize = ImVec2((float)app->mainWindow->Width, (float)app->mainWindow->Height);
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplSDL3_Shutdown();
		ImGui_ImplSDLGPU3_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnRenderImGui()
	{
		ImGui::ShowDemoWindow();
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplSDLGPU3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Render();
		ImDrawData* drawData = ImGui::GetDrawData();
		const bool isMinimized = (drawData->DisplaySize.x <= 0.0f || drawData->DisplaySize.y <= 0.0f);

		SDL_GPUCommandBuffer *commandBuffer = SDL_AcquireGPUCommandBuffer(context->Device);

		SDL_GPUTexture *swapchainTexture;
		SDL_WaitAndAcquireGPUSwapchainTexture(commandBuffer, context->Window, &swapchainTexture, nullptr, nullptr);

		if (swapchainTexture != nullptr && !isMinimized)
		{
			ImGui_ImplSDLGPU3_PrepareDrawData(drawData, commandBuffer);

			SDL_GPUColorTargetInfo targetInfo = {};
			targetInfo.texture = swapchainTexture;
			targetInfo.clear_color = SDL_FColor { 0.45f, 0.55f, 0.60f, 1.00f};
			targetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
			targetInfo.store_op = SDL_GPU_STOREOP_STORE;
			targetInfo.mip_level = 0;
			targetInfo.layer_or_depth_plane = 0;
			targetInfo.cycle = false;
			SDL_GPURenderPass *renderPass = SDL_BeginGPURenderPass(commandBuffer, &targetInfo, 1, nullptr);

			ImGui_ImplSDLGPU3_RenderDrawData(drawData, commandBuffer, renderPass);

			SDL_EndGPURenderPass(renderPass);
		}

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		SDL_SubmitGPUCommandBuffer(commandBuffer);
	}

	void ImGuiLayer::HandleSDLEvent(SDL_Event &event)
	{
		ImGui_ImplSDL3_ProcessEvent(&event);
	}
}
