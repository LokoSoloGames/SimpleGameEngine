#include <sgecore.h>

namespace SimpleGameEngine {
	class ShaderParser : public NonCopyable {
	public:
		struct ShaderManifest;

		ShaderManifest parseShader(Span<const u8> src);
	private:
		enum class TokenType : u8 {
			None,
			Identifier,
			Number,
			Operator,
		};

		struct Token {
			TokenType type;
			StrView value;
		};

		bool _parseScope();
		void _parseProperties();
		void _parsePass();

		bool _nextToken(); // return isNextLine
		void _readTokenInCurrentLine();
		Token _token;
		StrView _remaining;
		StrView _currentLine;
		int _lineNumber;


		inline void _checkTokenType(TokenType type);
		inline void _checkTokenValue(StrView value);
		void _error(StrView msg) { throw SGE_ERROR("{}: {}", _lineNumber, msg); }

		ShaderManifest& output;
	};
}