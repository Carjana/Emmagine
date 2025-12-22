#include "EmmaApplication.h"

#include "Logger.h"
#include "Events/MouseEvents.h"
#include "SDL_events.h"
#include "SDL_init.h"

namespace Emma
{

#define BIND_EVENT_FN(x) std::bind(&EmmaApplication::x, this, std::placeholders::_1)

	EmmaApplication* EmmaApplication::Instance = nullptr;

	EmmaApplication::EmmaApplication()
	{
		Instance = this;
		mainWindow = EmmaWindow::CreateEmmaWindow(WindowProps("Emmagine", 1280, 720));
		mainWindow->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	EmmaApplication::~EmmaApplication()
	{
		if (mainWindow)
			delete mainWindow;
	}

	void EmmaApplication::OnEvent(Event& event)
	{
		if (EventShouldLog[(char)event.GetEventType()])
		LOG_TRACE(event.ToString());
		EventDispatcher dispatcher(event);
		dispatcher.DispatchEvent<WindowCloseRequestEvent>(BIND_EVENT_FN(OnWindowCloseRequestEvent));

		for (std::vector<Layer *>::iterator it = layerStack.end(); it != layerStack.begin();)
		{
			(*--it)->OnEvent(event);
			if (event.IsHandled)
				break;
		}
	}

	bool EmmaApplication::OnWindowCloseRequestEvent(const WindowCloseRequestEvent &event)
	{
		assert(mainWindow);
		if (mainWindow->WindowId == event.WindowId)
		{
			isRunning = false;
		}
		return true;
	}

	void EmmaApplication::PushLayer(Layer *layer)
	{
		layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void EmmaApplication::PopLayer(Layer *layer)
	{
		layerStack.PopLayer(layer);
		layer->OnDetach();
	}

	void EmmaApplication::Run()
	{
		Init();
		while (isRunning)
		{
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				// Handle Events
				mainWindow->HandleEvent(event);
				switch (event.type)
				{
					case SDL_EVENT_MOUSE_MOTION:
					{
						if (mainWindow->HasFocus)
						{
							MouseMovedEvent mouseEvent(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel, event.motion.x / (float)mainWindow->Width, event.motion.y / (float)mainWindow->Height);
							OnEvent(mouseEvent);
						}
					}
					break;
					case SDL_EVENT_MOUSE_BUTTON_DOWN:
					case SDL_EVENT_MOUSE_BUTTON_UP:
					{
						MouseButton button = MouseButton::Left;
						switch (event.button.button)
						{
							case SDL_BUTTON_LEFT:
								button = MouseButton::Left;
								break;
							case SDL_BUTTON_MIDDLE:
								button = MouseButton::Middle;
								break;
							case SDL_BUTTON_RIGHT:
								button = MouseButton::Right;
								break;
							case SDL_BUTTON_X1:
								button = MouseButton::X1;
								break;
							case SDL_BUTTON_X2:
								button = MouseButton::X2;
								break;
							default:
								ASSERT(false, "Invalid code path!");
								break;
						}
						MouseButtonEvent emmaEvent(button, event.button.down);
						OnEvent(emmaEvent);
					}break;
					case SDL_EVENT_MOUSE_WHEEL:
					{
						MouseWheelEvent emmaEvent(event.wheel.x, event.wheel.y);
						OnEvent(emmaEvent);
					}break;
				}
			}

			for (Layer *layer : layerStack)
				layer->OnUpdate();
		}
		SDL_Quit();
	}

#undef BIND_EVENT_FN
}
