#include "EngineContext.h"

#include <sgeengine/ECS/Component/CTransform.h>


namespace SimpleGameEngine {

	EngineContext* EngineContext::s_instance = nullptr;

	EngineContext* EngineContext::create() {
		SGE_ASSERT(s_instance == nullptr);
		s_instance = new EngineContext();
		return s_instance;
	}

	void EngineContext::destroy() {
		if (s_instance) {
			delete s_instance;
			s_instance = nullptr;
		}
	}

	EngineContext::EngineContext() {
		registerComponentType<CTransform>();
	}

	void EngineContext::_registerComponentType(const TypeInfo* type) {
		_componentTypeMap.insert(type->name, type);
	}

}