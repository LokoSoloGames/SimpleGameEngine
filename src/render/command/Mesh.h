#pragma once

#include "VertexLayout.h"

namespace SimpleGameEngine {
	class Mesh {
		public:
			void add(Position3f& pos, Color4b& color);
			u32 getSize();
			Vector<Position3f> positions;
			Vector<Color4b> colors;
		private:
			u32 _size = 0;
	};
}