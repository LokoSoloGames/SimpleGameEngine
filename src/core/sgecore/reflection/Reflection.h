#pragma once

#include <sgebase.h>
#include "FieldInfo.h"
#include "TypeInfo.h"

#define SGE_STRUCT_TYPE(T, BASE) \
private: \
	using This = T; \
	using Base = BASE; \
	class TI_Base : public TypeInfoInit<T, BASE> { \
	public: \
		TI_Base() : TypeInfoInit<T, BASE>(#T) {} \
	}; \
public: \
	static const TypeInfo* s_getType(); \
//----

#define SGE_OBJECT_TYPE(T, BASE) \
private: \
	using This = T; \
	using Base = BASE; \
	class TI_Base : public TypeInfoInit<T, BASE> { \
	public: \
		TI_Base() : TypeInfoInit<T, BASE>(#T) {} \
	}; \
public: \
	static const TypeInfo* s_getType(); \
	virtual const TypeInfo* getType() const override { return s_getType(); } \
private: \
//-----

namespace SimpleGameEngine {

	template<class T> inline const TypeInfo* sge_typeof() { return T::s_getType(); }
	template<class T> inline const TypeInfo* sge_typeof(const T& v) { return sge_typeof<T>(); }

	class TypeManager {
	public:
		static TypeManager* instance();

		const TypeInfo* find(const char* name) {
			auto it = m_map.find(name);
			if (it == m_map.end()) return nullptr;
			return it->second;
		}

		void registerType(const char* name, const TypeInfo* ti) {
			m_map[name] = ti;
		}

	private:
		StringMap<const TypeInfo*> m_map;
	};

	#define SGE_TYPEOF_DEFINE(T) \
	template<> const TypeInfo* sge_typeof<T>();
	//----

	#define SGE_TYPEOF_IMPL(T, NAME) \
		template<> const TypeInfo* sge_typeof<T>() { \
			static TypeInfoInitNoBase<T> ti(NAME); \
			return &ti; \
		} \
	//----

	SGE_TYPEOF_DEFINE(float)
	SGE_TYPEOF_DEFINE(double)

	SGE_TYPEOF_DEFINE(int8_t)
	SGE_TYPEOF_DEFINE(int16_t)
	SGE_TYPEOF_DEFINE(int32_t)
	SGE_TYPEOF_DEFINE(int64_t)

	SGE_TYPEOF_DEFINE(uint8_t)
	SGE_TYPEOF_DEFINE(uint16_t)
	SGE_TYPEOF_DEFINE(uint32_t)
	SGE_TYPEOF_DEFINE(uint64_t)

	// SGE_TYPEOF_DEFINE(char8_t) c++20
	SGE_TYPEOF_DEFINE(char16_t)
	SGE_TYPEOF_DEFINE(char32_t)
	SGE_TYPEOF_DEFINE(wchar_t)

	template<class DST> inline
	DST* sge_cast(Object* src) {
		if (!src) return nullptr;
		auto* ti = Object(*src);
		if (!ti) return nullptr;
		if (!ti->isKindOf<DST>()) return nullptr;
		return static_cast<DST*>(src);
	}
}