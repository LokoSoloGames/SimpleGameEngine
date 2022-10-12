#pragma once
#include <sgebase.h>
#include <fmt/format.h>

#if SGE_COMPILER_VC
#if _DEBUG
#pragma comment(lib, "fmtd.lib")
#else
#pragma comment(lib, "fmt.lib")
#endif
#endif

#define SGE_FORMATTER_T(TEMPLATE_ARGS, CLASS) \
	} /* namespace SimpleGameEngine */ \
	template<TEMPLATE_ARGS> \
	struct fmt::formatter<SimpleGameEngine::CLASS> { \
		auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); } \
		auto format(const SimpleGameEngine::CLASS& v, fmt::format_context& ctx) { \
			v.onFormat(ctx); \
			return ctx.out(); \
		} \
	}; \
	namespace SimpleGameEngine { \
//------

#define SGE_FORMATTER(CLASS) SGE_FORMATTER_T(SGE_EMPTY, CLASS)

#define SGE_FORMATTER_ENUM(T) \
	} /* namespace SimpleGameEngine */ \
	template<> \
	struct fmt::formatter<SimpleGameEngine::T> { \
		auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); } \
		auto format(const SimpleGameEngine::T& v, fmt::format_context& ctx) { \
			return fmt::format_to(ctx.out(), "{}", SimpleGameEngine::enumStr(v)); \
		} \
	}; \
	namespace SimpleGameEngine { \
//-----

namespace SimpleGameEngine {

	template<class STR, class... ARGS> inline
		void FmtTo(STR& outStr, ARGS&&... args) {
		fmt::format_to(std::back_inserter(outStr), SGE_FORWARD(args)...);
	}

	template<class... ARGS> inline
		TempString Fmt(ARGS&&... args) {
		TempString o;
		FmtTo(o, SGE_FORWARD(args)...);
		return o;
	}

}