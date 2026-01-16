#include "ExampleEditorLayer.h"

#include "imgui.h"

namespace EmmaEditor
{
	void ExampleEditorLayer::OnRenderImGui()
	{
		static bool show = true;
		if (show == false)
			return;
		ImGui::Begin("Example", &show);
		ImGui::Text("Hello from ExampleEditorLayer!");
		ImGui::End();
	}
}
