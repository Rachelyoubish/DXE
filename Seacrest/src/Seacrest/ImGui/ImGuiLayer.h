#pragma once

#include "Seacrest/Layer.h"

#include "Seacrest/Events/ApplicationEvent.h"
#include "Seacrest/Events/KeyEvent.h"
#include "Seacrest/Events/MouseEvent.h"

namespace Seacrest {

	class SEACREST_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer( bool renderDemo = true, bool renderConsole = true );
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		bool m_RenderDemo;
		bool m_RenderConsole;
	};
}