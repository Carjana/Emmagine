#pragma once
#include "Engine/Core.h"
#include "Engine/Rendering/Buffer/VertexBufferLayout.h"

namespace Emma
{
	struct EmmaVertex
	{
		vec3 Position;
		vec2 UV;
		vec3 Color;

		static void FillVertexBufferLayout(VertexBufferLayout &vertexBufferLayout)
		{
			vertexBufferLayout.AddElement("Position", VertexBufferType::Float3);
			vertexBufferLayout.AddElement("UV", VertexBufferType::Float2);
			vertexBufferLayout.AddElement("Color", VertexBufferType::Float3);
			CORE_ASSERT_MESSAGE(sizeof(EmmaVertex) == vertexBufferLayout.Size, "Size of Vertex must match LayoutVertex size!")
		}
	};
}
