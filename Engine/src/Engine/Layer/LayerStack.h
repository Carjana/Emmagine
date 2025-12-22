#pragma once
#include "Engine/Core.h"
#include "Layer.h"

namespace Emma
{
	class EMMA_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() {return layers.begin();};
		std::vector<Layer*>::iterator end() {return layers.end();};

	private:
		std::vector<Layer*> layers;
		std::vector<Layer*>::iterator layerInsert;
	};
}
