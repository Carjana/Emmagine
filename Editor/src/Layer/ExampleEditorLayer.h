#pragma once
#include "Emmagine.h"
#include "Layer/ViewPortLayer.h"

namespace EmmaEditor
{
	class ExampleEditorLayer : public Emma::Layer
	{
	public:
		ViewPortLayer *viewportLayer = nullptr;

		void OnAttach() override;
		void OnRenderImGui() override;
	};
}
