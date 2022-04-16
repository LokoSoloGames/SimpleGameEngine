#pragma once

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

		bool vsync() const		{ return _vsync; }

	protected:
		static Renderer*	_current;
		bool _vsync : 1;
	};
}