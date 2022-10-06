#pragma once

#include <sgecore.h>

namespace SimpleGameEngine {
	class Component;

	class ComponentSystem : public NonCopyable {
	public:
		template<class T>
		void registerComponent(T* comp) {
			_components.emplace_back(comp);
		}

		template<class T>
		void unregisterComponent(T* comp) {
			_components.erase_first(comp);
		}

	private:
		Vector<Component*> _components;
	};
}