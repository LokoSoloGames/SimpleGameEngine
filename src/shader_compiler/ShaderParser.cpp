#include "ShaderParser.h"

namespace SimpleGameEngine {

	void ShaderParser::readFile(ShaderInfo& outInfo, StrView filename) {
		_memMapFile.open(filename);
		readMem(outInfo, _memMapFile, filename);
	}

	void ShaderParser::readMem(ShaderInfo& outInfo, ByteSpan data, StrView filename) {
		outInfo.clear();
		_outInfo = &outInfo;
		reset(data, filename);

		if (_token.isIdentifier("Shader")) {
			_readShader();
		} else {
			error("missing Shader tag");
		}
	}

	void ShaderParser::_readShader() {
		nextToken();
		if (_token.isString()) { readString(_outInfo->shaderName); }
		expectOperator("{");

		for(;;) {
			if (_token.isOperator("}"))		{ nextToken(); break; }
			if (_token.isNewline())			{ nextToken(); continue; }
			if (_token.isIdentifier("Properties"))	{ _readProperties(); continue; }
			if (_token.isIdentifier("Pass"))		{ _readPass(); continue; }
			return errorUnexpectedToken();
		}
	}

	void ShaderParser::_readProperties() {
		nextToken();
		expectOperator("{");

		for(;;) {
			skipNewlineTokens();
			if (_token.isOperator("}"))	{ nextToken(); break; }
			_readProperty();
		}
	}

	void ShaderParser::_readProperty() {
		auto& o = _outInfo->props.emplace_back();

		// optional attribute
		if (_token.isOperator("[")) {
			nextToken();
			while (!_token.isNone()) {
				skipNewlineTokens();

				if (_token.isIdentifier("DisplayName")) {
					nextToken();
					expectOperator("=");
					readString(o.displayName);
				}
				if (_token.isOperator("]"))	{ nextToken(); break; }

				expectOperator(",");
			}
		}

		skipNewlineTokens();

		{
			// prop type
			_readEnum(o.propType);

			// prop name
			readIdentifier(o.name);
		}

		// optional defaultValue
		if (_token.isOperator("=")) {
			nextToken();
			while (!_token.isNone()) {
				if (_token.isNewline())	{ break; }
				o.defaultValue += _token.str;
				nextToken();
			}
		}

		if (!_token.isNewline()) {
			errorUnexpectedToken();
		}
		nextToken();
	}

	void ShaderParser::_readPass() {
		nextToken();
		auto& o = _outInfo->passes.emplace_back();

		if (_token.isString()) {
			readString(o.name);
		}
		expectOperator("{");

		for(;;) {
			if (_token.isOperator("}"))	{ nextToken(); break; }
			if (_token.isNewline())		{ nextToken(); continue; }

			if (_token.isIdentifier("DepthWrite"))	{ nextToken(); _readBoolean(o.depthWrite);	continue; }
			if (_token.isIdentifier("DepthTest"))	{ nextToken(); _readEnum(o.depthTest);		continue; }
			if (_token.isIdentifier("Cull"))		{ nextToken(); _readEnum(o.cull);			continue; }

			if (_token.isIdentifier("BlendRGB")) { 
				nextToken(); 
				_readEnum(o.blendRGB.op);
				_readEnum(o.blendRGB.src);
				_readEnum(o.blendRGB.dst);
				o.blendRGB.enabled = true;
				continue;
			}

			if (_token.isIdentifier("BlendAlpha")) {
				nextToken(); 
				_readEnum(o.blendAlpha.op);
				_readEnum(o.blendAlpha.src);
				_readEnum(o.blendAlpha.dst);
				o.blendAlpha.enabled = true;
				continue;
			}
			
			if (_token.isIdentifier("VsFunc")) { nextToken(); readIdentifier(o.vsFunc); continue; }
			if (_token.isIdentifier("PsFunc")) { nextToken(); readIdentifier(o.psFunc); continue; }
			return errorUnexpectedToken();
		}
	}

	void ShaderParser::_readBoolean(bool& b) {
		if (!token().isIdentifier()) {
			errorUnexpectedToken();
			return;
		}

		if (_token.str == "true" || _token.str == "TRUE") {
			b = true;
			nextToken();
			return;
		}

		if (_token.str == "false" || _token.str == "FALSE") {
			b = false;
			nextToken();
			return;
		}
		error("read boolean [{}]", _token.str);
	}
}