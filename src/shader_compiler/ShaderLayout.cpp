#include "ShaderLayout.h"

namespace SimpleGameEngine {
	void ShaderLayout::clear() {
		profile = "";
		inputs.clear();
		uniformBuffers.clear();
	}

	// I Know This Is Stupid
	String ShaderLayout::toJson() {
		Json j;
		j["profile"] = profile.c_str();
		for (auto& ele : inputs) {
			j["inputs"].emplace_back(ele.toJson());
		}
		for (auto& ele : uniformBuffers) {
			j["uniformBuffers"].emplace_back(ele.toJson());
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

	nlohmann::json ShaderLayout::Variable::toJson() {
		Json j;
		j["name"] = name.c_str();
		j["offset"] = offset;
		j["dataType"] = String(enumStr(dataType)).c_str();
		return j;
	}

	nlohmann::json ShaderLayout::UniformBufferLayout::toJson() {
		Json j;
		j["name"] = name.c_str();
		j["bindPoint"] = bindPoint;
		j["bindCount"] = bindCount;
		j["dataSize"] = dataSize;
		for (auto& ele : variables) {
			j["variables"].emplace_back(ele.toJson());
		}
		return j;
	}
	// Stupidity Ends
}