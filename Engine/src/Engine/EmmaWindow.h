#pragma once

#include "emmapch.h"
#include "Core.h"
#include "Events/Event.h"
#include "SDL_events.h"
#include "SDL_video.h"

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

		unsigned int Width;
		unsigned int Height;

		unsigned int X;
		unsigned int Y;

		EmmaWindow(const WindowProps& props);
		~EmmaWindow();

		void HandleEvent(const SDL_Event &event);

		inline void SetEventCallback(const EventCallbackFunc &callback);

		SDL_Window *Window;
		unsigned int WindowId;
		bool HasFocus;

		static EmmaWindow* CreateEmmaWindow(const WindowProps &props)
		{
			return new EmmaWindow(props);
		}

	private:
		void Init(const WindowProps& props);
		void Shutdown() const;
		EventCallbackFunc eventCallback;
	};
}
