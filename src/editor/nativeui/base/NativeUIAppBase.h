#pragma once

#include <sgebase.h>

namespace SimpleGameEngine {

	class NativeUIAppBase : public NonCopyable {
	public:
		class CreateDesc {};

		int  run(CreateDesc& desc);
		void quit(int returnCode);
		virtual void willQuit() {}

	protected:
		virtual void onCreate(CreateDesc& desc) {}
		virtual void onRun() {}
		virtual	void onQuit() {}

		int _exitCode = 0;
	};

}