#include "VertexLayoutManager.h"

namespace SimpleGameEngine {

VertexLayoutManager* VertexLayoutManager::s_current = nullptr;

VertexLayoutManager::VertexLayoutManager() {
	registerLayout<Vertex_Pos>();
	registerLayout<Vertex_PosColor>();

	#define MY_REGISTER_VERTEX_UV(T) \
		registerLayout<T<1>>(); \
		registerLayout<T<2>>(); \
		registerLayout<T<3>>(); \
		registerLayout<T<4>>(); \
	//---
		MY_REGISTER_VERTEX_UV(Vertex_PosUv);
		MY_REGISTER_VERTEX_UV(Vertex_PosColorUv);

		MY_REGISTER_VERTEX_UV(Vertex_PosNormalUv);
		MY_REGISTER_VERTEX_UV(Vertex_PosColorNormalUv);

		MY_REGISTER_VERTEX_UV(Vertex_PosTangentUv);
		MY_REGISTER_VERTEX_UV(Vertex_PosColorTangentUv);

		MY_REGISTER_VERTEX_UV(Vertex_PosBinormalUv);
		MY_REGISTER_VERTEX_UV(Vertex_PosColorBinormalUv);

	#undef MY_REGISTER_VERTEX_UV
}

VertexLayoutManager* VertexLayoutManager::current() {
	static VertexLayoutManager s;
	return &s;
}

const VertexLayout* VertexLayoutManager::getLayout(VertexType type) {
	auto it = _table.find(type);
	if (it == _table.end()) {
		return nullptr;
	}
	return &it->second;
}

VertexLayout* VertexLayoutManager::_createLayout(VertexType type) {
	auto* p = getLayout(type);
	if (p) {
		SGE_ASSERT(false);
	}
	return &_table[type];
}

}