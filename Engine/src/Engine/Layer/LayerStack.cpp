#include "LayerStack.h"

namespace Emma
{
	LayerStack::LayerStack()
	{}

	LayerStack::~LayerStack()
	{
		for (Layer *layer : layers)
			delete layer;
	}

	void LayerStack::PushLayer(Layer *layer)
	{
		layers.emplace_back(layer);
	}

	void LayerStack::PopLayer(Layer *layer)
	{
		std::vector<Layer *>::iterator iterator = std::find(layers.begin(), layers.end(), layer);
		if (iterator != layers.end())
		{
			layers.erase(iterator);
		}
	}
}
