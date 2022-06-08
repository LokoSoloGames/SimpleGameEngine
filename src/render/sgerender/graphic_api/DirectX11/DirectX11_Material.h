#pragma once

#include <sgerender/material/Material.h>
#include "DirectX11_Shader.h"

namespace SimpleGameEngine {
	class DirectX11_Material : public Material {
	public:

	private:
		DirectX11_Shader* shader() { return static_cast<DirectX11_Shader*>(_shader.ptr()); }
	};
}