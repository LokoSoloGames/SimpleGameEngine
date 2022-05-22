#pragma once

namespace SimpleGameEngine {
	enum class RenderPrimitiveType {
		None,
		Points,
		Lines,
		Triangles,
	};

	enum class ShaderStage {
		None,
		Vertex,
		Pixel,
	};
}