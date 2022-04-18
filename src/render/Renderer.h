#pragma once

#include <command/VertexLayout.h>

namespace SimpleGameEngine {
	class RenderContext;
	struct RenderContextCreateDesc;

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

		virtual RenderContext*	onCreateContext(RenderContextCreateDesc& desc) = 0;
		void createBuffer(void* data, size_t& byteWidth, void*& pBuffer) { onCreateBuffer(data, byteWidth, pBuffer); }
		void releaseBuffer(void* pBuffer) { onReleaseBuffer(pBuffer); }
		void compileVertexShader(wchar_t* fileName, VertexLayout* layout, void*& pShader, void*& pVertexLayout) { onCompileVertexShader(fileName, layout, pShader, pVertexLayout); }
		void compilePixelShader(wchar_t* fileName, void*& pShader) { onCompilePixelShader(fileName, pShader); }
		void releaseShader(void* pShader) { onReleaseShader(pShader); }
		void releaseVertexLayout(void* pVertexLayout) { onReleaseVertexLayout(pVertexLayout); }

		bool vsync() const		{ return _vsync; }

	protected:
		static Renderer*	_current;
		bool _vsync : 1;
		virtual void onCreateBuffer(void* data, size_t& byteWidth, void*& pBuffer) {}
		virtual void onReleaseBuffer(void* pBuffer) {}
		virtual void onCompileVertexShader(wchar_t* fileName, VertexLayout* layout, void*& pShader, void*& pVertexLayout) {}
		virtual void onCompilePixelShader(wchar_t* fileName, void*& pShader) {}
		virtual void onReleaseShader(void* pShader) {}
		virtual void onReleaseVertexLayout(void* pVertexLayout) {}
	};
}