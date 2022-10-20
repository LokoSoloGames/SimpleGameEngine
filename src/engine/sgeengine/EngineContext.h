#pragma once

namespace SimpleGameEngine {

	class EngineContext {
	public:
		static EngineContext*	create();
		static void				destroy();

		static EngineContext*	instance() { return s_instance; }

		template<class T>
		void registerComponentType() { _registerComponentType(sge_typeof<T>()); }
		void _registerComponentType(const TypeInfo* type);

	private:
		static EngineContext*	s_instance;

		StringMap<const TypeInfo*>	_componentTypeMap;

		EngineContext();
	};

}
