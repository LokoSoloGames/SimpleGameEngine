#pragma once

#include <sgebase.h>

namespace SimpleGameEngine {
	class Object;
	class TypeInfo;

	class FieldInfo {
	public:
		template<class OBJ, class FIELD>
		FieldInfo(const char* name_, FIELD OBJ::* ptr)
			: name(name_)
			, fieldType(sge_typeof<FIELD>())
			, offset(memberOffset(ptr))
		{ }

		void* getValuePtr(void* obj) const { return reinterpret_cast<u8*>(obj) + offset; }
		const void* getValuePtr(const void* obj) const { return reinterpret_cast<const u8*>(obj) + offset; }

		template<class T>
		const T& getValue(const void* obj) const {
			SGE_ASSERT(sge_typeof<T>() == fieldType);
			return *reinterpret_cast<const T*>(getValuePtr(obj));
		}

		template<class T>
		void setValue(void* obj, const T& value) const {
			SGE_ASSERT(sge_typeof<T>() == fieldType);
			*reinterpret_cast<T*>(getValuePtr(obj)) = value;
		}

		const char* name = "";
		const TypeInfo* fieldType = nullptr;
		intptr_t offset = 0;
	};
}