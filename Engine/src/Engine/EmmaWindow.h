#pragma once

#include "emmapch.h"
#include "Core.h"
#include "Events/Event.h"
#include "SDL3/SDL_video.h"
#include "Events/WindowEvents.h"
#include "Rendering/GraphicsContext.h"

namespace Emma
{
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		WindowProps(const std::string& title = "Emmagine", unsigned int width = 1280, unsigned int height = 720)
			: Title(title), Width(width), Height(height) {}
	};

	class EMMA_API EmmaWindow
	{
	public:
		using EventCallbackFunc = std::function<void(Event&)>;

		EmmaWindow(const WindowProps& props);

		void OnEvent(Event &event);
		void Shutdown() const;

		unsigned int WindowId;
		SDL_Window *Window;
		GraphicsContext *Context;

		unsigned int Width;
		unsigned int Height;

		unsigned int X;
		unsigned int Y;

		bool HasFocus;
		bool HasMouseFocus;

	private:
		void Init(const WindowProps& props);

		bool OnWindowMoveEvent(const WindowMoveEvent &event);
		bool OnWindowResizeEvent(const WindowResizeEvent &event);
		bool OnWindowMouseFocusEvent(const WindowMouseFocusEvent &event);
		bool OnWindowFocusEvent(const WindowFocusEvent &event);
		bool OnWindowCloseRequestEvent(WindowCloseRequestEvent &event);
	};
}
