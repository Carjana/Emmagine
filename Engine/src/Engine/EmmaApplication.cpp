#include "EmmaApplication.h"

#include "SDL3/SDL_init.h"

namespace Emma
{
	EmmaApplication::EmmaApplication()
	{
		window = EmmaWindow::CreateEmmaWindow(WindowProps("Emmagine", 1280, 720));
	}

	EmmaApplication::~EmmaApplication()
	{

	}

	void EmmaApplication::Run()
	{
		while (isRunning)
		{
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				switch (event.type)
				{
					case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
					case SDL_EVENT_QUIT:
					{
						isRunning = false;
						return;
					}break;
					default:
						break;
				}

				if (event.window.windowID == SDL_GetWindowID(window->window))
				{
					window->OnUpdate(event.window);
				}
			}
		}
	}
}
