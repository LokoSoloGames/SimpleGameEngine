#pragma once

namespace SimpleGameEngine {
	class Entity;

	class Component : public Object {
		SGE_OBJECT_TYPE(Component, Object)
	public:
		Entity* entity() { return _entity; }
		void internal_setEntity(Entity* e) { _entity = e; }
	private:
		Entity* _entity;
	};
}