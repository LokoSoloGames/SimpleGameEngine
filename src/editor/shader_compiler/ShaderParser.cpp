#include "ShaderParser.h"

namespace SimpleGameEngine {
	ShaderParser::ShaderManifest ShaderParser::parseShader(Span<const u8> src) {
		_remaining = StrView(reinterpret_cast<const char*>(src.data()), src.size());
		_lineNumber = 1;

		// Parse Shader Keyword
		_nextToken();
		_checkTokenType(TokenType::Identifier);
		_checkTokenValue("Shader");

		// Parse Shader Name
		_nextToken();
		_checkTokenType(TokenType::Operator);

		if (_token.value == "\"") {
			_nextToken();
			_checkTokenType(TokenType::Identifier);
			output.shaderName = _token.value;
			_nextToken();
			_checkTokenType(TokenType::Operator);
			_checkTokenValue("\"");
			_nextToken();
			_checkTokenType(TokenType::Operator);
			_checkTokenValue("{");
		} else if (_token.value == "{") {
			output.shaderName = "";
		} else {
			_error("Invalid Character after Shader Keyword");
		}

		// Parse Scope
		while (_parseScope()) {	}
		output.shader = _remaining;
		return output;
	}

	bool ShaderParser::_parseScope() {
		_nextToken();
		if (_token.type == TokenType::Operator) {
			_checkTokenValue("}");
			return false;
		}
		_checkTokenType(TokenType::Identifier);
		if (_token.value == "Properties") {
			_parseProperties();
		} else if (_token.value == "Pass") {
			_parsePass();
		} else {
			_error(Fmt("Invalid Keyword: {}", _token.value));
		}
		return true;
	}

	bool ShaderParser::_nextToken() {
		_token.type = TokenType::None;
		_token.value = StrView();
		bool isNextLine = false;
		while (_token.type == TokenType::None && _remaining.size()) {
			_currentLine = StringUtil::trimChar(_currentLine, " \t\r");
			if (_currentLine.empty()) {
				auto ret = StringUtil::splitByChar(_remaining, '\n');
				_currentLine = ret.first;
				_remaining = ret.second;
				_lineNumber++;
				isNextLine = true;
			} else {
				_readTokenInCurrentLine();
			}
		}
		return isNextLine;
	}

	void ShaderParser::_readTokenInCurrentLine() {
		_currentLine = StringUtil::trimChar(_currentLine, " \t\r");
		
		if (_currentLine.empty())
			_error("Failed to read token: line is empty");

		if (isdigit(_currentLine[0])) {
			auto ret = StringUtil::trimChar(_currentLine, "0123456789.");
			_token.type = TokenType::Number;
			_token.value = _currentLine.substr(0, ret.begin() - _currentLine.data());
			_currentLine = ret;
			return;
		} else if (isalpha(_currentLine[0])) {
			auto ret = StringUtil::stopByAlphaNumeric(_currentLine, "_");
			_token.type = TokenType::Identifier;
			_token.value = ret.first;
			_currentLine = ret.second;
			return;
		} else if (_currentLine.substr(0, 2) == "//") {
			// ignore whole line
			_currentLine = StrView();
			return;
		} else {
			auto* s = _currentLine.begin();
			auto* e = _currentLine.end();
			auto* p = s;
			StrView seperator = " \t\r";
			for (; p < e; p++) {
				if (isalnum(*p) || StringUtil::hasChar(seperator, *p)) break;
			}
			_token.type = TokenType::Operator;
			_token.value = StrView(s, p - s);
			_currentLine = StrView(p, e - p);
			return;
		}
	}

	void ShaderParser::_parseProperties() {
		// TEMP <- TODO
		_nextToken();
		_checkTokenType(TokenType::Operator);
		_checkTokenValue("{");
		while (!(_token.type == TokenType::Operator && _token.value == "}")) {
			_nextToken();
		}
	}

	void ShaderParser::_parsePass() {
		_nextToken();
		_checkTokenType(TokenType::Operator);
		_checkTokenValue("{");
		ShaderManifest::Pass _pass;
		// Parse Tags
		_nextToken();
		while (!(_token.type == TokenType::Operator && _token.value == "}")) {
			_checkTokenType(TokenType::Identifier);
			if (_token.value == "VsFunc") {
				_nextToken();
				_checkTokenType(TokenType::Identifier);
				_pass.vsName = _token.value;
				while (!_nextToken()) {} // eat the rest of the line
			} else if (_token.value == "PsFunc") {
				_nextToken();
				_checkTokenType(TokenType::Identifier);
				_pass.psName = _token.value;
				while (!_nextToken()) {} // eat the rest of the line
			} else { // ignore for now
				while (!_nextToken()) {} // eat the rest of the line
			}
		}
		output.pass.emplace_back(_pass);
	}

	inline void ShaderParser::_checkTokenType(TokenType type) {
		if (_token.type != type) {
			_error("Invalid Syntax");
		}
	}

	inline void ShaderParser::_checkTokenValue(StrView value) {
		if (_token.value.compare(value) != 0) {
			_error(Fmt("Invalid Syntax: Require {}", value));
		}
	}
}