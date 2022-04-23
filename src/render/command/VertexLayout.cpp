#include "VertexLayout.h"

namespace SimpleGameEngine {
	VertexLayout VertexLayout::PosColor = VertexLayout{
		{
				{VertexLayout::Element(VertexSemantic::POSITION, RenderDataType::FLOAT32x3, 0)},
			{VertexLayout::Element(VertexSemantic::COLOR, RenderDataType::UNORM8x4, 12)},
		},
		RenderDataTypeSize(RenderDataType::FLOAT32x3) + RenderDataTypeSize(RenderDataType::UNORM8x4)
	};
}