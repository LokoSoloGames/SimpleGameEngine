#pragma once

#include "VertexLayout.h"

namespace SimpleGameEngine {
	class EditMesh {
		public:
			static const u8 kUvCountMax = 8;

			Vector<Tuple3f>	pos;
			Vector<Tuple2f>	uv[kUvCountMax];
			Vector<Color4b> color;
	};
}