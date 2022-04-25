#pragma once

#include <vertex/Vertex.h>

namespace SimpleGameEngine {
	class WavefrontObjReader {
	public:
		void loadFile(const StrView filename);

		Vector<Vec3f> m_vertices;
		Vector<Vec2f> m_texture_vertices;
		Vector<Vec3f> m_normal_vertices;
		Vector<u32>	  m_indices;

	private:
		Vector<Vec3f> tmp_vertices;
		Vector<Vec2f> tmp_texture_vertices;
		Vector<Vec3f> tmp_normal_vertices;

		void _readLine(const StrView& line);
		void _readFace(const StrView& str);
		StrView _readFaceVertex(const StrView& str);
	};
}