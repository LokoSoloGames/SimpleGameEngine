#pragma once

#include <vertex/Vertex.h>

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

		RenderContext*	createContext(RenderContextCreateDesc& desc) { return onCreateContext(desc); }
		RenderGpuBuffer* createGpuBuffer(RenderGpuBufferCreateDesc& desc) { return onCreateGpuBuffer(desc); }
		void compileVertexShader(wchar_t* fileName, const VertexLayout* layout, void*& pShader, void*& pVertexLayout) { onCompileVertexShader(fileName, layout, pShader, pVertexLayout); }
		void compilePixelShader(wchar_t* fileName, void*& pShader) { onCompilePixelShader(fileName, pShader); }
		void releaseShader(void* pShader) { onReleaseShader(pShader); }
		void releaseVertexLayout(void* pVertexLayout) { onReleaseVertexLayout(pVertexLayout); }

		bool vsync() const		{ return _vsync; }

	protected:
		static Renderer*	_current;
		bool _vsync : 1;
		virtual RenderContext* onCreateContext(RenderContextCreateDesc& desc) = 0;
		virtual RenderGpuBuffer* onCreateGpuBuffer(RenderGpuBufferCreateDesc& desc) = 0;
		virtual void onCompileVertexShader(wchar_t* fileName, const VertexLayout* layout, void*& pShader, void*& pVertexLayout) = 0;
		virtual void onCompilePixelShader(wchar_t* fileName, void*& pShader) = 0;
		virtual void onReleaseShader(void* pShader) = 0;
		virtual void onReleaseVertexLayout(void* pVertexLayout) = 0;
	};
}