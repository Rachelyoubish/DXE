#include "scpch.h"
#include "InputLayout.h"

#include "Renderer.h"
#include "Platform/Direct3D/Direct3DInputLayout.h"

namespace Seacrest {

	InputLayout* InputLayout::Create( const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
		ID3DBlob* pVertexShaderBytecode )
	{
		switch (Renderer::GetCurrentAPI())
		{
			case RendererAPI::None:     SEACREST_ASSERT( false, "RendererAPI::None is currently not supported!" );  return nullptr;
			case RendererAPI::Direct3D: return new Direct3DInputLayout( layout, pVertexShaderBytecode );
		}

		SEACREST_ASSERT( false, "Unknown RendererAPI!" );
		return nullptr;
	}
}
