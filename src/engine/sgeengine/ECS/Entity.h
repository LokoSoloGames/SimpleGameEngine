#pragma once

#include "Component.h"

namespace SimpleGameEngine {
	class Entity : public Object {
		SGE_TYPE(Entity, Object)

	public:
		template<class T>
		T* addComponent() {
			static_assert(std::is_base_of<Component, T>::value, "invalid component class");
			auto c = typeof(T)->createObject();
			_components.emplace_back(sge_cast<Component>(c))
			return sge_cast<T>(c);
		}

		template<class T>
		T* getComponent() {
			if (_components.size() == 0) return nullptr;
			auto typeInfo = typeof(T);
			for (auto& c : _components) {
				if (c.getType()->isKindOf(typeInfo)) {
					return &c;
				}
			}
			return nullptr;
		}

		virtual void onInspectorGUI() {
			for (auto& c : _components) {
				c.onGUI();
			}
		}

	private:
		Vector<Component> _components;
	};
}