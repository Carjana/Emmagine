#pragma once
#include "Core.h"
#include "EmmaWindow.h"
#include "Events/WindowEvents.h"
#include "Layer/Layer.h"
#include "Layer/LayerStack.h"
#include "emmapch.h"

namespace Emma
{
	class EMMA_API EmmaApplication
	{
	public:
		EmmaApplication();
		virtual ~EmmaApplication();

		void OnEvent(Event &event);
		bool OnWindowCloseRequestEvent(const WindowCloseRequestEvent &event);

		void PushLayer(Layer *layer);
		void PushOverlay(Layer *overlay);

		virtual void Run();

		bool isRunning = true;

	private:
		// rethink multiple windows...
		// maybe but isFocused etc. in EmmaWindow?
		EmmaWindow *mainWindow;
		LayerStack layerStack;
	};

	EmmaApplication *CreateEmmaApplication();
}
