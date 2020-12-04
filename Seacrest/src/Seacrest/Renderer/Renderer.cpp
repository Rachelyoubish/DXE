#include "scpch.h"
#include "Renderer.h"

namespace Seacrest {

    Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

    void Renderer::BeginScene( OrthographicCamera& camera  )
    {
        m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {

    }

    void Renderer::Submit( const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer )
    {
        shader->Bind();
        shader->UploadConstantBuffer( "Camera", m_SceneData->ViewProjectionMatrix );
        vertexBuffer->Bind();
        indexBuffer->Bind();
        RenderCommand::DrawIndexed( indexBuffer );
    }
}