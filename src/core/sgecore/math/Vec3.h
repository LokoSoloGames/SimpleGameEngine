#pragma once

#include "Vec2.h"
#include "Vec3_Basic.h"
#include "Vec3_SSE.h"

namespace SimpleGameEngine {

#ifndef SGE_MATH_USE_SSE
	#error "Please include sgecore-config.h"
#elif SGE_MATH_USE_SSE
	template<class T> using Vec3 = Vec3_SSE<T>;
#else
	template<class T> using Vec3 = Vec3_Basic<T>;
#endif

using Vec3f = Vec3<float>;
using Vec3d = Vec3<double>;

template<> const TypeInfo* sge_typeof<Vec3f>();
template<> const TypeInfo* sge_typeof<Vec3d>();

}
