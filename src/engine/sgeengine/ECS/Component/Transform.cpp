#include "Transform.h"

namespace SimpleGameEngine {

	const TypeInfo* Transform::s_getType() {
		class TI : public TI_Base {
		public:
			TI() {
				static FieldInfo fi[] = {
					FieldInfo("position",	&This::position),
					FieldInfo("rotation",	&This::rotation),
					FieldInfo("localScale",	&This::localScale),
				};
				setFields(fi);
			}
		};
		static TI ti;
		return &ti;

	}
}