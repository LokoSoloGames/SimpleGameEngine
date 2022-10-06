#include "Entity.h"

namespace SimpleGameEngine {
	const TypeInfo* Entity::s_getType() {
		static TypeInfoInitNoBase<Entity> ti("Entity");
		return &ti;
	}
}