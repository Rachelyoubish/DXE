#pragma once

#include "Seacrest/Renderer/RendererAPI.h"

namespace Seacrest {

    class Direct3DRendererAPI : public RendererAPI
    {
    public:
        ~Direct3DRendererAPI() = default;
        virtual void Init() override;

        virtual void SetRenderTargets() override;
        virtual void SetClearColor( const DirectX::XMFLOAT4& color ) override;
        virtual void Clear() override;

        virtual void DrawIndexed( const Ref<IndexBuffer>& indexBuffer ) override;
    private:
        const FLOAT* m_ClearColor;
        Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRTV;
    };
}