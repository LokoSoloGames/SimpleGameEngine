#pragma once

#include "../base/NativeUIAppBase.h"

#if SGE_OS_WINDOWS
#include <Windows.h>

namespace SimpleGameEngine {

class NativeUIAppWin32 : public NativeUIAppBase {
	using Base = NativeUIAppBase;
public:

	MSG _win32_msg;

protected:
	virtual String	onGetExecutableFilename() override;
	virtual String	onGetCurrentDir() override;
	virtual void	onSetCurrentDir(StrView dir) override;

	virtual void onCreate(CreateDesc& desc) override;
	virtual void onRun	() override;
	virtual	void onQuit	() override;
};

}

#endif