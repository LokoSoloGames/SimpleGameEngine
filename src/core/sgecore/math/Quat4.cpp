#include "Quat4.h"

namespace SimpleGameEngine {

	template<> const TypeInfo* sge_typeof<Quat4f>() {
		using This = Quat4f;
		using TI_Base = TypeInfoInitNoBase<This>;

		class TI : public TI_Base {
		public:
			TI() : TI_Base("Quat4f") {
				static FieldInfo fi[] = {
					FieldInfo("x", &This::x),
					FieldInfo("y", &This::y),
					FieldInfo("z", &This::z),
					FieldInfo("w", &This::w),
				};
				setFields(fi);
			}
		};
		static TI ti;
		return &ti;
	}

	template<> const TypeInfo* sge_typeof<Quat4d>() {
		using This = Quat4d;
		using TI_Base = TypeInfoInitNoBase<This>;

		class TI : public TI_Base {
		public:
			TI() : TI_Base("Quat4d") {
				static FieldInfo fi[] = {
					FieldInfo("x", &This::x),
					FieldInfo("y", &This::y),
					FieldInfo("z", &This::z),
					FieldInfo("w", &This::w),
				};
				setFields(fi);
			}
		};
		static TI ti;
		return &ti;
	}
}