#include "scpch.h"
#include "Direct3DRendererAPI.h"

#include <DirectXMath.h>

#include "Seacrest/Application.h"

namespace Seacrest {

    void Direct3DRendererAPI::SetRenderTargets()
    {
        Application& app = Application::Get();
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext = app.GetWindow().GetGraphicsContext()->GetD3D11DeviceContext();
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRTV = app.GetWindow().GetGraphicsContext()->GetD3D11RTV();
        // Set the render target as the back buffer
        pDeviceContext->OMSetRenderTargets( 1, pRTV.GetAddressOf(), nullptr );
    }

    void Direct3DRendererAPI::SetClearColor( const DirectX::XMFLOAT4& color )
    {
        // Pass in desired color.
        m_ClearColor = (FLOAT*)&color;
    }

    void Direct3DRendererAPI::Clear()
    {
        Application& app = Application::Get();
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext = app.GetWindow().GetGraphicsContext()->GetD3D11DeviceContext();
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRTV = app.GetWindow().GetGraphicsContext()->GetD3D11RTV();
        // Clear the back buffer to desired color.
        pDeviceContext->ClearRenderTargetView( pRTV.Get(), m_ClearColor );
    }
    
    void Direct3DRendererAPI::DrawIndexed( const std::shared_ptr<InputLayout>& inputLayout )
    {
        Application& app = Application::Get();
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext = app.GetWindow().GetGraphicsContext()->GetD3D11DeviceContext();
        pDeviceContext->DrawIndexed( inputLayout->GetIndexBuffer()->GetCount(), 0u, 0u );
    }
}