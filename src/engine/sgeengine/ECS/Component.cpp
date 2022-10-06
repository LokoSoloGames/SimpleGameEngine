#include "Component.h"

namespace SimpleGameEngine {
	const TypeInfo* Component::s_getType() {
		/*class TI : public TI_Base {
		public:
			TI() {
				static FieldInfo fi[] = {
					FieldInfo("_entity", &This::_entity),
				};
				setFields(fi);
			}
		};
		static TI ti;*/

		static TypeInfoInitNoBase<Component> ti("Component");
		return &ti;
	}

	Component::Component() {

	}

	Component::~Component() {

	}
}