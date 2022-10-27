#include "CTransform.h"

namespace SimpleGameEngine {

	const TypeInfo* CTransform::s_getType() {
		class TI : public TI_Base {
		public:
			TI() {
				static FieldInfo fi[] = {
					FieldInfo("position",	&This::position),
					FieldInfo("rotation",	&This::rotation),
					FieldInfo("localScale",	&This::localScale),
				};
				setFields(fi);
			}
		};
		static TI ti;
		return &ti;
	}

	void CTransform::calculateMatrix() {
		Mat4f parentMatrix = _parent ? Mat4f::s_identity() : _parent->localToWorldMatrix();
		_localToWorldMatrix = parentMatrix * Mat4f::s_TRS(position, rotation.euler(), localScale);
	}

	Mat4f CTransform::localToWorldMatrix() {
		if (_isDirtyMatrix) {
			calculateMatrix();
			_isDirtyMatrix = false;
		}
		return _localToWorldMatrix;
	}

	void CTransform::dirtyMatrix() {
		_isDirtyMatrix = true;
		for (auto& p : children) {
			p->dirtyMatrix();
		}
	}
}