#pragma once

#include <sgerender.h>
#include <nlohmann/json.hpp>

namespace SimpleGameEngine {
	struct ShaderLayout {
		using Json = nlohmann::json;
		struct InputLayout {
			String attrId;
			RenderDataType dataType = RenderDataType::None;

			Json toJson();
		};

		struct Variable {
			String name;
			u32 offset;
			RenderDataType dataType = RenderDataType::None;

			Json toJson();
		};

		struct UniformBufferLayout {
			String name;
			u8 bindPoint;
			u8 bindCount;
			u32 dataSize;
			Vector<Variable> variables;

			Json toJson();
		};

		String profile;
		Vector<InputLayout> inputs;
		Vector<UniformBufferLayout> uniformBuffers;

		void clear();
		String toJson();
	};
}