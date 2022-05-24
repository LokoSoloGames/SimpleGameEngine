#include "ShaderLayout.h"

namespace SimpleGameEngine {
	void ShaderLayout::clear() {
		profile = "";
		inputs.clear();
	}

	String ShaderLayout::toJson() {
		Json j;
		j["profile"] = profile.c_str();
		for (auto& ele : inputs) {
			j["inputs"].emplace_back(ele.toJson());
		}
		auto str = j.dump();
		return String(StrView(str.data(), str.size()));
	}

	nlohmann::json ShaderLayout::InputLayout::toJson() {
		Json j;
		j["attrId"] = attrId.c_str();
		j["dataType"] = String(enumStr(dataType)).c_str();
		return j;
	}
}