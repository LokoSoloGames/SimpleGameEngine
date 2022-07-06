#pragma once

#include <sgerender/RenderDataType.h>
#include <sgerender/vertex/Vertex.h>
#include <sgecore/serializer/json/JsonUtil.h>

namespace SimpleGameEngine {

	enum class ShaderStageMask {
		None,
		Vertex = 1 << 0,
		Pixel = 1 << 1,
	};

	enum class ShaderPropType {
		None,
		Int,
		Float,
		Vec2f,
		Vec3f,
		Vec4f,
		Color4f,
	};

#define ShaderPropType_ENUM_LIST(E) \
	E(Int) \
	E(Float) \
	E(Vec2f) \
	E(Vec3f) \
	E(Vec4f) \
	E(Color4f) \
//----
	SGE_ENUM_STR_UTIL(ShaderPropType)

	enum class ShaderCullType {
		Back,
		Front,
		None,
	};

#define ShaderCullType_ENUM_LIST(E) \
	E(Back) \
	E(Front) \
	E(None) \
//----
	SGE_ENUM_STR_UTIL(ShaderCullType)

	enum class ShaderBlendType {
		Zero,
		One,
		SrcColor,
		SrcAlpha,
		DstColor,
		DstAlpha,
		OneMinusSrcColor,
		OneMinusSrcAlpha,
		OneMinusDstColor,
		OneMinusDstAlpha,
	};

#define ShaderBlendType_ENUM_LIST(E) \
	E(One) \
	E(Zero) \
	E(SrcColor) \
	E(SrcAlpha) \
	E(DstColor) \
	E(DstAlpha) \
	E(OneMinusSrcColor) \
	E(OneMinusSrcAlpha) \
	E(OneMinusDstColor) \
	E(OneMinusDstAlpha) \
//----
	SGE_ENUM_STR_UTIL(ShaderBlendType)

	enum class ShaderBlendOp {
		Add,
		Sub,
		RevSub,
		Min,
		Max
	};

#define ShaderBlendOp_ENUM_LIST(E) \
	E(Add) \
	E(Sub) \
	E(RevSub) \
	E(Min) \
	E(Max) \
//----
	SGE_ENUM_STR_UTIL(ShaderBlendOp)

	enum class ShaderDepthTest {
		Never,
		Less,
		Greater,
		LEqual,
		GEqual,
		Equal,
		NotEqual,
		Always,
	};

#define ShaderDepthTest_ENUM_LIST(E) \
	E(Never) \
	E(Less) \
	E(Greater) \
	E(LEqual) \
	E(GEqual) \
	E(Equal) \
	E(NotEqual) \
	E(Always) \
//----
	SGE_ENUM_STR_UTIL(ShaderDepthTest)

	struct ShaderInfo {
		struct Prop {
			ShaderPropType	propType = ShaderPropType::None;
			String		name;
			String		displayName;
			String		defaultValue;

			template<class SE>
			void onJson(SE& se) {
				SGE_NAMED_IO(se, name);
				SGE_NAMED_IO(se, displayName);
				SGE_NAMED_IO(se, defaultValue);
			}
		};

		struct Pass {
			struct Blend {
				bool			enabled;
				ShaderBlendOp	op;
				ShaderBlendType src;
				ShaderBlendType dst;

				template<class SE>
				void onJson(SE& se) {
					SGE_NAMED_IO(se, enabled);
					SGE_NAMED_IO(se, op);
					SGE_NAMED_IO(se, src);
					SGE_NAMED_IO(se, dst);
				}
			};

			String name;
		
			bool depthWrite = true;
			ShaderDepthTest depthTest = ShaderDepthTest::LEqual;
			ShaderCullType cull = ShaderCullType::Back;
			Blend blendRGB;
			Blend blendAlpha;

			String vsFunc;
			String psFunc;

			template<class SE>
			void onJson(SE& se) {
				SGE_NAMED_IO(se, name);
				SGE_NAMED_IO(se, depthWrite);
				SGE_NAMED_IO(se, depthTest);
				SGE_NAMED_IO(se, cull);
				SGE_NAMED_IO(se, blendRGB);
				SGE_NAMED_IO(se, blendAlpha);
				SGE_NAMED_IO(se, vsFunc);
				SGE_NAMED_IO(se, psFunc);
			}
		};

