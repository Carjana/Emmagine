#include "EmmaWindow.h"

#include "Logger.h"
#include "Events/MouseEvents.h"
#include "Events/WindowEvents.h"
#include "SDL_events.h"
#include "SDL_gpu.h"
#include "SDL_video.h"

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
		LOG_INFO("Window Created");
		Width = props.Width;
		Height = props.Height;
		SDL_GetWindowPosition(Window, ((int*)&X), (int*)&Y);
		WindowId = SDL_GetWindowID(Window);
	}

	void EmmaWindow::HandleEvent(const SDL_Event &event)
	{
		if (event.window.windowID != WindowId)
			return;
		switch (event.type)
		{
			case SDL_EVENT_WINDOW_SHOWN:
    		case SDL_EVENT_WINDOW_HIDDEN:
    		case SDL_EVENT_WINDOW_EXPOSED:
					break;
    		case SDL_EVENT_WINDOW_MOVED:
    		{
    			X = event.window.data1;
    			Y = event.window.data2;
    			WindowMoveEvent emmaEvent(X, Y, WindowId);
    			eventCallback(emmaEvent);
    		}break;
    		case SDL_EVENT_WINDOW_RESIZED:
    		{
    			Width = event.window.data1;
    			Height = event.window.data2;
    			WindowResizeEvent emmaEvent(Width, Height, WindowId);
    			eventCallback(emmaEvent);
    		}break;
    		case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
    		case SDL_EVENT_WINDOW_METAL_VIEW_RESIZED:
    		case SDL_EVENT_WINDOW_MINIMIZED:
    		case SDL_EVENT_WINDOW_MAXIMIZED:
    		case SDL_EVENT_WINDOW_RESTORED:
				break;
    		case SDL_EVENT_WINDOW_MOUSE_ENTER:
    		{
    			WindowMouseFocusEvent emmaEvent(WindowId, true);
    			eventCallback(emmaEvent);
    		}break;
    		case SDL_EVENT_WINDOW_MOUSE_LEAVE:
    		{
				WindowMouseFocusEvent emmaEvent(WindowId, false);
    			eventCallback(emmaEvent);
    		}break;
    		case SDL_EVENT_WINDOW_FOCUS_GAINED:
    		{
    			WindowFocusEvent emmaEvent(WindowId, true);
    			eventCallback(emmaEvent);
    			HasFocus = true;
    		}break;
    		case SDL_EVENT_WINDOW_FOCUS_LOST:
    		{
    			WindowFocusEvent emmaEvent(WindowId, false);
    			eventCallback(emmaEvent);
    			HasFocus = false;
    		}
				break;
    		case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
    		{
    			WindowCloseRequestEvent emmaEvent(WindowId);
    			eventCallback(emmaEvent);
    		}break;
    		case SDL_EVENT_WINDOW_HIT_TEST:
    		case SDL_EVENT_WINDOW_ICCPROF_CHANGED:
    		case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
    		case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
    		case SDL_EVENT_WINDOW_SAFE_AREA_CHANGED:
    		case SDL_EVENT_WINDOW_OCCLUDED:
    		case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
    		case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
    		case SDL_EVENT_WINDOW_DESTROYED:
    		case SDL_EVENT_WINDOW_HDR_STATE_CHANGED:
				break;
		}
	}

	void EmmaWindow::SetEventCallback(const EventCallbackFunc &callback)
	{
		eventCallback = callback;
	}

	void EmmaWindow::Shutdown() const
	{
		SDL_DestroyWindow(Window);
	}
}
