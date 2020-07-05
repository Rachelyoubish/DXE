#pragma once

#include "DXE/Layer.h"
#include <d3d11.h>

namespace DXE {

	class DXE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent( Event& event );
	private:
		// float m_Time = 0.0f;
	};
}