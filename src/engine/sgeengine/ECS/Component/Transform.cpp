#include "Transform.h"

namespace SimpleGameEngine {
	const TypeInfo* Transform::s_getType() {
		//class TI : public TI_Base {
		//public:
		//	TI() {
		//		static FieldInfo fi[] = {
		//			FieldInfo("_position", &This::_position),
		//			FieldInfo("_rotation", &This::_rotation),
		//			FieldInfo("_localScale", &This::_localScale),
		//		};
		//		setFields(fi);
		//	}
		//};
		//static TI ti;

		static TypeInfoInitNoBase<Transform> ti("Transform");
		return &ti;
	}
}