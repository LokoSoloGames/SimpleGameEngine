#pragma once

#include <imgui.h>

namespace SimpleGameEngine {
	class Entity;

	class Component : public Object {
		SGE_TYPE(Component, Object)
	public:
		Component();
		~Component();
		Entity* entity() { return _entity; }

		virtual void onGUI() {
			auto* typeInfo = getType();
			for (auto& field : typeInfo->fields()) {
				ImGui::LabelText(field.name, field.fieldType->name);
			}
		}
	private:
		Entity* _entity;
	};
}