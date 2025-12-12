#pragma once

#include "emmapch.h"
#include "Core.h"
#include "Events/Event.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_video.h"

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
		~EmmaWindow();

		void OnUpdate(SDL_WindowEvent event);

		inline unsigned int GetWidth() const;
		inline unsigned int GetHeight() const;
		inline void GetWindowSize(int *width, int *height) const;

		inline void SetEventCallback(const EventCallbackFunc &callback);

		SDL_Window *window;

		static EmmaWindow* CreateEmmaWindow(const WindowProps &props)
		{
			return new EmmaWindow(props);
		}

	private:
		void Init(const WindowProps& props);
		void Shutdown();
	};
}
