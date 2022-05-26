#pragma once

#include <sgerender.h>

namespace SimpleGameEngine {
	class Material {
	public:
		Material(const RenderMesh& renderMesh, wchar_t* shaderFile) {
			Renderer::current()->compileVertexShader(shaderFile, renderMesh.subMeshes()[0].vertexLayout(), vertexShader, vertexLayout);
			Renderer::current()->compilePixelShader(shaderFile, pixelShader);
		}

		~Material(){
			Renderer::current()->releaseShader(vertexShader);
			vertexShader = nullptr;
			Renderer::current()->releaseShader(pixelShader);
			pixelShader = nullptr;
			Renderer::current()->releaseVertexLayout(vertexLayout);
			vertexLayout = nullptr;
		}

		void* vertexShader = nullptr;
		void* pixelShader = nullptr;
		void* vertexLayout = nullptr;
	};
}