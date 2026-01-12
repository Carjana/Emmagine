#include "EmmaWindow.h"

#include "EmmaApplication.h"
#include "Logger.h"
#include "Events/MouseEvents.h"
#include "Events/WindowEvents.h"

namespace Emma
{
	EmmaWindow::EmmaWindow(const WindowProps &props)
	{
		Init(props);
	}

	EmmaWindow::~EmmaWindow()
	{
		Shutdown();
	}

	void EmmaWindow::Init(const WindowProps &props)
	{
		LOG_INFO("Creating Window with Title \"{0}\", Width:{1}, Height:{2}", props.Title, props.Width, props.Height);
		Window = SDL_CreateWindow(props.Title.c_str(), props.Width, props.Height, SDL_WINDOW_RESIZABLE);
		ASSERT(Window, "Window Creation Failed!", SDL_GetError());
		LOG_INFO("Window Created with ID {0}", WindowId);
		WindowId = SDL_GetWindowID(Window);

		Width = props.Width;
		Height = props.Height;
		SDL_GetWindowPosition(Window, ((int*)&X), (int*)&Y);

		GPUDevice = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL | SDL_GPU_SHADERFORMAT_METALLIB, true, nullptr);
		ASSERT(GPUDevice, "GPU Device Creation Failed!", SDL_GetError());
		ASSERT_FUNC(SDL_ClaimWindowForGPUDevice(GPUDevice, Window), "Failed to claim window for GPU Device!", SDL_GetError());

		SDL_SetGPUSwapchainParameters(GPUDevice, Window, SDL_SWAPCHAIN_COMPOSITION, SDL_PRESENT_MODE);
	}

	void EmmaWindow::Shutdown() const
	{
		LOG_INFO("Destroying Window with ID {0}", WindowId);
		SDL_DestroyWindow(Window);
	}

	void EmmaWindow::OnEvent(Event &event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.DispatchEvent<WindowMoveEvent>(BIND_EVENT(EmmaWindow::OnWindowMoveEvent));
		dispatcher.DispatchEvent<WindowResizeEvent>(BIND_EVENT(EmmaWindow::OnWindowResizeEvent));
		dispatcher.DispatchEvent<WindowMouseFocusEvent>(BIND_EVENT(EmmaWindow::OnWindowMouseFocusEvent));
		dispatcher.DispatchEvent<WindowFocusEvent>(BIND_EVENT(EmmaWindow::OnWindowFocusEvent));
		dispatcher.DispatchEvent<WindowCloseRequestEvent>(BIND_EVENT(EmmaWindow::OnWindowCloseRequestEvent));
	}

	bool EmmaWindow::OnWindowMoveEvent(const WindowMoveEvent &event)
	{
		X = event.xPos;
		Y = event.yPos;
		return false;
	}

	bool EmmaWindow::OnWindowResizeEvent(const WindowResizeEvent &event)
	{
		Width = event.Width;
		Height = event.Height;
		return false;
	}

	bool EmmaWindow::OnWindowMouseFocusEvent(const WindowMouseFocusEvent &event)
	{
		HasMouseFocus = event.gainedFocus;
		return false;
	}

	bool EmmaWindow::OnWindowFocusEvent(const WindowFocusEvent &event)
	{
		HasFocus = event.gainedFocus;
		return false;
	}

	bool EmmaWindow::OnWindowCloseRequestEvent(WindowCloseRequestEvent &event)
	{
		if (event.WindowId != WindowId)
			return false;
		EmmaApplication::GetInstance()->DestroyEmmaWindow();
		return false;
	}
}
