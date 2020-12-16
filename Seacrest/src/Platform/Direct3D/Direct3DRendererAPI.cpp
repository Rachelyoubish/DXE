#include "scpch.h"
#include "Direct3DRendererAPI.h"

#include <DirectXMath.h>

#include "Seacrest/Application.h"

namespace Seacrest {

    void Direct3DRendererAPI::Init()
    {
        Application& app = Application::Get();
        m_Device = app.GetWindow().GetGraphicsContext()->GetD3D11Device();
        m_DeviceContext = app.GetWindow().GetGraphicsContext()->GetD3D11DeviceContext();
        pRTV = app.GetWindow().GetGraphicsContext()->GetD3D11RTV();

        ID3D11BlendState* pBlendStateNoBlend = NULL;

        D3D11_BLEND_DESC BlendState = { 0 };
        SecureZeroMemory( &BlendState, sizeof( D3D11_BLEND_DESC ) );
        BlendState.RenderTarget[0].BlendEnable = TRUE;
        BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
        BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        m_Device->CreateBlendState( &BlendState, &pBlendStateNoBlend );

        float blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        UINT sampleMask = 0xffffffff;

        m_DeviceContext->OMSetBlendState( pBlendStateNoBlend, blendFactor, sampleMask );
    }

    void Direct3DRendererAPI::SetRenderTargets()
    {
        // Set the render target as the back buffer
        m_DeviceContext->OMSetRenderTargets( 1, pRTV.GetAddressOf(), nullptr );
    }

    void Direct3DRendererAPI::SetClearColor( const DirectX::XMFLOAT4& color )
    {
        // Pass in desired color.
        m_ClearColor = (FLOAT*)&color;
    }

    void Direct3DRendererAPI::Clear()
    {
        // Clear the back buffer to desired color.
        m_DeviceContext->ClearRenderTargetView( pRTV.Get(), m_ClearColor );
    }
    
    void Direct3DRendererAPI::DrawIndexed( const Ref<IndexBuffer>& indexBuffer )
    {
        m_DeviceContext->DrawIndexed( indexBuffer->GetCount(), 0u, 0u );
    }
}