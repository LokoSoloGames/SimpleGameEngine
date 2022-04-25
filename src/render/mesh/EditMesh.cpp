#include "EditMesh.h"
#include "reader/WavefrontObjReader .h"

namespace SimpleGameEngine {
	void EditMesh::loadObjFile(StrView filename) {
		WavefrontObjReader reader;
		reader.loadFile(filename);

		pos.clear();
		pos.resize(reader.m_vertices.size());
		for (auto i = 0; i < reader.m_vertices.size(); i++) {
			auto v = reader.m_vertices[i];
			pos[i] = Tuple3f(v.x, v.y, v.z);
		}

		uv[0].clear();
		uv[0].resize(reader.m_texture_vertices.size());
		for (auto i = 0; i < reader.m_texture_vertices.size(); i++) {
			auto v = reader.m_texture_vertices[i];
			uv[0][i] = Tuple2f(v.x, v.y);
		}

		color.clear();
		color.resize(reader.m_vertices.size());
		for (auto i = 0; i < reader.m_vertices.size(); i++) {
			color[i] = Color4b(255, 255, 255, 255);
		}

		normal.clear();
		normal.resize(reader.m_normal_vertices.size());
		for (auto i = 0; i < reader.m_normal_vertices.size(); i++) {
			auto v = reader.m_normal_vertices[i];
			normal[i] = Tuple3f(v.x, v.y, v.z);
		}

		tangent.clear();
		binormal.clear();
	}
}