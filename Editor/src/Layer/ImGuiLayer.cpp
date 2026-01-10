#include "ImGuiLayer.h"

#include "imgui.h"
#include "SDL_time.h"
#include "backends/imgui_impl_sdl3.h"
#include "Emmagine.h"

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
		const Emma::EmmaWindow *window = app->mainWindow;

		const float mainScale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
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

		io.DisplaySize = ImVec2((float)app->mainWindow->Width, (float)app->mainWindow->Height);
	}

	void ImGuiLayer::OnDetach()
	{

	}

	void ImGuiLayer::OnEvent(Emma::Event &event)
	{
		ImGuiIO io = ImGui::GetIO();

		Emma::EventDispatcher dispatcher(event);
		dispatcher.DispatchEvent<Emma::MouseMovedEvent>(BIND_EVENT(ImGuiLayer::OnMouseMovedEvent));
		dispatcher.DispatchEvent<Emma::MouseButtonEvent>(BIND_EVENT(ImGuiLayer::OnMouseButtonEvent));
		dispatcher.DispatchEvent<Emma::MouseWheelEvent>(BIND_EVENT(ImGuiLayer::OnMouseWheelEvent));
		dispatcher.DispatchEvent<Emma::KeyEvent>(BIND_EVENT(ImGuiLayer::OnKeyEvent));
		dispatcher.DispatchEvent<Emma::KeyTextEvent>(BIND_EVENT(ImGuiLayer::OnKeyTextEvent));
		dispatcher.DispatchEvent<Emma::WindowResizeEvent>(BIND_EVENT(ImGuiLayer::OnWindowResizeEvent));
	}

	bool ImGuiLayer::OnMouseMovedEvent(const Emma::MouseMovedEvent &event)
	{
		ImGuiIO &io = ImGui::GetIO();
		io.AddMouseSourceEvent(ImGuiMouseSource_Mouse);
		io.AddMousePosEvent(event.X, event.Y);
		return false;
	}

	bool ImGuiLayer::OnMouseButtonEvent(const Emma::MouseButtonEvent &event)
	{
		ImGuiIO &io = ImGui::GetIO();
		int mouse_button = -1;
		if (event.ButtonID == SDL_BUTTON_MASK(SDL_BUTTON_LEFT)) { mouse_button = 0; }
		if (event.ButtonID == SDL_BUTTON_MASK(SDL_BUTTON_RIGHT)) { mouse_button = 1; }
		if (event.ButtonID == SDL_BUTTON_MASK(SDL_BUTTON_MIDDLE)) { mouse_button = 2; }
		if (event.ButtonID == SDL_BUTTON_MASK(SDL_BUTTON_X1)){ mouse_button = 3; }
		if (event.ButtonID == SDL_BUTTON_MASK(SDL_BUTTON_X2)) { mouse_button = 4; }
		if (mouse_button == -1)
			return false;
		io.AddMouseSourceEvent(ImGuiMouseSource_Mouse);
		io.AddMouseButtonEvent(mouse_button, event.IsPressed);
		return false;
	}

	bool ImGuiLayer::OnMouseWheelEvent(const Emma::MouseWheelEvent &event)
	{
		ImGuiIO &io = ImGui::GetIO();
		io.AddMouseSourceEvent(ImGuiMouseSource_Mouse);
		io.AddMouseWheelEvent(event.X, event.Y);
		return false;
	}

	bool ImGuiLayer::OnKeyEvent(const Emma::KeyEvent &event)
	{
		ImGuiIO &io = ImGui::GetIO();

		io.AddKeyEvent(ImGuiMod_Ctrl, (event.Keymod & SDL_KMOD_CTRL) != 0);
		io.AddKeyEvent(ImGuiMod_Shift, (event.Keymod & SDL_KMOD_SHIFT) != 0);
		io.AddKeyEvent(ImGuiMod_Alt, (event.Keymod & SDL_KMOD_ALT) != 0);
		io.AddKeyEvent(ImGuiMod_Super, (event.Keymod & SDL_KMOD_GUI) != 0);

		const ImGuiKey key = SDLKeyToImGUIKey(event.Keycode);
		io.AddKeyEvent(key, event.IsPressed);
		io.SetKeyEventNativeData(key, (int)event.Keycode, (int)event.Scancode, (int)event.Scancode);
		return false;
	}

	bool ImGuiLayer::OnWindowResizeEvent(const Emma::WindowResizeEvent &event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)event.Width, (float)event.Height);
		return false;
	}

	bool ImGuiLayer::OnKeyTextEvent(const Emma::KeyTextEvent &event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddInputCharactersUTF8(event.Text);
		return false;
	}

	void ImGuiLayer::OnUpdate()
	{
		ImGui_ImplSDLGPU3_NewFrame();
		ImGuiIO& io = ImGui::GetIO();

		SDL_Time newTime;
		SDL_GetCurrentTime(&newTime);
		io.DeltaTime = time > 0.0f ? ((float)(newTime - time)/(float)1000000000) : (1.0f / 60.0f);
		time = newTime;


		ImGui::NewFrame();

		ImGui::ShowDemoWindow();

		ImGui::Render();

		ImDrawData *drawData = ImGui::GetDrawData();

		const Emma::EmmaWindow *window = app->mainWindow;

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
