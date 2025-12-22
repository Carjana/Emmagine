#pragma once
#include "Core.h"
#include "EmmaWindow.h"
#include "Events/WindowEvents.h"
#include "Layer/Layer.h"
#include "Layer/LayerStack.h"

namespace Emma
{
	class EMMA_API EmmaApplication
	{
	public:
		EmmaApplication();
		virtual ~EmmaApplication();

		virtual void Init() {}

		void OnEvent(Event &event);
		bool OnWindowCloseRequestEvent(const WindowCloseRequestEvent &event);

		void PushLayer(Layer *layer);

		void PopLayer(Layer *layer);

		virtual void Run();

		bool isRunning = true;
		EmmaWindow *mainWindow;


		static EmmaApplication *GetInstance(){return Instance;}

	private:
		static EmmaApplication *Instance;
		// rethink multiple windows...
		// maybe but isFocused etc. in EmmaWindow?
		LayerStack layerStack;
	};

	EmmaApplication *CreateEmmaApplication();
}
