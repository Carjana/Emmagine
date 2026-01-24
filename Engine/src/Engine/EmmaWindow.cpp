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
		LOG_CORE_INFO("Creating Window with Title \"{0}\", Width:{1}, Height:{2}", props.Title, props.Width, props.Height);
		Window = SDL_CreateWindow(props.Title.c_str(), (int)props.Width, (int)props.Height, SDL_WINDOW_RESIZABLE);
		CORE_ASSERT_MESSAGE(Window, "Window Creation Failed!", SDL_GetError());
		LOG_CORE_INFO("Window Created with ID {0}", WindowId);
		WindowId = SDL_GetWindowID(Window);

		Width = props.Width;
		Height = props.Height;
		SDL_GetWindowPosition(Window, (int*)&X, (int*)&Y);

		GraphicsContextCreateInfo createInfo;
		createInfo.SwapchainComposition = SDL_GPU_SWAPCHAINCOMPOSITION_SDR;
		createInfo.PresentMode = SDL_GPU_PRESENTMODE_MAILBOX;

		LOG_CORE_TRACE("Creating Graphics Context for Window ID {0}...", WindowId);
		Context = new GraphicsContext(Window);
		Context->Init(createInfo);
		LOG_CORE_TRACE("Done creating Graphics Context for Window ID {0}!", WindowId);
	}

	void EmmaWindow::Shutdown() const
	{
		LOG_CORE_INFO("Destroying Window with ID {0}", WindowId);
		Context->Destroy();
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
