#include "Scene.h"

namespace SimpleGameEngine {

	Scene::Scene() {
		_rootEntity = addEntity();
	}

	Scene::~Scene() {
	}

	Entity* Scene::addEntity() {
		auto* p  = new Entity();
		p->_internalInit(this, static_cast<EntityId>(++_nextEntityId));
		_entities.emplace_back(p);
		_entityIdMap.emplace(p->id(), p);

		if (_rootEntity) {
			_rootEntity->transform()->addChild(p->transform());
		}
		return p;
	}

	Entity* Scene::addEntity(StrView name) {
		auto p = addEntity();
		p->setName(name);
		return p;
	}

}
