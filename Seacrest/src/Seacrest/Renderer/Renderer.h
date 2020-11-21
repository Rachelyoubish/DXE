#pragma once

namespace Seacrest
{
	enum class RendererAPI
	{
		None = 0, Direct3D = 1
	};

	class Renderer
	{
	public:
		inline static RendererAPI GetCurrentAPI() { return s_RendererAPI; }
	private:
		static RendererAPI s_RendererAPI;
	};
}