#pragma once

#include "Seacrest/Renderer/RendererAPI.h"

namespace Seacrest {

    class Direct3DRendererAPI : public RendererAPI
    {
    public:
        virtual void SetRenderTargets() override;
        virtual void SetClearColor( const DirectX::XMFLOAT4& color ) override;
        virtual void Clear() override;

        // TODO: Rework Input layout implementation to be independent
        // from drawing commands. It should only be concerned with the
        // Input-assembler stage of Direct3D 11. 
        virtual void DrawIndexed( const std::shared_ptr<InputLayout>& inputLayout ) override;
    private:
        const FLOAT* m_ClearColor;
    };
}