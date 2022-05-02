#pragma once

#include "../string/UtfUtil.h"

namespace SimpleGameEngine {

	using FileSize = u64;

	enum class FileMode {
		CreateNew,
		OpenExists,
		OpenOrCreate,
	};

	enum class FileAccess {
		Read,
		ReadWrite,
		WriteOnly,
	};

	enum class FileShareMode {
		None,
		Read,
		Write,
		ReadWrite,
	};
}