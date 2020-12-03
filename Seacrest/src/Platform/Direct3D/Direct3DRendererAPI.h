#pragma once

#include "Seacrest/Renderer/RendererAPI.h"

namespace Seacrest {

    class Direct3DRendererAPI : public RendererAPI
    {
    public:
        virtual void SetRenderTargets() override;
        virtual void SetClearColor( const DirectX::XMFLOAT4& color ) override;
        virtual void Clear() override;

        virtual void DrawIndexed( const std::shared_ptr<IndexBuffer>& indexBuffer ) override;
    private:
        const FLOAT* m_ClearColor;
    };
}