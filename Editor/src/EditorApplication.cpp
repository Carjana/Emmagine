#include "Emmagine.h"
#include "Engine/MainEntry.h"
#include "Layer/ExampleEditorLayer.h"


namespace EmmaEditor
{
	class EditorApplication : public Emma::EmmaApplication
	{
		void OnInit() override
		{
			ImGui::SetCurrentContext(GetImGuiContext());
			PushLayer(new ExampleEditorLayer());
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
