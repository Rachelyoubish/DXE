#include "scpch.h"
#include "Direct3DTexture.h"

#include "Seacrest/Application.h"

#include "WICTextureLoader.h"

namespace Seacrest {

    Direct3DTexture2D::Direct3DTexture2D( const std::string& path )
        : m_Path( path )
    {
		Application& app = Application::Get();
		Microsoft::WRL::ComPtr<ID3D11Device> pDevice = app.GetWindow().GetGraphicsContext()->GetD3D11Device();
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext = app.GetWindow().GetGraphicsContext()->GetD3D11DeviceContext();

		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		pDevice->CreateSamplerState( &samplerDesc, &m_SamplerState );

		std::wstring stemp = std::wstring( m_Path.begin(), m_Path.end() );

		CreateWICTextureFromFile( pDevice.Get(), pDeviceContext.Get(), stemp.c_str(), &m_Resource, &m_TextureView );

		// TOAST_CORE_ASSERT( SUCCEEDED( result ), "Unable to load texture!" );

		ID3D11Texture2D* textureInterface;
		D3D11_TEXTURE2D_DESC desc;
		m_Resource->QueryInterface<ID3D11Texture2D>( &textureInterface );
		textureInterface->GetDesc( &desc );

		m_Width = desc.Width;
		m_Height = desc.Height;
		DXGI_FORMAT channels = desc.Format;
    }

    void Direct3DTexture2D::Bind( uint32_t slot ) const
    {
		Application& app = Application::Get();
		//Microsoft::WRL::ComPtr<ID3D11Device> pDevice = app.GetWindow().GetGraphicsContext()->GetD3D11Device();
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext = app.GetWindow().GetGraphicsContext()->GetD3D11DeviceContext();

		pDeviceContext->PSSetSamplers( slot, 1, m_SamplerState.GetAddressOf() );
		pDeviceContext->PSSetShaderResources( slot, 1, m_TextureView.GetAddressOf() );
    }
}