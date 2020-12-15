#pragma once

#include "Seacrest/Renderer/Texture.h"

namespace Seacrest {

    class Direct3DTexture2D : public Texture2D
    {
    public:
        Direct3DTexture2D( const std::string& path );
        virtual ~Direct3DTexture2D() = default;

        virtual uint32_t GetWidth() const override { return m_Width; }
        virtual uint32_t GetHeight() const override { return m_Height; }

        virtual void Bind( uint32_t slot = 0 ) const override;
    private:
        std::string m_Path;
        uint32_t m_Width, m_Height;

        Microsoft::WRL::ComPtr<ID3D11Resource> m_Resource;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_TextureView;
        Microsoft::WRL::ComPtr<ID3D11SamplerState> m_SamplerState;
    };


}