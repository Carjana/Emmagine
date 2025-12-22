#include "LayerStack.h"

namespace Emma
{
	LayerStack::LayerStack()
	{
		layerInsert = layers.begin();
	}

	LayerStack::~LayerStack()
	{
		for (Layer *layer : layers)
			delete layer;
	}

	void LayerStack::PushLayer(Layer *layer)
	{
		layerInsert = layers.emplace(layerInsert, layer);
		layer->OnAttach();
	}

	void LayerStack::PushOverlay(Layer *overlay)
	{
		layers.emplace_back(overlay);
		overlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer *layer)
	{
		std::vector<Layer *>::iterator iterator = std::find(layers.begin(), layers.end(), layer);
		if (iterator != layers.end())
		{
			layer->OnDetach();
			layers.erase(iterator);
			layerInsert--;
		}
	}

	void LayerStack::PopOverlay(Layer *overlay)
	{
		std::vector<Layer *>::iterator iterator = std::find(layers.begin(), layers.end(), overlay);
		if (iterator != layers.end())
		{
			overlay->OnDetach();
			layers.erase(iterator);
		}
	}
}
