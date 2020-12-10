#include "scpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/Direct3D/Direct3DShader.h"

namespace Seacrest {

	Shader* Shader::Create( const std::string& vertexSrc, const std::string& pixelSrc )
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:     SEACREST_ASSERT( false, "RendererAPI::None is currently not supported!" );  return nullptr;
			case RendererAPI::API::Direct3D: return new Direct3DShader( vertexSrc, pixelSrc );
		}

		SEACREST_ASSERT( false, "Unknown RendererAPI!" );
		return nullptr;
	}
}
