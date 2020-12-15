#include "scpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/Direct3D/Direct3DTexture.h"

namespace Seacrest
{
    Ref<Texture2D> Texture2D::Create( const std::string& path )
    {
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:     SEACREST_ASSERT( false, "RendererAPI::None is currently not supported!" );  return nullptr;
			case RendererAPI::API::Direct3D: return std::make_shared<Direct3DTexture2D>(path);
		}

		SEACREST_ASSERT( false, "Unknown RendererAPI!" );
		return nullptr;
    }

}
