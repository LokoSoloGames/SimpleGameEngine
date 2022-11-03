#pragma once

#include <sgebase.h>

namespace SimpleGameEngine {
	class Object;
	class TypeInfo;

	class FieldInfo {
	public:
		using Getter = const void* (*)(const void* obj);
		using Setter = void (*)(void* obj, const void* value);

		template<class OBJ, class FIELD>
		FieldInfo(	const char* name_,
					FIELD OBJ::* ptr_,
					const FIELD& (*getter_)(const OBJ& obj) = nullptr,
					void (*setter_)(OBJ& obj, const FIELD& field) = nullptr)
			: name(name_)
			, fieldType(sge_typeof<FIELD>())
			, offset(memberOffset(ptr_))
			, getter(reinterpret_cast<Getter>(getter_))
			, setter(reinterpret_cast<Setter>(setter_))
		{ }

		void* getValuePtr(void* obj) const { return reinterpret_cast<u8*>(obj) + offset; }
		const void* getValuePtr(const void* obj) const { return reinterpret_cast<const u8*>(obj) + offset; }

		template<class T>
		const T& getValue(const void* obj) const {
			SGE_ASSERT(sge_typeof<T>() == fieldType);
			if (getter) {
				return *reinterpret_cast<const T*>(getter(obj));
			} else {
				return *reinterpret_cast<const T*>(getValuePtr(obj));
			}
		}

		template<class T>
		void setValue(void* obj, const T& value) const {
			SGE_ASSERT(sge_typeof<T>() == fieldType);
			if (setter) {
				setter(obj, &value);
			} else {
				*reinterpret_cast<T*>(getValuePtr(obj)) = value;
			}
		}

		const char* name = "";
		const TypeInfo* fieldType = nullptr;
		intptr_t offset = 0;

		Getter	getter;
		Setter	setter;
	};
}