#pragma once

#if SGE_CPLUSPLUS_17
	#define SGE_FALLTHROUGH		[[fallthrough]]
	#define SGE_NODISCARD		[[nodiscard]]
#else
	#define SGE_FALLTHROUGH
	#define SGE_NODISCARD
#endif

#define SGE_COMMA ,
#define SGE_EMPTY
#define SGE_ARGS(...) __VA_ARGS__
#define SGE_STRINGIFY(...)	#__VA_ARGS__

#define SGE_IDENTITY(x) x
#define SGE_CALL(M, ARGS) SGE_IDENTITY( M(ARGS) )

#define SGE_VA_ARGS_COUNT(...) SGE_IDENTITY( SGE_VA_ARGS_COUNT_INTERNAL(__VA_ARGS__, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1) )
#define SGE_VA_ARGS_COUNT_INTERNAL(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, COUNT, ...) COUNT

#define SGE_DUMP_VAR_1(v0)				do{ SGE_LOG("DUMP_VAR: {}=[{}]",								#v0, (v0)); } while(false)
#define SGE_DUMP_VAR_2(v0, v1)			do{ SGE_LOG("DUMP_VAR: {}=[{}], {}=[{}]",						#v0, (v0), #v1, (v1)); } while(false)
#define SGE_DUMP_VAR_3(v0, v1, v2)		do{ SGE_LOG("DUMP_VAR: {}=[{}], {}=[{}], {}=[{}]",				#v0, (v0), #v1, (v1), #v2, (v2)); } while(false)
#define SGE_DUMP_VAR_4(v0, v1, v2, v3)	do{ SGE_LOG("DUMP_VAR: {}=[{}], {}=[{}], {}=[{}], {}=[{}]",		#v0, (v0), #v1, (v1), #v2, (v2), #v3, (v3)); } while(false)

#define SGE_DUMP_VAR_SELECT(COUNT) SGE_DUMP_VAR_##COUNT
#define SGE_DUMP_VAR(...) SGE_IDENTITY(SGE_CALL(SGE_DUMP_VAR_SELECT, SGE_VA_ARGS_COUNT(__VA_ARGS__) (__VA_ARGS__)))

#define SGE_DUMP_HEX(v) \
	do{ \
		String tmp; \
		StringUtil::binToHex(tmp, v); \
		SGE_LOG("DUMP_HEX: {}\n{}", #v, tmp); \
	} while(false) \
//------

#define SGE_FORWARD(a)	::std::forward< decltype(a) >(a)
#define SGE_ASSERT(...)	assert(__VA_ARGS__)

#if _DEBUG
#define SGE_LOC	SrcLoc(__FILE__, __LINE__, SGE_FUNC_NAME_SZ)
#else
#define SGE_LOC	SrcLoc()
#endif

#define SGE_ENUM_BITWISE_OPERATOR(T) \
	constexpr T operator~ (T  a)      { return static_cast<T>(~enumInt(a)); } \
	constexpr T operator| (T  a, T b) { return static_cast<T>(enumInt(a) | enumInt(b)); } \
	constexpr T operator& (T  a, T b) { return static_cast<T>(enumInt(a) & enumInt(b)); } \
	constexpr T operator^ (T  a, T b) { return static_cast<T>(enumInt(a) ^ enumInt(b)); } \
	constexpr void operator|=(T& a, T b) { a = static_cast<T>(enumInt(a) | enumInt(b)); } \
	constexpr void operator&=(T& a, T b) { a = static_cast<T>(enumInt(a) & enumInt(b)); } \
	constexpr void operator^=(T& a, T b) { a = static_cast<T>(enumInt(a) ^ enumInt(b)); } \
//--------

#define SGE_ENUM_ARITHMETIC_OPERATOR(T) \
	constexpr T operator+ (T  a, T b) { return static_cast<T>(enumInt(a) + enumInt(b)); } \
	constexpr T operator- (T  a, T b) { return static_cast<T>(enumInt(a) - enumInt(b)); } \
	constexpr T operator* (T  a, T b) { return static_cast<T>(enumInt(a) * enumInt(b)); } \
	constexpr T operator/ (T  a, T b) { return static_cast<T>(enumInt(a) / enumInt(b)); } \
	constexpr void operator+=(T& a, T b) { a = static_cast<T>(enumInt(a) + enumInt(b)); } \
	constexpr void operator-=(T& a, T b) { a = static_cast<T>(enumInt(a) - enumInt(b)); } \
	constexpr void operator*=(T& a, T b) { a = static_cast<T>(enumInt(a) * enumInt(b)); } \
	constexpr void operator/=(T& a, T b) { a = static_cast<T>(enumInt(a) / enumInt(b)); } \
//--------

#define SGE_ENUM_ARITHMETIC_OPERATOR_INT(T) \
	constexpr T operator+ (T  a, int b) { return static_cast<T>(enumInt(a) + b); } \
	constexpr T operator- (T  a, int b) { return static_cast<T>(enumInt(a) - b); } \
	constexpr T operator* (T  a, int b) { return static_cast<T>(enumInt(a) * b); } \
	constexpr T operator/ (T  a, int b) { return static_cast<T>(enumInt(a) / b); } \
	constexpr void operator+=(T& a, int b) { a = static_cast<T>(enumInt(a) + b); } \
	constexpr void operator-=(T& a, int b) { a = static_cast<T>(enumInt(a) - b); } \
	constexpr void operator*=(T& a, int b) { a = static_cast<T>(enumInt(a) * b); } \
	constexpr void operator/=(T& a, int b) { a = static_cast<T>(enumInt(a) / b); } \
//--------

#define SGE_ENUM_ALL_OPERATOR(T) \
	SGE_ENUM_BITWISE_OPERATOR(T) \
	SGE_ENUM_ARITHMETIC_OPERATOR(T) \
	SGE_ENUM_ARITHMETIC_OPERATOR_INT(T) \
//-------

#define SGE_ENUM_STR__CASE(V) case E::V: return #V;

#define SGE_ENUM_STR(T) \
	inline const char* enumStr(const T& v) { \
		using E = T; \
		switch (v) { \
			T##_ENUM_LIST(SGE_ENUM_STR__CASE) \
			default: return nullptr; \
		} \
	} \
//----

#define SGE_ENUM_TRY_PARSE__CASE(V) if (str == #V) { outValue = E::V; return true; }

#define SGE_ENUM_TRY_PARSE(T) \
	inline bool enumTryParse(T& outValue, StrView str) { \
		using E = T; \
		T##_ENUM_LIST(SGE_ENUM_TRY_PARSE__CASE) \
		return false; \
	} \
//----

#define SGE_ENUM_STR_UTIL(T) \
	SGE_ENUM_STR(T) \
	SGE_ENUM_TRY_PARSE(T) \
	SGE_FORMATTER_ENUM(T) \
//----

#define SGE_NAMED_IO(SE, V)	SE.named_io(#V, V)
