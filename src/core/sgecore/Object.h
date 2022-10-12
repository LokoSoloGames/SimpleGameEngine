#pragma once

#include "reflection/Reflection.h"

namespace SimpleGameEngine {
	class Object;
	template<> const TypeInfo* sge_typeof<Object>();

	class Object : public RefCountBase {
	public:
		virtual ~Object() = default;
		virtual const TypeInfo* getType() const { return sge_typeof<Object>(); }
	};


	inline const TypeInfo* sge_typeof(Object& obj) {
		return obj.getType();
	}
}
