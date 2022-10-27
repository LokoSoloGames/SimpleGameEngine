#include "Entity.h"

namespace SimpleGameEngine {
	const TypeInfo* Entity::s_getType() {
		class TI : public TI_Base {
		public:
			TI() {
				//static FieldInfo fi[] = {
				//};
				//setFields(fi);
			}
		};
		static TI ti;
		return &ti;
	}

	SPtr<CTransform> Entity::transform() {
		if (!_transform) {
			_transform = getComponent<CTransform>();
		}
		return _transform;
	}
}