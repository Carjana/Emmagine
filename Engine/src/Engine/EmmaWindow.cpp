#include "EmmaWindow.h"

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_video.h"

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
		window = SDL_CreateWindow(props.Title.c_str(), props.Width, props.Height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN);
		ASSERT(window, "Window Creation Failed!", SDL_GetError());
		LOG_INFO("Window Created");
	}

	void EmmaWindow::OnUpdate(SDL_WindowEvent event)
	{
		switch (event.type)
		{
			case SDL_EVENT_WINDOW_MOVED:
				LOG_TRACE("Window Moved to x:{0}, y:{1}", event.data1, event.data2);
				break;
			case SDL_EVENT_WINDOW_RESIZED:
				LOG_TRACE("Window resized: w:{0}, h:{1}", event.data1, event.data2);
				break;
			default:
				break;
		}
	}

	unsigned int EmmaWindow::GetWidth() const
	{
		int w;
		SDL_GetWindowSize(window, &w, nullptr);
		return w;
	}

	unsigned int EmmaWindow::GetHeight() const
	{
		int h;
		SDL_GetWindowSize(window, nullptr, &h);
		return h;
	}

	void EmmaWindow::GetWindowSize(int *width, int *height) const
	{
		SDL_GetWindowSize(window, width, height);
	}

	void EmmaWindow::SetEventCallback(const EventCallbackFunc &callback)
	{

	}

	void EmmaWindow::Shutdown()
	{
		SDL_DestroyWindow(window);
	}
}
