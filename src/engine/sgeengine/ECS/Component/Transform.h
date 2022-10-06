#pragma once

#include <sgeengine/ECS/Component.h>

namespace SimpleGameEngine {
	class Transform : public Component {
		SGE_TYPE(Transform, Component)
	public:

	private:
		Vec3f  _position;
		Quat4f _rotation;
		Vec3f  _localScale;
	};
}