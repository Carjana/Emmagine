#include "Emmagine.h"
#include "Layer/ImGuiLayer.h"

namespace EmmaEditor
{
	class EditorApplication : public Emma::EmmaApplication
	{
		void Init() override
		{
			Emma::Layer *layer = new ImGuiLayer();
			PushLayer(layer);

		}
	};
}

Emma::EmmaApplication* Emma::CreateEmmaApplication()
{
	return new EmmaEditor::EditorApplication();
}
