#pragma once

#include "Vertex.h"

namespace SimpleGameEngine {

class VertexLayoutManager {
public:
	VertexLayoutManager();

	static VertexLayoutManager* current();

	const VertexLayout* getLayout(VertexType type);

	VertexLayout* _createLayout(VertexType type);

	template<class VERTEX>
	void registerLayout() {
		VertexLayout* layout = _createLayout(VERTEX::kType);
		layout->stride = sizeof(VERTEX);
		layout->type = VERTEX::kType;
		VERTEX::onRegister(layout);
	}

private:
	Map<VertexType, VertexLayout>	_table;
};

}