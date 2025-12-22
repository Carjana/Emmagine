#include "ImGuiLayer.h"

#include "imgui.h"
#include "SDL_time.h"
#include "backends/imgui_impl_sdl3.h"
#include "Engine/EmmaApplication.h"
#include "Engine/Logger.h"
#include "Engine/Events/MouseEvents.h"

#include "ImGui/ImGuiSDL3GPU.h"

namespace EmmaEditor
{
	void ImGuiLayer::OnAttach()
	{
		LOG_INFO("Creating ImGui Context")
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		app = Emma::EmmaApplication::GetInstance();
		Emma::EmmaWindow *window = app->mainWindow;

		float mainScale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
		ImGuiStyle& style = ImGui::GetStyle();
		style.ScaleAllSizes(mainScale);
		style.FontScaleDpi = mainScale;

		ImGui_ImplSDL3_InitForSDLGPU(window->Window);

		ImGui_ImplSDLGPU3_InitInfo init_info;
		init_info.Device = window->GPUDevice;
		init_info.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(window->GPUDevice, window->Window);
		init_info.MSAASamples = SDL_GPU_SAMPLECOUNT_1;                      // Only used in multi-viewports mode.
		init_info.SwapchainComposition = SDL_GPU_SWAPCHAINCOMPOSITION_SDR;  // Only used in multi-viewports mode.
		init_info.PresentMode = SDL_GPU_PRESENTMODE_VSYNC;
		ImGui_ImplSDLGPU3_Init(&init_info);
		LOG_INFO("Im Gui Context created")

	}

	void ImGuiLayer::OnDetach()
	{

	}

	void ImGuiLayer::OnEvent(Emma::Event &event)
	{
		ImGuiIO io = ImGui::GetIO();

		if (!(event.GetCategoryFlags() & (int)Emma::EventCategory::Mouse))
			return;

		switch (event.GetEventType())
		{
			case Emma::EventType::MouseMovedEvent:
			{
				Emma::MouseMovedEvent &mouseEvent = (Emma::MouseMovedEvent&)event;
				io.AddMouseSourceEvent(ImGuiMouseSource_Mouse);
				io.AddMousePosEvent(mouseEvent.X, mouseEvent.Y);
			}break;
			case Emma::EventType::MouseButtonEvent:
			{
				Emma::MouseButtonEvent &mouseEvent = (Emma::MouseButtonEvent&)event;
				int mouse_button = -1;
				if (mouseEvent.ButtonID == Emma::MouseButton::Left) { mouse_button = 0; }
				if (mouseEvent.ButtonID == Emma::MouseButton::Right) { mouse_button = 1; }
				if (mouseEvent.ButtonID == Emma::MouseButton::Middle) { mouse_button = 2; }
				if (mouseEvent.ButtonID == Emma::MouseButton::X1) { mouse_button = 3; }
				if (mouseEvent.ButtonID == Emma::MouseButton::X2) { mouse_button = 4; }
				if (mouse_button == -1)
					break;
				io.AddMouseSourceEvent(ImGuiMouseSource_Mouse);
				io.AddMouseButtonEvent(mouse_button, mouseEvent.IsPressed);
			}break;
			case Emma::EventType::MouseWheelEvent:
			{
				Emma::MouseWheelEvent &mouseEvent = (Emma::MouseWheelEvent&)event;
				io.AddMouseSourceEvent(ImGuiMouseSource_Mouse);
				io.AddMouseWheelEvent(mouseEvent.X, mouseEvent.Y);
			}break;
		}
		event.IsHandled = io.WantCaptureMouse;
	}

	void ImGuiLayer::OnUpdate()
	{
		ImGui_ImplSDLGPU3_NewFrame();
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)app->mainWindow->Width, (float)app->mainWindow->Height);


		SDL_Time newTime;
		SDL_GetCurrentTime(&newTime);
		io.DeltaTime = time > 0.0f ? (newTime - time) : (1.0f / 60.0f);
		time = newTime;


		ImGui::NewFrame();

		ImGui::ShowDemoWindow();

		ImGui::Render();

		ImDrawData *drawData = ImGui::GetDrawData();

		Emma::EmmaWindow *window = app->mainWindow;

		SDL_GPUCommandBuffer* command_buffer = SDL_AcquireGPUCommandBuffer(window->GPUDevice); // Acquire a GPU command buffer

		SDL_GPUTexture* swapchain_texture;
		SDL_WaitAndAcquireGPUSwapchainTexture(command_buffer, window->Window, &swapchain_texture, nullptr, nullptr); // Acquire a swapchain texture

		if (swapchain_texture != nullptr)
		{
			// This is mandatory: call ImGui_ImplSDLGPU3_PrepareDrawData() to upload the vertex/index buffer!
			ImGui_ImplSDLGPU3_PrepareDrawData(drawData, command_buffer);

			// Setup and start a render pass
			SDL_GPUColorTargetInfo target_info = {};
			target_info.texture = swapchain_texture;
			target_info.clear_color = SDL_FColor { 0.45f, 0.55f, 0.60f, 1.00f};
			target_info.load_op = SDL_GPU_LOADOP_CLEAR;
			target_info.store_op = SDL_GPU_STOREOP_STORE;
			target_info.mip_level = 0;
			target_info.layer_or_depth_plane = 0;
			target_info.cycle = false;
			SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(command_buffer, &target_info, 1, nullptr);

			// Render ImGui
			ImGui_ImplSDLGPU3_RenderDrawData(drawData, command_buffer, renderPass);

			SDL_EndGPURenderPass(renderPass);
		}

		// Submit the command buffer
		SDL_SubmitGPUCommandBuffer(command_buffer);
	}
}
