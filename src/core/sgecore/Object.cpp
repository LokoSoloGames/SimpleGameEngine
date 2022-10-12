#include "Object.h"

namespace SimpleGameEngine {
	template<> const TypeInfo* sge_typeof<Object>() {
		class TI : public TypeInfo {
		public:
			TI() {
				name = "Object";
			}
		};
		static TI ti;
		return &ti;
	}
}
