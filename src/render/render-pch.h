#pragma once

#include <sgebase.h>
#include <sgecore/log/Log.h>
#include <sgecore/error/Error.h>
#if SGE_OS_WINDOWS
	#define SGE_RENDER_HAS_DX11 	1
	#define SGE_RENDER_HAS_OPENGL 	1
#endif