		String shaderName;
		Vector_<Prop, 8>	props;
		Vector_<Pass, 1>	passes;

		void clear();

		template<class SE>
		void onJson(SE& se) {
			SGE_NAMED_IO(se, props);
			SGE_NAMED_IO(se, passes);
		}
	};

	class ShaderStageInfo {
	public:
		using DataType = RenderDataType;

		void loadFile(StrView filename_) {
			filename = filename_;
			JsonUtil::readFile(filename_, *this);
		}

		String	filename;
		String	profile;
		ShaderStageMask	stage = ShaderStageMask::None;

		class Param {
		public:
			String name;
			DataType dataType;
			i16	bindPoint = 0;
			i16	bindCount = 0;

			template<class SE>
			void onJson(SE& se) {
				SGE_NAMED_IO(se, name);
				SGE_NAMED_IO(se, dataType);
				SGE_NAMED_IO(se, bindPoint);
				SGE_NAMED_IO(se, bindCount);
			}
		};

		class Input {
		public:
			String			name;
			VertexSemantic	semantic = VertexSemantic::None;
			RenderDataType	dataType = RenderDataType::None;

			template<class SE>
			void onJson(SE& se) {
				SGE_NAMED_IO(se, name);
				SGE_NAMED_IO(se, semantic);
				SGE_NAMED_IO(se, dataType);
			}
		};

		class Variable {
		public:
			String		name;
			size_t		offset = 0;
			DataType	dataType = DataType::None;
			bool		rowMajor = true;

			template<class SE>
			void onJson(SE& se) {
				SGE_NAMED_IO(se, name);
				SGE_NAMED_IO(se, offset);
				SGE_NAMED_IO(se, dataType);
			}
		};

		class ConstBuffer {
		public:
			String			name;
			i16				bindPoint = 0;
			i16				bindCount = 0;
			size_t			dataSize = 0;
			Vector_<Variable, 4>	variables;

			template<class SE>
			void onJson(SE& se) {
				SGE_NAMED_IO(se, name);
				SGE_NAMED_IO(se, bindPoint);
				SGE_NAMED_IO(se, bindCount);
				SGE_NAMED_IO(se, dataSize);
				SGE_NAMED_IO(se, variables);
			}

			const Variable* findVariable(StrView propName) const {
				for (auto& v : variables) {
					if (v.name == propName) return &v;
				}
				return nullptr;
			}
		};

		Vector_<Input, 8>		inputs;
		Vector_<Param, 8>		params;
		Vector_<ConstBuffer, 4>	constBuffers;

		//----------
		class Texture {
		public:
			String		name;
			i16			bindPoint = 0;
			i16			bindCount = 0;
			DataType	dataType = DataType::None;

			template<class SE>
			void onJson(SE& se) {
				SGE_NAMED_IO(se, name);
				SGE_NAMED_IO(se, bindPoint);
				SGE_NAMED_IO(se, bindCount);
				SGE_NAMED_IO(se, dataType);
			}
		};
		Vector_<Texture, 8>		textures;

		//----------
		class Sampler {
		public:
			String		name;
			i16			bindPoint = 0;
			i16			bindCount = 0;
			RenderDataType	dataType = RenderDataType::None;

			template<class SE>
			void onJson(SE& se) {
				SGE_NAMED_IO(se, name);
				SGE_NAMED_IO(se, bindPoint);
				SGE_NAMED_IO(se, bindCount);
				SGE_NAMED_IO(se, dataType);
			}
		};
		Vector_<Sampler, 8>		samplers;

		//----------
		template<class SE>
		void onJson(SE& se) {
			SGE_NAMED_IO(se, profile);
			SGE_NAMED_IO(se, inputs);
			SGE_NAMED_IO(se, params);
			SGE_NAMED_IO(se, constBuffers);
			SGE_NAMED_IO(se, textures);
			SGE_NAMED_IO(se, samplers);
		}
	};
}