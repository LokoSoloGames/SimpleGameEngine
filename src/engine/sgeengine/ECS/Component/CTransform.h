#pragma once

#include "../Component.h"

namespace SimpleGameEngine {
	class CTransform : public Component {
		SGE_OBJECT_TYPE(CTransform, Component)
	public:
		Vec3f	position{ 0,0,0 };
		Quat4f	rotation{ 0,0,0,0 };
		Vec3f	localScale{ 1,1,1 };

		Mat4f	localToWorldMatrix();
		void	dirtyMatrix();

		void	setParent(CTransform* parent) { _parent.reset(parent); }

		const SPtr<CTransform> parent() { return _parent; }
	private:
		void	calculateMatrix();
		bool	_isDirtyMatrix = true;
		Mat4f	_localToWorldMatrix;

		SPtr<CTransform> _parent = nullptr;
		Vector<SPtr<CTransform>> children;
	};
}