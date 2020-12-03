#pragma once

#include <DirectXMath.h>

#include "Buffer.h"

namespace Seacrest
{
    class RendererAPI
    {
    public:
        enum class API
        {
            None = 0, Direct3D = 1
        };
    public:
        virtual void SetRenderTargets() = 0;
        virtual void SetClearColor( const DirectX::XMFLOAT4& color ) = 0;
        virtual void Clear() = 0;

        virtual void DrawIndexed( const std::shared_ptr<IndexBuffer>& indexBufer ) = 0;

        inline static API GetAPI() { return s_API; }
    private:
        static API s_API;
    };
}