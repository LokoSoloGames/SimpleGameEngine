#pragma once

namespace SimpleGameEngine {
	struct Shaderinfo {
		struct InputInfo {
			String attrId;
			RenderDataType dataType;
		};
		
		Vector<InputInfo> inputs;
	};
}