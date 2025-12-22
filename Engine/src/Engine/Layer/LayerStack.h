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
		void PopLayer(Layer* layer);

		std::vector<Layer*>::iterator begin() {return layers.begin();};
		std::vector<Layer*>::iterator end() {return layers.end();};

	private:
		std::vector<Layer*> layers;
	};
}
