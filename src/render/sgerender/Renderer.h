#pragma once

#include "RenderCommon.h"
#include "RenderContext.h"
#include "material/Material.h"

namespace SimpleGameEngine {
	class RenderContext;
	struct RenderContextCreateDesc;

	class RenderGpuBuffer;
	struct RenderGpuBufferCreateDesc;

	class Renderer : public NonCopyable {
	public:
		static Renderer*	current() { return _current; }

		enum class ApiType {
			None,
			DX11,
			OpenGL,
		};

		struct CreateDesc {
			CreateDesc();
			ApiType		apiType;
			bool multithread : 1;
		};

		static Renderer* create(CreateDesc& desc);

		Renderer();
		virtual ~Renderer();

		SPtr<RenderContext>		createContext(RenderContextCreateDesc& desc) { return onCreateContext(desc); }
		SPtr<RenderGpuBuffer>	createGpuBuffer(RenderGpuBufferCreateDesc& desc) { return onCreateGpuBuffer(desc); }
		SPtr<Shader>			createShader(StrView filename);
		SPtr<Material>			createMaterial() { return onCreateMaterial(); }

		void onShaderDestroy(Shader* shader);

		bool vsync() const		{ return _vsync; }

	protected:
		static Renderer*	_current;
		bool _vsync : 1;
		virtual SPtr<RenderContext>		onCreateContext(RenderContextCreateDesc& desc) = 0;
		virtual SPtr<RenderGpuBuffer>	onCreateGpuBuffer(RenderGpuBufferCreateDesc& desc) = 0;
		virtual SPtr<Shader>			onCreateShader(StrView filename) = 0;
		virtual SPtr<Material>			onCreateMaterial() = 0;

		StringMap<Shader*>	_shaders;
	};
}