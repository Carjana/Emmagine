#pragma once
#include "Engine/Layer/Layer.h"

namespace EmmaEditor
{
	class ExampleEditorLayer : public Emma::Layer
	{
		void OnRenderImGui() override;
	};
}
