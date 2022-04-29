#include "WavefrontObjReader .h"
#include <sgecore/file/FileReader.h>

namespace SimpleGameEngine {
	void WavefrontObjReader::loadFile(const StrView filename) {
		String fileBuffer;
		sgeLoadFile(fileBuffer, filename);

		StrView buffer = fileBuffer;
		auto m_lineNumber = 0;

		while (!buffer.empty()) {
			StrView lineBuffer;
			auto remaining = sgeReadLine(lineBuffer, buffer);
			m_lineNumber++;
			_readLine(lineBuffer);
			buffer.swap(remaining);
		}

		SGE_LOG("Finish Reading OBJ File: {0} - Total Line: {1}", filename, m_lineNumber);
	}

	void WavefrontObjReader::_readLine(const StrView& line) {
		if (line.empty()) return;
		StrView token;
		StrView	r = sgeReadToken(token, line);
		if (r.empty()) return;

		if (token == "v") {
			Vec3f v;
			float w = 1;
			int ret = sscanf_s(r.data(), "%f %f %f %f", &v.x, &v.y, &v.z, &w);
			if (ret < 3)
				throw SGE_ERROR("error read v");

			v = v / w;
			tmp_vertices.emplace_back(v);
			return;
		}

		if (token == "vt") {
			Vec2f vt;
			int ret = sscanf_s(r.data(), "%f %f", &vt.x, &vt.y);
			if (ret < 2)
				throw SGE_ERROR("error read vt");

			tmp_texture_vertices.emplace_back(vt);
			return;
		}

		if (token == "vn") {
			Vec3f vn;
			int ret = sscanf_s(r.data(), "%f %f %f", &vn.x, &vn.y, &vn.z);
			if (ret < 3)
				throw SGE_ERROR("error read vn");

			tmp_normal_vertices.emplace_back(vn);
			return;
		}

		if (token == "f") {
			_readFace(r);
			return;
		}

		SGE_LOG("WaveFront OBJ Reader Ignored Token: {0}", token);
	}

	void WavefrontObjReader::_readFace(const StrView& str) {
		auto r = _readFaceVertex(str);
		r = _readFaceVertex(r);
		r = _readFaceVertex(r);
	}

	StrView WavefrontObjReader::_readFaceVertex(const StrView& str) {
		if (str.empty()) return str;
		StrView token;
		StrView r = sgeReadToken(token, str);

		int v = 0;
		int vt = 0;
		int vn = 0;

		StrView tmp;
		token = sgeReadToken(tmp, token, '/');

		if (sscanf_s(tmp.data(), "%d", &v) < 1)
			throw SGE_ERROR("error read f");

		if (!token.empty()) {
			token = sgeReadToken(tmp, token, '/');
			if (tmp.size()) {
				if (sscanf_s(tmp.data(), "%d", &vt) < 1)
					throw SGE_ERROR("error read f");
			}

			token = sgeReadToken(tmp, token, '/');
			if (tmp.size()) {
				if (sscanf_s(tmp.data(), "%d", &vn) < 1)
					throw SGE_ERROR("error read f");
			}
		}

		if (v == 0)
			throw SGE_ERROR("error read f v = 0");

		if (v < 0) {
			if (static_cast<size_t>(-v) > tmp_vertices.size())
				throw SGE_ERROR("error read f v");

			m_vertices.emplace_back(tmp_vertices[tmp_vertices.size() - v]);
		}
		else {
			if (static_cast<size_t>(v) > tmp_vertices.size())
				throw SGE_ERROR("error read f v");
			m_vertices.emplace_back(tmp_vertices[v - 1]);
		}

		//---------
		if (vt == 0) {
			m_texture_vertices.emplace_back(Vec2f(0, 0));
		} else if (vt < 0) {
			if (static_cast<size_t>(-vt) > tmp_texture_vertices.size())
				throw SGE_ERROR("error read f vt");
			m_texture_vertices.emplace_back(tmp_texture_vertices[tmp_texture_vertices.size() - vt]);
		} else {
			if (static_cast<size_t>(vt) > tmp_texture_vertices.size())
				throw SGE_ERROR("error read f vt");
			m_texture_vertices.emplace_back(tmp_texture_vertices[vt - 1]);
		}

		//---------
		if (vn == 0) {
			m_normal_vertices.emplace_back(Vec3f(0, 0, 0));
		} else if (vn < 0) {
			if (static_cast<size_t>(-vn) > tmp_normal_vertices.size())
				throw SGE_ERROR("error read f vn");
			m_normal_vertices.emplace_back(tmp_normal_vertices[tmp_normal_vertices.size() - vn]);
		} else {
			if (static_cast<size_t>(vn) > tmp_normal_vertices.size())
				throw SGE_ERROR("error read f vn");
			m_normal_vertices.emplace_back(tmp_normal_vertices[vn - 1]);
		}

		//---------
		m_indices.emplace_back(static_cast<u16>(m_vertices.size() - 1));
		return r;
	}
}