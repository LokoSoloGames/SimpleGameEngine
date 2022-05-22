#include "EditMesh.h"

namespace SimpleGameEngine {
	void EditMesh::clear() {
		indices.clear();

		pos.clear();
		for (auto& t : uv) {
			t.clear();
		}
		color.clear();
		normal.clear();
		tangent.clear();
		binormal.clear();
	}
}