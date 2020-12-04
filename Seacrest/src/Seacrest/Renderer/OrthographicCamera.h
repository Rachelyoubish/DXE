#pragma once

#include <DirectXMath.h>

namespace Seacrest {

    class OrthographicCamera
    {
    public:
        OrthographicCamera( float left, float right, float bottom, float top );
        ~OrthographicCamera() = default;

        const DirectX::XMFLOAT3& GetPosition() const { return m_Position; }
        void SetPosition( const DirectX::XMFLOAT3& position ) { m_Position = position; RecalculateViewMatrix(); }
        
        // Rotation along the Zed axis only, as this camera is 2D focused. 
        float GetRotation() const { return m_Rotation; }
        void SetRotation( float rotation ) { m_Rotation = rotation; RecalculateViewMatrix(); }

        const DirectX::XMMATRIX& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        const DirectX::XMMATRIX& GetViewMatrix() const { return m_ViewMatrix; }
        const DirectX::XMMATRIX& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
    private:
        // Recalculate View matrix after new values are set. 
        void RecalculateViewMatrix();
    private:
        DirectX::XMMATRIX m_ProjectionMatrix;
        DirectX::XMMATRIX m_ViewMatrix;
        DirectX::XMMATRIX m_ViewProjectionMatrix;

        DirectX::XMFLOAT3 m_Position = DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f );
        float m_Rotation = 0.0f;
    };
}