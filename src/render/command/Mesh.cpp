#include "Mesh.h"

namespace SimpleGameEngine {
	void Mesh::add(Position3f& pos, Color4b& color) {
		positions.push_back(pos);
		colors.push_back(color);
		_size++;
	}

	u32 Mesh::getSize() {
		return _size;
	}
}