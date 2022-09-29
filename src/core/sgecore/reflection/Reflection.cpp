#include "Reflection.h"

namespace SimpleGameEngine {
#define SGE_TYPEOF_IMPL(T) \
	template<> const TypeInfo* typeof<T>() { \
		static TypeInfoInitNoBase<T> ti(#T); \
		return &ti; \
	} \
//----

	SGE_TYPEOF_IMPL(float)
	SGE_TYPEOF_IMPL(double)

	SGE_TYPEOF_IMPL(int8_t)
	SGE_TYPEOF_IMPL(int16_t)
	SGE_TYPEOF_IMPL(int32_t)
	SGE_TYPEOF_IMPL(int64_t)

	SGE_TYPEOF_IMPL(uint8_t)
	SGE_TYPEOF_IMPL(uint16_t)
	SGE_TYPEOF_IMPL(uint32_t)
	SGE_TYPEOF_IMPL(uint64_t)
	// SGE_TYPEOF_IMPL(char8_t)
	SGE_TYPEOF_IMPL(char16_t)
	SGE_TYPEOF_IMPL(char32_t)
	SGE_TYPEOF_IMPL(wchar_t)

	template<> const TypeInfo* typeof<Object>() {
		class TI : public TypeInfo {
		public:
			TI() {
				name = "Object";
			}
		};
		static TI ti;
		return &ti;
	}

	TypeManager* TypeManager::instance() {
		static TypeManager m;
		return &m;
	}
}