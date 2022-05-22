#pragma once

#include <sgecore.h>

namespace SimpleGameEngine {

	class NativeUIAppBase : public AppBase {
	public:
		struct CreateDesc {};

		int  run(CreateDesc& desc);
		void quit(int returnCode);
		virtual void willQuit() {}

	protected:
		virtual void onCreate(CreateDesc& desc) {}
		virtual void onRun() {}
		virtual void onUpdate() {}
		virtual	void onQuit() {}

		int _exitCode = 0;
	};

}