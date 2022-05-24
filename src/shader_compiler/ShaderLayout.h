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

		String profile;
		Vector<InputLayout> inputs;

		void clear();
		String toJson();
	};
}