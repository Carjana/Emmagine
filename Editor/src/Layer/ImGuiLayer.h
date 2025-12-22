#pragma once
#include "Engine/EmmaApplication.h"
#include "Engine/Layer/Layer.h"

namespace EmmaEditor
{

	class ImGuiLayer : public Emma::Layer
	{
	public:
		ImGuiLayer(): Layer("ImGuiLayer")
		{}

	private:
		SDL_Time time = 0.0f;
		Emma::EmmaApplication *app;
		void OnAttach() override;
		void OnDetach() override;
		void OnEvent(Emma::Event &event) override;
		void OnUpdate() override;
	};
}
