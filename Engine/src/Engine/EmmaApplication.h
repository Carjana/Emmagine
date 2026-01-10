#pragma once
#include "Core.h"
#include "EmmaWindow.h"
#include "Layer/Layer.h"
#include "Layer/LayerStack.h"

namespace Emma
{
	class EMMA_API EmmaApplication
	{
	public:
		EmmaApplication();
		virtual ~EmmaApplication();

		void CreateEmmaWindow(const WindowProps &props);
		void DestroyEmmaWindow();

		virtual void OnInit() {}
		virtual void OnQuit() {}

		void OnEvent(Event &event);
		void Quit();

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
