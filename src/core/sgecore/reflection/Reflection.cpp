#include "Reflection.h"

namespace SimpleGameEngine {

	SGE_TYPEOF_IMPL(float, "float")
	SGE_TYPEOF_IMPL(double, "double")

	SGE_TYPEOF_IMPL(int8_t, "int8")
	SGE_TYPEOF_IMPL(int16_t, "int16")
	SGE_TYPEOF_IMPL(int32_t, "int32")
	SGE_TYPEOF_IMPL(int64_t, "int64")

	SGE_TYPEOF_IMPL(uint8_t, "uint8")
	SGE_TYPEOF_IMPL(uint16_t, "uint16")
	SGE_TYPEOF_IMPL(uint32_t, "uint32")
	SGE_TYPEOF_IMPL(uint64_t, "uint64")

	// SGE_TYPEOF_IMPL(char8_t, "char8")
	SGE_TYPEOF_IMPL(char16_t, "char16")
	SGE_TYPEOF_IMPL(char32_t, "char32")
	SGE_TYPEOF_IMPL(wchar_t, "wchar")

	TypeManager* TypeManager::instance() {
		static TypeManager m;
		return &m;
	}
}