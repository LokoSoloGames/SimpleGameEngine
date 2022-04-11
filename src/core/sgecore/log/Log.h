#pragma once

#include <sgebase.h>
#include <sgecore/string/Fmt.h>
#include <sgecore/string/String.h>

#define SGE_LOG(...)       do{ SimpleGameEngine::g_log.write(SimpleGameEngine::Log::Level::Info,    __VA_ARGS__); } while(false)
#define SGE_LOG_WARN(...)  do{ SimpleGameEngine::g_log.write(SimpleGameEngine::Log::Level::Warning, __VA_ARGS__); } while(false)
#define SGE_LOG_ERROR(...) do{ SimpleGameEngine::g_log.write(SimpleGameEngine::Log::Level::Error,   __VA_ARGS__); } while(false)

namespace SimpleGameEngine {

class Log : public NonCopyable {
public:

	enum class Level {
		Unknown,
		Info,
		Warning,
		Error,
	};

	template<class... Args>
	void write(Level lv, Args&&... args) {
		TempString tmp;
		FmtTo(tmp, SGE_FORWARD(args)...);
		onWrite(lv, tmp);
	}

	void onWrite(Level lv, StrView str);
};

extern Log g_log;

} //namespace


