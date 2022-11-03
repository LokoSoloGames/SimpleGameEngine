#pragma once

#include "Component.h"
#include "Component/CTransform.h"

namespace SimpleGameEngine {
	class Scene;
	class CTransform;

	enum class EntityId : u64 { None };
	SGE_FORMATTER_ENUM_AS_INT(EntityId)

	class Entity : public Object {
		SGE_OBJECT_TYPE(Entity, Object)

	public:
		Entity();

		void setName(StrView name) { _name = name; }
		StrView name() const { return _name; }

		void		_internalInit(Scene* scene, EntityId id) { _scene = scene; _id = id; }

		Scene* scene() const { return _scene; }
		EntityId	id() const { return _id; }

		template<class T>
		T* addComponent() {
			static_assert(std::is_base_of<Component, T>::value, "invalid component class");
			auto* c = new T();
			_components.emplace_back(c);
			c->_internalSetEntity(this);
			return c;
		}

		template<class T>
		T* getComponent() {
			if (_components.size() == 0) return nullptr;
			auto typeInfo = typeof(T);
			for (auto* c : _components) {
				if (c->getType()->isKindOf(typeInfo)) {
					return c;
				}
			}
			return nullptr;
		}

		Span< SPtr<Component> >	components() { return _components; }

		CTransform* transform() { return _transform.ptr(); }

	private:
		Vector< SPtr<Component> >	_components;
		SPtr<CTransform>			_transform = nullptr;

		String		_name;
		Scene*		_scene = nullptr;
		EntityId	_id = EntityId::None;
	
	};
}