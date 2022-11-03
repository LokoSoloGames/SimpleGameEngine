#include "CRenderer.h"

namespace SimpleGameEngine {
	const TypeInfo* CRenderer::s_getType() {
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

	template<> inline
	static Object* TypeCreator<CRenderer>() {
		return nullptr;
	}
}