#pragma once

#include "Engine/Core.h"
#include "emmapch.h"
#include "Engine/Events/Event.h"

namespace Emma
{
class EMMA_API Layer
{
	public:
	std::string Name;

		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnRenderImGui() {}
		virtual void PostRenderImGui() {}
		virtual void OnEvent(Event& event) {}
	};
}
