#include "scpch.h"
#include "Renderer.h"

namespace Seacrest {

    void Renderer::BeginScene()
    {

    }

    void Renderer::EndScene()
    {

    }

    void Renderer::Submit( const std::shared_ptr<IndexBuffer>& indexBuffer )
    {
        RenderCommand::DrawIndexed( indexBuffer );
    }
}