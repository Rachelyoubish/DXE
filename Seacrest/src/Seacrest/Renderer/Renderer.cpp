#include "scpch.h"
#include "Renderer.h"

namespace Seacrest {

    void Renderer::BeginScene()
    {

    }

    void Renderer::EndScene()
    {

    }

    void Renderer::Submit( const std::shared_ptr<InputLayout>& inputLayout )
    {
        //inputLayout->Bind();
        RenderCommand::DrawIndexed( inputLayout );
    }
}