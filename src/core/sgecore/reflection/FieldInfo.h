#pragma once

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

		const char* name = "";
		const TypeInfo* fieldType = nullptr;
		intptr_t offset = 0;
	};

	class FieldsEnumerator {
	public:
		FieldsEnumerator(const TypeInfo* typeInfo_) : typeInfo(typeInfo_) { }

		class Iterator {
		public:
			Iterator(const TypeInfo* typeInfo_, size_t fieldIndex_)
				: typeInfo(typeInfo_), fieldIndex(fieldIndex_) { }

			bool operator!=(const Iterator& r) const {
				return typeInfo != r.typeInfo || fieldIndex != r.fieldIndex;
			}

			void operator++();
			const FieldInfo& operator*();

		private:
			const TypeInfo* typeInfo = nullptr;
			size_t fieldIndex = 0;
		};

		Iterator begin() { return Iterator(typeInfo, 0); }
		Iterator end() { return Iterator(nullptr, 0); }

		const TypeInfo* typeInfo = nullptr;
	};
}