#include "scpch.h"
#include "OrthographicCamera.h"

namespace Seacrest {

    OrthographicCamera::OrthographicCamera( float left, float right, float bottom, float top )
        : m_ProjectionMatrix( DirectX::XMMatrixOrthographicLH( ( right - left ), ( bottom - top ), -1.0f, 1.0f ) ), m_ViewMatrix( DirectX::XMMatrixIdentity() )
    {
        m_ViewProjectionMatrix = DirectX::XMMatrixMultiply( m_ViewMatrix, m_ProjectionMatrix );
    }

    void OrthographicCamera::RecalculateViewMatrix()
    {
        DirectX::XMMATRIX transform = DirectX::XMMatrixRotationZ( DirectX::XMConvertToRadians( m_Rotation ) ) * DirectX::XMMatrixTranslation( m_Position.x, m_Position.y, m_Position.z );

        m_ViewMatrix = DirectX::XMMatrixInverse( nullptr, transform );
        // Keep in mind, DirectX is row major. 
        m_ViewProjectionMatrix = DirectX::XMMatrixMultiply( m_ViewMatrix, m_ProjectionMatrix );
    }
}