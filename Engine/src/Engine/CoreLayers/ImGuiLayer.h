#pragma once
#include "SDL3/SDL_events.h"
#include "Engine/Layer/Layer.h"

namespace Emma
{

	class EMMA_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer() : Layer("ImGuiLayer")
		{}
		void OnAttach() override;
		void OnDetach() override;
		void OnRenderImGui() override;

		void Begin();
		void End();

		void HandleSDLEvent(SDL_Event &event);

	};
}
