#include "scpch.h"
#include "Direct3DContext.h"

namespace Seacrest {

	D3DContext::D3DContext( HWND windowHandle)
		: m_WindowHandle(windowHandle)
	{
		SEACREST_CORE_ASSERT( windowHandle, "Window handle is null!" );
	}

	void D3DContext::Init()
	{
		// Create a struct to hold information about swap chain
		DXGI_SWAP_CHAIN_DESC sd = { 0 };

		// Clear out the struct for use
		ZeroMemory( &sd, sizeof( sd ) );

		// Fill the swap chain description struct
		sd.BufferCount = 2;
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 0;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = m_WindowHandle;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = true;
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		// Data
		UINT createDeviceFlags = 0;
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
		D3D_FEATURE_LEVEL featureLevel;
		const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };

		// Create a device, device context and swap chain
		// using information from the sd struct
		HRESULT result = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			createDeviceFlags,
			featureLevelArray,
			2,
			D3D11_SDK_VERSION,
			&sd,
			&m_SwapChain,
			&m_D3DDevice,
			&featureLevel,
			&m_D3DDeviceContext );
		
		SEACREST_CORE_ASSERT( SUCCEEDED(result), "Failed to create DirectX device and swap chain!" );

		LogAdapterInfo();

		CreateRenderTarget();
	}

	void D3DContext::CreateRenderTarget()
	{
		// Set the render target
		// Get the address of the back buffer
		ID3D11Texture2D* backBuffer;
		m_SwapChain->GetBuffer( 0, IID_PPV_ARGS( &backBuffer ) );

		// Use the back buffer address to create the render target
		m_D3DDevice->CreateRenderTargetView( backBuffer, nullptr, &m_RenderTargetView );
		backBuffer->Release();
	}

	void D3DContext::SetRenderTargets()
	{
		// Set the render target as the back buffer
		m_D3DDeviceContext->OMSetRenderTargets( 1, m_RenderTargetView.GetAddressOf(), nullptr );
	}

	void D3DContext::ClearScreen()
	{
		// Clear back buffer to desired color
		const float clear_color[4] = { 0.06f, 0.60f, 0.06f, 1.0f };

		// Clear the back buffer to desired color
		m_D3DDeviceContext->ClearRenderTargetView( m_RenderTargetView.Get(), (float*)clear_color );
	}

	void D3DContext::SwapBuffers()
	{
		// Switch the front and back buffer
		m_SwapChain->Present( 1, 0 );
	}

	void D3DContext::ResizeContext()
	{
		CleanupRenderTarget();
		m_SwapChain->ResizeBuffers( 0, 0, 0, DXGI_FORMAT_UNKNOWN, 0 );
		CreateRenderTarget();
	}

	ID3D11Device* D3DContext::GetD3D11Device()
	{
		return m_D3DDevice.Get();
	}
	
	ID3D11DeviceContext* D3DContext::GetD3D11DeviceContext()
	{
		return m_D3DDeviceContext.Get();
	}

	void D3DContext::CleanupRenderTarget()
	{
		if ( m_RenderTargetView )
		{
			// m_RenderTargetView->Release();
			m_RenderTargetView = nullptr;
		}
	}

	void D3DContext::LogAdapterInfo()
	{
		IDXGIFactory* factory = NULL;
		IDXGIAdapter* adapter = NULL;
		DXGI_ADAPTER_DESC adapterDesc;

		CreateDXGIFactory( __uuidof( IDXGIFactory ), (void**)&factory );

		factory->EnumAdapters( 0, &adapter );

		adapter->GetDesc( &adapterDesc );

		char videoCardDescription[128];
		std::string vendor, major, minor, release, build;
		LARGE_INTEGER driverVersion;

		wcstombs_s( NULL, videoCardDescription, 128, adapterDesc.Description, 128 );

		if ( adapterDesc.VendorId == 0x10DE )
			vendor = "NVIDIA Corporation";
		else if ( adapterDesc.VendorId == 0x1002 )
			vendor = "AMD";
		else if ( adapterDesc.VendorId == 0x8086 )
			vendor = "Intel";
		else if ( adapterDesc.VendorId == 0x1414 )
			vendor = "Microsoft";
		else
			vendor = "Unknown vendor!";

		adapter->CheckInterfaceSupport( __uuidof( IDXGIDevice ), &driverVersion );

		major = std::to_string( HIWORD( driverVersion.HighPart ) );
		minor = std::to_string( LOWORD( driverVersion.HighPart ) );
		release = std::to_string( HIWORD( driverVersion.LowPart ) );
		build = std::to_string( LOWORD( driverVersion.LowPart ) );

		SEACREST_CORE_WARN( "DirectX Information:" );
		SEACREST_CORE_INFO( "Vendor:	{0}", vendor );
		SEACREST_CORE_INFO( "Renderer:	{0}", videoCardDescription );
		SEACREST_CORE_INFO( "Version:	{0}.{1}.{2}.{3}", major, minor, release, build );
	}
}