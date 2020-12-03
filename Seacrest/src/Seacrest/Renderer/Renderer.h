#pragma once

#include "RenderCommand.h"

namespace Seacrest {

	class Renderer
	{
	public:
		static void BeginScene(); 
		static void EndScene();

		static void Submit( const std::shared_ptr<IndexBuffer>& indexBuffer );

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}