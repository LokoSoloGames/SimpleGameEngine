#pragma once

#include <sgeengine/ECS/Component.h>

namespace SimpleGameEngine {
	class Transform : public Component {
		SGE_OBJECT_TYPE(Transform, Component)
	public:
		Vec3f	position{ 0,0,0 };
		Quat4f	rotation{ 0,0,0,0 };
		Vec3f	localScale{ 1,1,1 };
	};
}