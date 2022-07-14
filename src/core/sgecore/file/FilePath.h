#pragma once

#include <sgecore.h>

namespace SimpleGameEngine {

	struct FilePath {
		static StrView	dirname(StrView path);
		static StrView	basename(StrView path);
		static StrView	extension(StrView path);

		static bool		isRealpath(const StrView& path);
		static String	realpath(StrView path);
	};

}