#pragma once

#include <sgebase.h>
#include "Fmt.h"

namespace SimpleGameEngine {

class StringUtil {
public:
	static void binToHex(String& result, Span<u8> data) {
		result.clear();
		appendBinToHex(result, data.data(), data.size());
	}

	static void appendBinToHex(String& result, const u8* data, size_t dataSize);
};

} // namespace

inline
std::ostream& operator<<(std::ostream& s, const SimpleGameEngine::StrView& v) {
	s.write(v.data(), v.size());
	return s;
}

template<>
struct fmt::formatter<SimpleGameEngine::StrView> {
	auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); }
	auto format(const SimpleGameEngine::StrView& v, fmt::format_context& ctx) {
		auto it = *ctx.out();
		for (const auto& c : v) {
			it = c;
			it++;
		}
		return ctx.out();
	}
};

template<>
struct fmt::formatter<SimpleGameEngine::StrViewW> {
	auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); }
	auto format(const SimpleGameEngine::StrViewW& v, fmt::format_context& ctx) {
		auto it = *ctx.out();
		for (const auto& c : v) {
			it = c;
			it++;
		}
		return ctx.out();
	}
};

template<>
struct fmt::formatter<SimpleGameEngine::String> {
	auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); }
	auto format(const SimpleGameEngine::String& v, fmt::format_context& ctx) {
		SimpleGameEngine::StrView view(v.data(), v.size());
		return fmt::format_to(ctx.out(), "{}", view);
	}
};

template<>
struct fmt::formatter<SimpleGameEngine::StringW> {
	auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); }
	auto format(const SimpleGameEngine::StringW& v, fmt::format_context& ctx) {
		SimpleGameEngine::StrViewW view(v.data(), v.size());
		return fmt::format_to(ctx.out(), "{}", view);
	}
};
