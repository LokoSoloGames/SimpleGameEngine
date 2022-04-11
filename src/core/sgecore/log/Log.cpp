#include "Log.h"
#include <iostream>

namespace SimpleGameEngine {

Log g_log;

void Log::onWrite(Level lv, StrView str) {
	std::cout << str << "\n";
}

} // namespace