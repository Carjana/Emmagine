#include "EmmaApplication.h"

#include "EmmaInput.h"
#include "Events/MouseEvents.h"
#include "Events/KeyboardEvents.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"

#include "Logger.h"
#include "Engine/Rendering/Renderer.h"
#include "Engine/ServiceProvider/ServiceProvider.h"

namespace Emma
{
	EmmaApplication* EmmaApplication::Instance = nullptr;

	EmmaApplication::EmmaApplication()
	{
		Instance = this;
		mainWindow = new EmmaWindow(WindowProps("Emmagine", 1280, 720));

		Renderer *renderer = new Renderer();
		renderer->CreateBuffersAndPipeline(mainWindow->Context);
		RegisterService<Renderer>(renderer);

		coreInputLayer = new CoreInput();
		PushLayer(coreInputLayer);

		imGuiLayer = new ImGuiLayer();
		PushLayer(imGuiLayer);
	}

	void EmmaApplication::OnEvent(Event& event)
	{
		if (EventShouldLog[(char)event.GetEventType()])
			LOG_CORE_TRACE(event);

		// Maybe put window in an extra window layer?
		if (event.WindowId == mainWindow->WindowId)
			mainWindow->OnEvent(event);

		for (std::vector<Layer *>::iterator it = layerStack.end(); it != layerStack.begin();)
		{
			(*--it)->OnEvent(event);
			if (event.IsHandled)
				break;
		}
	}

	void EmmaApplication::Quit()
	{
		isRunning = false;
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
		OnInit();
		while (isRunning)
		{
			PollEvents();

			for (int i = (int)layerStack.layers.size() - 1; i >= 0; --i)
				layerStack.layers[i]->OnUpdate();

			if (imGuiLayer)
			{
				imGuiLayer->Begin();
				for (int i = (int)layerStack.layers.size() - 1; i >= 0; --i)
					layerStack.layers[i]->OnRenderImGui();
				imGuiLayer->End();
				for (int i = (int)layerStack.layers.size() - 1; i >= 0; --i)
					layerStack.layers[i]->PostRenderImGui();
			}


		}
		OnQuit();

		Shutdown();
		SDL_Quit();
	}

	void EmmaApplication::Shutdown()
	{
		for (int i = (int)layerStack.layers.size() - 1; i >= 0; --i)
			PopLayer(layerStack.layers[i]);

		GetService<Renderer>()->Shutdown();
		mainWindow->Shutdown();
	}

	void EmmaApplication::PollEvents()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			// maybe handle events via emma events;
			if (imGuiLayer)
				imGuiLayer->HandleSDLEvent(event);
			// Handle Events
			switch (event.type)
			{
				case SDL_EVENT_WINDOW_MOVED:
				{
					WindowMoveEvent emmaEvent(event.window.windowID, event.window.data1, event.window.data2);
					OnEvent(emmaEvent);
				}break;
				case SDL_EVENT_WINDOW_RESIZED:
				{
					WindowResizeEvent emmaEvent(event.window.windowID, event.window.data1, event.window.data2);
					OnEvent(emmaEvent);
				}break;
				case SDL_EVENT_WINDOW_MOUSE_ENTER:
				{
					WindowMouseFocusEvent emmaEvent(event.window.windowID, true);
					OnEvent(emmaEvent);
				}break;
				case SDL_EVENT_WINDOW_MOUSE_LEAVE:
				{
					WindowMouseFocusEvent emmaEvent(event.window.windowID, false);
					OnEvent(emmaEvent);
				}break;
				case SDL_EVENT_WINDOW_FOCUS_GAINED:
				{
					WindowFocusEvent emmaEvent(event.window.windowID, true);
					OnEvent(emmaEvent);
				}break;
				case SDL_EVENT_WINDOW_FOCUS_LOST:
				{
					WindowFocusEvent emmaEvent(event.window.windowID, false);
					OnEvent(emmaEvent);
				}break;
				case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
				{
					WindowCloseRequestEvent emmaEvent(event.window.windowID);
					OnEvent(emmaEvent);
				}break;

				case SDL_EVENT_MOUSE_MOTION:
				{
					if (mainWindow->HasFocus)
					{
						MouseMovedEvent mouseEvent(event.motion.windowID, event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel, event.motion.x / (float)mainWindow->Width, event.motion.y / (float)mainWindow->Height);
						OnEvent(mouseEvent);
					}
				}
				break;
				case SDL_EVENT_MOUSE_BUTTON_DOWN:
				case SDL_EVENT_MOUSE_BUTTON_UP:
				{
					MouseButtonEvent emmaEvent(event.button.windowID, SDL_BUTTON_MASK(event.button.button), event.button.down);
					OnEvent(emmaEvent);
				}break;
				case SDL_EVENT_MOUSE_WHEEL:
				{
					MouseWheelEvent emmaEvent(event.wheel.windowID, event.wheel.x, event.wheel.y);
					OnEvent(emmaEvent);
				}break;
				case SDL_EVENT_KEY_DOWN:
				case SDL_EVENT_KEY_UP:
				{
					KeyEvent emmaEvent(event.key.windowID, event.key.key, event.key.scancode, event.key.mod, event.key.down);
					OnEvent(emmaEvent);
				}break;
				case SDL_EVENT_TEXT_INPUT:
				{
					KeyTextEvent emmaEvent(event.text.windowID, event.text.text);
					OnEvent(emmaEvent);
				}break;
				default:
					break;
			}
		}
	}

}
