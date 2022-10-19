#pragma once

#include <type_traits>

namespace SimpleGameEngine {
	class Object;
	class FieldInfo;
	class FieldsEnumerator;

	class TypeInfo {
	public:
		const char* name = "";
		const TypeInfo* base = nullptr;
		size_t dataSize = 0;

		Span<const FieldInfo> fields() const { return _fields; }

		typedef Object* (*Creator)();
		
		const TypeInfo* elementType = nullptr;

		Object* createObject() const {
			if (!creator) return nullptr;
			return creator();
		}

		bool isKindOf(const TypeInfo* t) const {
			auto* p = this;
			while (p) {
				if (p == t) return true;
				p = p->base;
			}
			return false;
		}

		template<class R>
		bool isKindOf() const {
			return isKindOf(sge_typeof<R>());
		}
	protected:
		Creator creator;
		Span<const FieldInfo>	_fields;
	};

	template<class T>
	class TypeInfoInitNoBase : public TypeInfo {
	public:
		TypeInfoInitNoBase(const char* name_) {
			name = name_;
			dataSize = sizeof(T);
		};

		template<size_t N>
		void setFields(const FieldInfo(&fi)[N]) {
			_fields = fi;
		}
	};

	template<class T> inline
	static Object* TypeCreator() {
		return new T();
	}

	template<class T, class Base>
	class TypeInfoInit : public TypeInfoInitNoBase<T> {
	public:
		TypeInfoInit(const char* name_) : TypeInfoInitNoBase<T>(name_) {
			static_assert(std::is_base_of<Base, T>::value, "invalid base class");
			this->base = sge_typeof<Base>();

			this->creator = &TypeCreator<T>;
		};
	};
}