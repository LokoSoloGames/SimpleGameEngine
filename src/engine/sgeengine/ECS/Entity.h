#pragma once

#include "Component.h"

namespace SimpleGameEngine {
	enum EntityId : u64 { None = 0 };

	class Entity : public Object {
		SGE_OBJECT_TYPE(Entity, Object)

	public:
		void setName(StrView name) { _name = name; }
		StrView name() const { return _name; }

		void		setId(EntityId id) { _id = id; }
		EntityId	id() const { return _id; }

		template<class T>
		T* addComponent() {
			static_assert(std::is_base_of<Component, T>::value, "invalid component class");
			auto* c = new T();
			_components.emplace_back(c);
			c->internal_setEntity(this);
			return c;
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

		Span< SPtr<Component> >	components() { return _components; }

	private:
		Vector< SPtr<Component> >	_components;

		String		_name;
		EntityId	_id = EntityId::None;
	
	};
}