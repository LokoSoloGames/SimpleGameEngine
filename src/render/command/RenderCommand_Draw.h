#pragma once

#include <mesh/RenderMesh.h>
#include <Renderer.h>

namespace SimpleGameEngine {
	struct ShaderPass {
		void* vertexShader = nullptr;
		void* pixelShader = nullptr;
		void* vertexLayout = nullptr;

		ShaderPass(const RenderMesh& renderMesh, wchar_t* shaderFile) {
			Renderer::current()->compileVertexShader(shaderFile, renderMesh.layout(), vertexShader, vertexLayout);
			Renderer::current()->compilePixelShader(shaderFile, pixelShader);
		}

		~ShaderPass(){
			Renderer::current()->releaseShader(vertexShader);
			vertexShader = nullptr;
			Renderer::current()->releaseShader(pixelShader);
			pixelShader = nullptr;
			Renderer::current()->releaseVertexLayout(vertexLayout);
			vertexLayout = nullptr;
		}
	};

	class RenderCommand_Draw {
	public:
		RenderMesh* renderMesh;
		ShaderPass* shaderPass;
	};
}