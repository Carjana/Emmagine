#include "Emmagine.h"
#include "Engine/MainEntry.h"
#include "Layer/ImGuiLayer.h"


namespace EmmaEditor
{
	class EditorApplication : public Emma::EmmaApplication
	{
		void OnInit() override
		{
			Emma::Layer *layer = new ImGuiLayer();
			PushLayer(layer);
		}
		void OnQuit() override
		{
		}
	};
}

Emma::EmmaApplication* Emma::CreateEmmaApplication()
{
	return new EmmaEditor::EditorApplication();
}
