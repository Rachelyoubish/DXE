#pragma once

#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace Seacrest {

	class Renderer
	{
	public:
		static void BeginScene( OrthographicCamera& camera ); 
		static void EndScene();

		static void Submit( const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer );

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			DirectX::XMMATRIX ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};
}