#pragma once

#include <sgeengine.h>

namespace SimpleGameEngine {

class EntitySelection : public NonCopyable {
public:
	void add(EntityId id);
	void select(EntityId id);
	void deselectAll();
	bool has(EntityId id) const;

	Span<const EntityId> entities() const { return _entities; }

private:
	Vector<EntityId>	_entities;
	Set<EntityId>		_entitiesSet;

};

}