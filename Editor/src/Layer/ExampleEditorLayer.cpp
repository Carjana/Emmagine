#include "ExampleEditorLayer.h"

#include "imgui.h"

namespace EmmaEditor
{
	void ExampleEditorLayer::OnRenderImGui()
	{
		bool isOpen = true;
		ImGui::Begin("Example", &isOpen);
		ImGui::Text("Hello from ExampleEditorLayer!");
		ImGui::End();
	}
}
