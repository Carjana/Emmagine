#pragma once
#include "Emmagine.h"

namespace EmmaEditor
{
	class ViewPortLayer : public Emma::Layer
	{
	public:
		ViewPortLayer() : Layer("Editor Viewport")
		{}

		void OnAttach() override;
		void OnRenderImGui() override;
		void PostRenderImGui() override;

		Emma::Renderer *renderer = nullptr;
		Emma::GraphicsContext *graphicsContext = nullptr;

		SDL_GPUTexture *testTexture = nullptr;
	};
}
