#include "FieldInfo.h"

namespace SimpleGameEngine {
	void FieldsEnumerator::Iterator::operator++() {
		if (!typeInfo) return;

		fieldIndex++;
		for (;;) {
			if (!typeInfo) return;
			if (fieldIndex < typeInfo->fieldCount) return;

			typeInfo = typeInfo->base;
			fieldIndex = 0;
		}
	}
	
	const FieldInfo& FieldsEnumerator::Iterator::operator*() {
		return typeInfo->fieldsArray[fieldIndex];
	}
}