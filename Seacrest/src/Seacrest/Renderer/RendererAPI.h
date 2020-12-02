#pragma once

#include <DirectXMath.h>

#include "InputLayout.h"

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

        // TODO: Rework Input layout implementation to be independent
        // from drawing commands. It should only be concerned with the
        // Input-assembler stage of Direct3D 11. 
        virtual void DrawIndexed( const std::shared_ptr<InputLayout>& inputLayout ) = 0;

        inline static API GetAPI() { return s_API; }
    private:
        static API s_API;
    };
}