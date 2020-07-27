#pragma once

#include "DXE/Layer.h"

#include "DXE/Events/ApplicationEvent.h"
#include "DXE/Events/KeyEvent.h"
#include "DXE/Events/MouseEvent.h"

namespace DXE {

	class DXE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	};
}