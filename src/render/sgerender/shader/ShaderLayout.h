#pragma once

#include <sgerender/vertex/Vertex.h>
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
			u32 bindPoint;
			u32 bindCount;
			u32 dataSize;
			Vector<Variable> variables;

			Json toJson();

			const Variable* findVariable(StrView propName) const {
				for (auto& v : variables) {
					if (v.name == propName) return &v;
				}
				return nullptr;
			}
		};

		String profile;
		Vector<InputLayout> inputs;
		Vector<UniformBufferLayout> uniformBuffers;

		void clear();
		String toJson();
	};
}