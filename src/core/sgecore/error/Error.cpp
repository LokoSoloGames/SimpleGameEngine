#include "Error.h"
#include <sgecore/log/Log.h>

namespace SimpleGameEngine {

	Error::Error(const SrcLoc& loc, StrView msg)
		: _loc(loc), _msg(msg)
	{
		SGE_LOG("Error: {}", msg);
		SGE_ASSERT(false);
	}

} // namespace
