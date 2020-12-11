#pragma once

#include "Seacrest/Renderer/RendererAPI.h"

namespace Seacrest {

    class Direct3DRendererAPI : public RendererAPI
    {
    public:
        ~Direct3DRendererAPI() = default;
        virtual void SetRenderTargets() override;
        virtual void SetClearColor( const DirectX::XMFLOAT4& color ) override;
        virtual void Clear() override;

        virtual void DrawIndexed( const Ref<IndexBuffer>& indexBuffer ) override;
    private:
        const FLOAT* m_ClearColor;
    };
}