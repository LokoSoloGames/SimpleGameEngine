#include "VertexLayout.h"

namespace SimpleGameEngine {
	VertexLayout VertexLayout::PosColor = VertexLayout{{
		{VertexLayout::Element(VertexSemantic::POSITION, RenderDataType::FLOAT32x3)},
		{VertexLayout::Element(VertexSemantic::COLOR, RenderDataType::UNORM8x4)},
	}};
}