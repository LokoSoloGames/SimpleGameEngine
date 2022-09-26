#pragma once

#include "sgebase.h"
#include "app/ConsoleApp.h"
#include "app/ProjectSettings.h"

#include "sgecore/BitUtil.h"

#include "sgecore/serializer/binary/BinDeserializer.h"
#include "sgecore/serializer/binary/BinSerializer.h"

#include "sgecore/allocator/LinearAllocator.h"

#include "sgecore/math/Math.h"
#include "sgecore/math/Tuple2.h"
#include "sgecore/math/Tuple3.h"
#include "sgecore/math/Tuple4.h"
#include "sgecore/math/Vec2.h"
#include "sgecore/math/Vec3.h"
#include "sgecore/math/Vec4.h"
#include "sgecore/math/Rect2.h"
#include "sgecore/math/Mat4.h"
#include "sgecore/math/Quat4.h"
#include "sgecore/math/MathGeometry.h"
#include "sgecore/math/MathCamera3.h"

#include "sgecore/pointer/SPtr.h"
#include "sgecore/pointer/ComPtr.h"

#include "sgecore/graph/Color.h"
#include "sgecore/graph/ColorUtil.h"

#include "sgecore/log/Log.h"
#include "sgecore/error/Error.h"

#include "sgecore/string/String.h"
#include "sgecore/string/Fmt.h"
#include "sgecore/string/UtfUtil.h"

#include "sgecore/file/FilePath.h"
#include "sgecore/file/FileStream.h"
#include "sgecore/file/MemMapFile.h"
#include "sgecore/file/Directory.h"

#include "nativeui/NativeUI.h"
