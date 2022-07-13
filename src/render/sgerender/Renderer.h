#pragma once

#include "RenderCommon.h"
#include "RenderContext.h"
#include "material/Material.h"
#include "textures/Texture.h"

namespace SimpleGameEngine {
	class RenderContext;
	struct RenderContextCreateDesc;

	class RenderGpuBuffer;
	struct RenderGpuBufferCreateDesc;

	class Renderer : public NonCopyable {
	public:
		static Renderer* instance() { return _instance; }

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

		bool vsync() const		{ return _vsync; }

		SPtr<RenderContext>		createContext(RenderContextCreateDesc& desc) { return onCreateContext(desc); }
		SPtr<RenderGpuBuffer>	createGpuBuffer(RenderGpuBufferCreateDesc& desc) { return onCreateGpuBuffer(desc); }
		SPtr<Texture2D>			createTexture2D(Texture2D_CreateDesc& desc) { return onCreateTexture2D(desc); }
		SPtr<Shader>			createShader(StrView filename);
		SPtr<Material>			createMaterial() { return onCreateMaterial(); }

		void onShaderDestroy(Shader* shader);

		struct StockTextures {
			SPtr<Texture2D>	white;
			SPtr<Texture2D>	black;
			SPtr<Texture2D>	red;
			SPtr<Texture2D>	green;
			SPtr<Texture2D>	blue;
			SPtr<Texture2D>	magenta;
			SPtr<Texture2D>	error;
		};

		StockTextures stockTextures;

		SPtr<Texture2D>	createSolidColorTexture2D(const Color4b& color);

	protected:
		static Renderer* _instance;
		bool _vsync : 1;
		virtual SPtr<RenderContext>		onCreateContext(RenderContextCreateDesc& desc) = 0;
		virtual SPtr<RenderGpuBuffer>	onCreateGpuBuffer(RenderGpuBufferCreateDesc& desc) = 0;
		virtual SPtr<Texture2D>			onCreateTexture2D(Texture2D_CreateDesc& desc) = 0;
		virtual SPtr<Shader>			onCreateShader(StrView filename) = 0;
		virtual SPtr<Material>			onCreateMaterial() = 0;

		StringMap<Shader*>	_shaders;
	};
}