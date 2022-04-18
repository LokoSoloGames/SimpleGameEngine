#pragma once

namespace SimpleGameEngine {
	enum class VertexSemantic {
		POSITION,
		COLOR,
	};
	inline const char* VertexSemanticToStringC(VertexSemantic v){
		switch (v) {
			case VertexSemantic::POSITION:	return "POSITION";
			case VertexSemantic::COLOR:		return "COLOR";
			default:						return "";
		}
	}

	enum class RenderDataType {
		FLOAT32x3,
		FLOAT32x4,
		UNORM8x4,
		UINT32x4,
	};
	inline const u32 RenderDataTypeSize(RenderDataType type){
		switch (type) {
			case RenderDataType::FLOAT32x3:	return 12;
			case RenderDataType::FLOAT32x4:	return 16;
			case RenderDataType::UNORM8x4:	return 4;
			case RenderDataType::UINT32x4:	return 16;
			default:						return 0;
		}
	}

	struct Color4b {
		u8 r, g, b, a;
	};

	struct Position3f {
		float x, y, z;
	};

	struct Vertex_PosColor {
		Position3f position;
		Color4b color;
	};

	struct VertexLayout {
		struct Element {
			VertexSemantic semantic;
			RenderDataType type;

			Element(VertexSemantic s, RenderDataType t) : semantic(s), type(t) {}
		};

		Vector<Element> elements;

		static VertexLayout PosColor;
	};
}