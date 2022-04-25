#pragma once

#include <sgebase.h>

namespace SimpleGameEngine {
	inline
	void sgeLoadFile(String& outStr, const StrView filename) {
		auto* fs = fopen(filename.data(), "rb");
		if (!fs)
			throw SGE_ERROR("cannot open file: {0}", filename);

		fseek(fs, 0, SEEK_END);
		auto filesize = ftell(fs);

		fseek(fs, 0, SEEK_SET);

		outStr.resize(filesize);
		if (fread(outStr.data(), filesize, 1, fs) < 0) {
			fclose(fs);
			throw SGE_ERROR("error reading file: {0}", filename);
		}
		fclose(fs);
	}

	inline
	const StrView sgeReadLine(StrView& outLine, const StrView& content) {
		auto idx = content.find_first_of('\n');
		if (idx == StrView::npos) {
			outLine = content;
			return StrView(nullptr, 0);
		} else {
			outLine = content.substr(0, idx);
			return content.substr(idx + 1, content.size() - idx);
		}
	}

	inline
	const StrView sgeReadToken(StrView& outToken, const StrView& content, char delimiter = ' ') {
		// trim spaces
		size_t pos = 0;
		while (pos < content.length() && content[pos] == ' ') {
			pos++;
		}

		auto idx = content.find_first_of(delimiter, pos);
		if (idx == StrView::npos) {
			outToken = content;
			return StrView(nullptr, 0);
		} else {
			outToken = content.substr(pos, idx - pos);
			return content.substr(idx + 1, content.size() - idx);
		}
	}
}